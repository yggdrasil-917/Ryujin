

#include "JobSystem.hpp"
#include "../Math/MathUtils.hpp"
#include "../Allocators/Memory.hpp"

namespace Ryujin
{
    static std::atomic<int32> workerThreadCounter(0);
    static THREAD_LOCAL_VAR(int32, tlsWorkerThreadIndex(0));
    
    static int32 GetWorkerThreadIndex()
    {
        return tlsWorkerThreadIndex;
    }
    
    JobAllocator::JobAllocator()
    : allocatedJobs(0)
    {
    }
    
    JobAllocator::JobAllocator(const JobAllocator& other)
    : allocatedJobs(other.allocatedJobs.load())
    {
        Memory::MemCpy(jobAllocator, (void*)&other.jobAllocator[0], sizeof(Job) * MAX_JOB_COUNT);
    }
    
    JobAllocator& JobAllocator::operator=(const Ryujin::JobAllocator &other)
    {
        allocatedJobs = other.allocatedJobs.load();
        Memory::MemCpy(jobAllocator, (void*)&other.jobAllocator[0], sizeof(Job) * MAX_JOB_COUNT);
        
        return *this;
    }
    
    Job* JobAllocator::AllocateJob(JobId& jobId)
    {
        ++allocatedJobs;
        uint32 index = allocatedJobs.load() - 1u;
        jobId = index;
        return &jobAllocator[index & JOB_MASK];
    }
    
    
    Job* JobAllocator::GetJobPtr(JobId id)
    {
        int32 index = DecodeJobIndex(id);
        RYUJIN_ASSERT(index >= 0 && index < MAX_JOB_COUNT, "Index out of bounds");
        return &jobAllocator[index];
    }
    
    JobSystemWorkerThread::JobSystemWorkerThread()
    : bWorkerThreadActive(false)
    {
        
    }
    
    JobSystemWorkerThread::JobSystemWorkerThread(JobSystemWorkerThread&& other)
    {
        bWorkerThreadActive = other.bWorkerThreadActive.load();
        owner = other.owner;
        thread = Move(other.thread);
        
        other.owner = nullptr;
    }
    
    JobSystemWorkerThread::JobSystemWorkerThread(const JobSystemWorkerThread& other)
    : bWorkerThreadActive(other.bWorkerThreadActive.load())
    {
        
    }
    
    JobSystemWorkerThread& JobSystemWorkerThread::operator=(const JobSystemWorkerThread& other)
    {
        bWorkerThreadActive = other.bWorkerThreadActive.load();
        
        return *this;
    }
    
    JobSystemWorkerThread& JobSystemWorkerThread::operator=(JobSystemWorkerThread&& other)
    {
        bWorkerThreadActive = other.bWorkerThreadActive.load();
        owner = other.owner;
        thread = Move(other.thread);
        
        other.owner = nullptr;
        
        return *this;
    }
    
    void JobSystemWorkerThread::Start(JobSystem* inOwner)
    {
        if (bWorkerThreadActive)
            return;
        
        bWorkerThreadActive = true;
        owner = inOwner;
        
        Thread t(
                 [this]
                 {
                     workerThreadCounter.fetch_add(1);
                     tlsWorkerThreadIndex = workerThreadCounter.load();
                     owner->semaphore.Wait();
                     while (bWorkerThreadActive)
                     {
                         const JobId jobId = owner->GetJob();
                         if (jobId == InvalidJob)
                         {
                             owner->semaphore.Wait();
                         }
                         else
						 {
							 const int32 workerThreadAllocatorIndex = GetJobQueueOwnerFromId(jobId);
                             Job* job = owner->jobAllocator[workerThreadAllocatorIndex].GetJobPtr(jobId);
                             if (!owner->HasJobCompleted(job))
                             {
                                 owner->Execute(jobId);
                             }
                             else
                             {
                                 owner->semaphore.Wait();
                             }
                         }
                     }
                     
                     const JobId jobId = owner->GetJob();
                     if (jobId != InvalidJob)
					 {
						 const int32 workerThreadAllocatorIndex = GetJobQueueOwnerFromId(jobId);
                         Job* job = owner->jobAllocator[workerThreadAllocatorIndex].GetJobPtr(jobId);
                         if (!owner->HasJobCompleted(job))
                         {
                             owner->Execute(jobId);
                         }
                     }
                 }
                 );
        thread = Move(t);
    }
    
    void JobSystemWorkerThread::Stop()
    {
        bWorkerThreadActive = false;
    }

	void JobSystemWorkerThread::Finish()
	{
		thread.Finish();
	}
    
    
    JobSystem::JobSystem()
    : semaphore(0)
    {
        
    }
    
