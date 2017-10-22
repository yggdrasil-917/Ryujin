
#pragma once


#include "Semaphore.hpp"
#include "WorkStealingQueue.hpp"


namespace Ryujin
{
    
    // This should be thread local but Apple does not support TLS
    class CORE_API JobAllocator
    {
    private:
        Job jobAllocator[MAX_JOB_COUNT];
        std::atomic<uint32> allocatedJobs;
        
    public:
        JobAllocator();
        JobAllocator(const JobAllocator& other);
        JobAllocator& operator=(const JobAllocator& other);
        Job* AllocateJob(JobId& jobId);
        
        Job* GetJobPtr(JobId id);
    };
    
    class CORE_API JobSystemWorkerThread
    {
    private:
        Thread thread;
        std::atomic<bool> bWorkerThreadActive;
        class JobSystem* owner;
        
    public:
        JobSystemWorkerThread();
        JobSystemWorkerThread(JobSystemWorkerThread&& other);
        JobSystemWorkerThread(const JobSystemWorkerThread& other);
        
        JobSystemWorkerThread& operator=(const JobSystemWorkerThread& other);
        JobSystemWorkerThread& operator=(JobSystemWorkerThread&& other);
        
        void Start(JobSystem* inOwner);
        void Stop();
		void Finish();
    };
    
    
    
    extern CORE_API void ParallelForJob(Job* job, void* data);
    
    class CORE_API JobSystem
    {
        friend class JobSystemWorkerThread;
        
    private:
        DynamicArray<JobAllocator> jobAllocator;
        DynamicArray<WorkStealingQueue> jobQueues;
        DynamicArray<JobSystemWorkerThread> workers;
        AdaptiveSemaphore semaphore;

		void ResetJob(Job* job);
		void Execute(JobId job);
		JobId GetJob();

    public:
        JobSystem();
        ~JobSystem();
        
        void Initialize(int32 numThreadsToMake);
        void Shutdown();
        
		JobId CreateJob(JobFunction function, void* jobData = nullptr, int32 dataSize = 0);
		JobId CreateJobAsChild(JobId parent, JobFunction function, void* jobData = nullptr, int32 dataSize = 0);
		JobId CreateJobAsChild(Job* parent, JobFunction function, void* jobData = nullptr, int32 dataSize = 0);
        
        void Schedule(JobId job);
        void Finish(JobId job);
        void Wait(JobId job);
        
        bool HasJobCompleted(Job* job) const;
        
        
        template <typename T>
        const JobId ParallelFor(T* data, uint32 count, void* other, ParallelForJobFunc parallelForJobFunc, SplitterFunc splitterFunc)
        {
            ParallelForJobData jobData((uint8*)data, count, other, sizeof(T), parallelForJobFunc, splitterFunc);
            const JobId jobId = CreateJob(&ParallelForJob, &jobData, sizeof(jobData));
            
            return jobId;
        }
    };
    
}






