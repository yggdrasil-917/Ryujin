

#include "WorkStealingQueue.hpp"



namespace Ryujin
{
    Job::Job()
    : function(nullptr),
      parent(-1),
      queueOwner(-1),
      unfinishedJobs(-1),
      data(nullptr)
    {
        
    }
    
    Job::Job(const Job& other)
    : function(other.function),
      parent(other.parent),
      unfinishedJobs(other.unfinishedJobs.load())
    {
        
    }
    
    Job& Job::operator=(const Ryujin::Job &other)
    {
        function = other.function;
        parent = other.parent;
        unfinishedJobs = other.unfinishedJobs.load();
        
        return *this;
    }
    
    WorkStealingQueue::WorkStealingQueue()
    : top(0),
      bottom(0)
    {
        Memory::MemSet(jobs, InvalidJob, sizeof(jobs));
    }
    
    WorkStealingQueue::WorkStealingQueue(WorkStealingQueue&& other)
    : top(Move(other.top)),
      bottom(Move(other.bottom))
    {
        Memory::MemCpy(jobs, other.jobs, sizeof(jobs));
    }
    
    WorkStealingQueue& WorkStealingQueue::operator=(WorkStealingQueue&& other)
    {
        bottom = Move(other.bottom);
        top = Move(other.top);
        Memory::MemCpy(jobs, other.jobs, sizeof(jobs));
        
        return *this;
    }
    
    void WorkStealingQueue::Push(JobId job)
    {
		int64 b = bottom.load(std::memory_order_acquire);
		if (b < static_cast<int64>(MAX_JOB_COUNT))
		{
			jobs[b & JOB_MASK] = job;
			bottom.store(b + 1, std::memory_order_release);
		}
    }
    
    JobId WorkStealingQueue::Pop()
    {
		int64 b = bottom.load(std::memory_order_acquire);
		b = MathUtils::Max<int64>(0, b - 1);
		bottom.store(b, std::memory_order_release);
		int64 t = top.load(std::memory_order_acquire);

		if (t <= b)
		{
			JobId job = jobs[b & JOB_MASK];
			if (t != b)
			{
				return job;
			}

			int64 expectedTop = t;
			int64 desiredTop = t + 1;
			if (!top.compare_exchange_weak(expectedTop, desiredTop, std::memory_order_acq_rel))
			{
				job = InvalidJob;
			}

			bottom.store(t + 1, std::memory_order_release);
			return job;
		}
		else
		{
			bottom.store(t, std::memory_order_release);
			return InvalidJob;
		}
    }
    
    JobId WorkStealingQueue::Steal()
    {
        int64 t = top.load(std::memory_order_acquire);
        int64 b = bottom.load(std::memory_order_acquire);
        
        if (t >= b)
        {
            return MAX_JOB_COUNT - 1;
        }
        
        
        JobId job = jobs[t & JOB_MASK];
        int64 expected = t;
        if (!std::atomic_compare_exchange_weak(&top, &expected, t + 1))
        {
            return MAX_JOB_COUNT - 1;
        }
        
        return job;
    }
}
