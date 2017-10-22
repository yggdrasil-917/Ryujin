
#pragma once



#include "Thread.hpp"
#include "../Containers/DynamicArray.hpp"


namespace Ryujin
{
    
#define MAX_JOB_COUNT   4096
#define JOB_MASK        (MAX_JOB_COUNT - 1u)
    RYUJIN_STATIC_ASSERT((MAX_JOB_COUNT & JOB_MASK) == 0, "MAX_JOB_COUNT must be a power of two");
    
    
    using ParallelForJobFunc = void (*)(uint8* data, uint32 count, void* other);
    
    typedef void (*JobFunction)(struct Job*, void* data);
    using JobId = uint32;
    
    // Do 64 byte padding so as to avoid false sharing on 64 byte cache lines
//#pragma pack(push, 1)
    struct Job
    {
        JobFunction function;
		JobId parent;
		int32 queueOwner;
		std::atomic<uint32> unfinishedJobs;
        void* data;
		static constexpr int32 jobPayloadSize = sizeof(JobFunction) +
												sizeof(JobId) +
												sizeof(int32) +
												sizeof(std::atomic<int32>) +
												sizeof(void*);
        char padding[64 - jobPayloadSize];
        
        Job();
        Job(const Job& other);
        Job& operator=(const Job& other);
        
        template <typename T>
        T* GetTypedJobData()
        {
            return data != nullptr ? static_cast<T*>(data) : reinterpret_cast<T*>(padding);
        }
    };
//#pragma pack(pop)

	FORCEINLINE int32 GetJobQueueOwnerFromId(JobId jobId)
	{
		return (jobId & 0xFF000000) >> 24;
	}
    
    FORCEINLINE int32 DecodeJobIndex(JobId jobId)
    {
        return (jobId & 0x00FFFFFF);
    }
    
    
    using SplitterFunc = bool (*)(uint32 count, uint32 elementSize);
    
    struct ParallelForJobData
    {
        ParallelForJobData(uint8* data, uint32 count, void* other, uint32 elementSize, ParallelForJobFunc function, SplitterFunc splitter)
        : data(data),
        count(count),
        other(other),
        elementSize(elementSize),
        function(function),
        splitterFunc(splitter)
        {
        }
        
        uint8* data;
        uint32 count;
        void* other;
        uint32 elementSize;
        ParallelForJobFunc function;
        SplitterFunc splitterFunc;
    };
    
#define InvalidJob -1
    
    
    class WorkStealingQueue
    {
    private:
        JobId jobs[MAX_JOB_COUNT];
        
        volatile std::atomic<int64> top;
        volatile std::atomic<int64> bottom;
        
    public:
        WorkStealingQueue();
        
        WorkStealingQueue(WorkStealingQueue&& other);
        
        WorkStealingQueue& operator=(WorkStealingQueue&& other);
        
        void Push(JobId job);
        JobId Pop();
        JobId Steal();
        
        FORCEINLINE uint32 GetCount() const { return (uint32)(bottom - top); }
    };
}