    JobSystem::~JobSystem()
    {
        
    }
    
    void JobSystem::Initialize(int32 numThreadsToMake)
    {
        workers.Resize(numThreadsToMake);
        jobQueues.Resize(numThreadsToMake + 1);
        jobAllocator.Resize(numThreadsToMake + 1);
        for (int32 i = 0; i < numThreadsToMake; ++i)
        {
            jobAllocator.Add(JobAllocator());
            jobQueues.Add(WorkStealingQueue());
            workers.Add(JobSystemWorkerThread());
        }
        jobAllocator.Add(JobAllocator());
        jobQueues.Add(WorkStealingQueue());
        
        for (int32 i = 0; i < numThreadsToMake; ++i)
        {
            workers[i].Start(this);
        }
    }
    
    void JobSystem::Shutdown()
    {
		// Inform worker threads that they will finish
        for (auto& worker : workers)
        {
            worker.Stop();
        }

		// Wake up all the worker threads
		semaphore.Signal(workers.GetSize());

		// Finish any remaining work and join the thread
		for (auto& worker : workers)
		{
			worker.Finish();
		}
        
        workerThreadCounter = 0;
    }
    
    JobId JobSystem::CreateJob(JobFunction function, void* jobData, int32 dataSize)
    {
		JobId jobId = 0;
		uint32 workerThreadIndex = GetWorkerThreadIndex();
        Job* job = jobAllocator[workerThreadIndex].AllocateJob(jobId);
        job->function = function;
        job->parent = InvalidJob;
        job->queueOwner = workerThreadIndex;
        job->unfinishedJobs = 1;

		// Encode owning allocator inside the jobId (use 8 bits for it)
		jobId = ((workerThreadIndex << 24) & 0xFF000000) | jobId;
        
        if (dataSize > 0)
        {
            if (dataSize <= sizeof(job->padding))
            {
                Memory::MemCpy(job->padding, jobData, dataSize);
                job->data = nullptr;
            }
            else
            {
                if (jobData)
                {
                    job->data = Memory::Malloc(dataSize);
                    Memory::MemCpy(job->data, jobData, dataSize);
                }
                else
                {
                    job->data = nullptr;
                }
            }
        }
        
        return jobId;
    }
    
    JobId JobSystem::CreateJobAsChild(JobId parent, JobFunction function, void* jobData, int32 dataSize)
	{
		uint32 workerThreadIndex = GetWorkerThreadIndex();
		int32 parentJobQueue = GetJobQueueOwnerFromId(parent);
        Job* parentPtr = jobAllocator[parentJobQueue].GetJobPtr(parent);
        parentPtr->unfinishedJobs++;
        
        JobId jobId = 0;
        Job* job = jobAllocator[workerThreadIndex].AllocateJob(jobId);
        job->function = function;
        job->parent = parent;
        job->queueOwner = workerThreadIndex;
		job->unfinishedJobs = 1;

		// Encode owning allocator inside the jobId (use 8 bits for it)
		jobId = ((workerThreadIndex << 24) & 0xFF000000) | jobId;
        
        if (dataSize > 0)
        {
            if (dataSize <= sizeof(job->padding))
            {
                Memory::MemCpy(job->padding, jobData, dataSize);
                job->data = nullptr;
            }
            else
            {
                if (jobData)
                {
                    job->data = Memory::Malloc(dataSize);
                    Memory::MemCpy(job->data, jobData, dataSize);
                }
                else
                {
                    job->data = nullptr;
                }
            }
        }
        
        return jobId;
    }
    
    JobId JobSystem::CreateJobAsChild(Job* parent, JobFunction function, void* jobData, int32 dataSize)
    {
		parent->unfinishedJobs++;
		uint32 workerThreadIndex = GetWorkerThreadIndex();
        
        JobId jobId = 0;
        Job* job = jobAllocator[workerThreadIndex].AllocateJob(jobId);
        job->function = function;
        job->parent = (JobId)((uintptr)parent - (uintptr)jobAllocator[parent->queueOwner].GetJobPtr(0)) / sizeof(Job);
        job->parent = ((parent->queueOwner << 24) & 0xFF000000) | job->parent;
        job->queueOwner = workerThreadIndex;
        job->unfinishedJobs = 1;

		// Encode owning allocator inside the jobId (use 8 bits for it)
		jobId = ((workerThreadIndex << 24) & 0xFF000000) | jobId;
        
        if (dataSize > 0)
        {
            if (dataSize <= sizeof(job->padding))
            {
                Memory::MemCpy(job->padding, jobData, dataSize);
                job->data = nullptr;
            }
            else
            {
                if (jobData)
                {
                    job->data = Memory::Malloc(dataSize);
                    Memory::MemCpy(job->data, jobData, dataSize);
                }
                else
                {
                    job->data = nullptr;
                }
            }
        }
        
        return jobId;
    }
    
    void JobSystem::Schedule(JobId job)
    {
        jobQueues[GetWorkerThreadIndex()].Push(job);
        semaphore.Signal(1);
    }
    
    void JobSystem::Execute(JobId jobId)
    {
		const int32 threadAllocatorIndex = GetJobQueueOwnerFromId(jobId);
        Job* job = jobAllocator[threadAllocatorIndex].GetJobPtr(jobId);
        RYUJIN_ASSERT(job->function, "Null function pointer");
        
        (job->function)(job, this);
        Finish(jobId);
    }
    
    void JobSystem::Finish(JobId jobId)
	{
		const int32 threadAllocatorIndex = GetJobQueueOwnerFromId(jobId);
        Job* job = jobAllocator[threadAllocatorIndex].GetJobPtr(jobId);
        job->unfinishedJobs--;
        
        if (HasJobCompleted(job))
        {
            if (job->parent != InvalidJob)
            {
                Finish(job->parent);
            }
            
            ResetJob(job);
        }
    }
    
    void JobSystem::Wait(JobId jobId)
	{
		const int32 threadAllocatorIndex = GetJobQueueOwnerFromId(jobId);
        // wait until the job has completed. in the meantime, work on any other job.
        Job* job = jobAllocator[threadAllocatorIndex].GetJobPtr(jobId);
        while (!HasJobCompleted(job))
        {
            const JobId nextJob = GetJob();
            if (nextJob != InvalidJob)
                Execute(nextJob);
        }
    }
    
    void JobSystem::ResetJob(Job* job)
    {
        if (!job)
            return;
        
        if (job->queueOwner != InvalidJob && job->data)
        {
            Memory::Free(job->data);
            job->data = nullptr;
        }
        
        job->function = nullptr;
        job->parent = InvalidJob;
        job->queueOwner = -1;
        job->unfinishedJobs = 0;
    }
    
    JobId JobSystem::GetJob()
    {
        JobId jobId = jobQueues[GetWorkerThreadIndex()].Pop();
        
        if (jobId == InvalidJob)
        {
            return jobId;
        }
        
        Job* job = jobAllocator[GetJobQueueOwnerFromId(jobId)].GetJobPtr(jobId);
        if (HasJobCompleted(job)) // job is empty
        {
            uint32 largestJobCount = 0;
            uint32 index = 0;
            for (uint32 i = 0; i < jobQueues.GetSize(); ++i)
            {
                if (jobQueues[i].GetCount() > largestJobCount)
                {
                    largestJobCount = jobQueues[i].GetCount();
                    index = i;
                }
            }
            
            JobId stolenJobId = jobQueues[index].Steal();
            Job* stolenJob = jobAllocator[index].GetJobPtr(stolenJobId);
            if (HasJobCompleted(stolenJob))
            {
                Threading::YieldThread();
                return InvalidJob;
            }
            
            return stolenJobId;
        }
        
        return jobId;
    }
    
    bool JobSystem::HasJobCompleted(Job* job) const
    {
        return job->unfinishedJobs == 0;
    }
    
    void ParallelForJob(Job *job, void* data)
    {
        JobSystem* jobSystem = (JobSystem*)data;
        ParallelForJobData* jobData = job->GetTypedJobData<ParallelForJobData>();
        SplitterFunc splitter = jobData->splitterFunc;
        
        if (splitter(jobData->count, jobData->elementSize))
        {
            const uint32 leftCount = jobData->count / 2u;
            ParallelForJobData leftData(jobData->data, leftCount, jobData->other, jobData->elementSize, jobData->function, splitter);
            JobId left = jobSystem->CreateJobAsChild(job, &ParallelForJob, &leftData, sizeof(leftData));
            jobSystem->Schedule(left);
            
            const uint32 rightCount = jobData->count - leftCount;
            ParallelForJobData rightData(jobData->data + leftCount * jobData->elementSize, rightCount, jobData->other, jobData->elementSize, jobData->function, splitter);
            JobId right = jobSystem->CreateJobAsChild(job, &ParallelForJob, &rightData, sizeof(rightData));
            jobSystem->Schedule(right);
        }
        else
        {
            // execute the function on the range of data
            (jobData->function)(jobData->data, jobData->count, jobData->other);
        }
    }
}








