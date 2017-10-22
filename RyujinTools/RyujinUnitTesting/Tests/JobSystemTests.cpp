

#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"

#include "RyujinCore/Threading/JobSystem.hpp"



namespace Ryujin
{
    TEST_CASE(ParallelForTest, "Test for parallel for loops")
    {
		JobSystem jobSystem;
		jobSystem.Initialize(Threading::NumConcurrentThreadsSupported() - 1);
        
        DynamicArray<int32> arr;
        for (uint32 i = 0; i < 100; ++i)
            arr.Add(i);
        int32 arrCount = arr.GetSize();
        AtomicVar<PtrSize> counter(0);
        PtrSize correct = arr.GetSize();
        const JobId root = jobSystem.ParallelFor<int32>(arr.GetTypedData(), arrCount, &counter,
                                                        [](uint8* data, uint32 count, void* other)
                                                        {
                                                            int32* a = (int32*)data;
                                                            AtomicVar<int32>* counter = (AtomicVar<int32>*)other;
                                                            for (uint32 i = 0; i < count; ++i)
                                                            {
                                                                counter->Increment(1);
//                                                                Debugf(JobSystem, "arr[%d] = %d thread id %llu", i, a[i], Threading::GetThreadID());
                                                            }
                                                        },
                                                        [](uint32 count, uint32 elementSize) -> bool
                                                        {
                                                            return count > 2;
                                                        });
        
        jobSystem.Schedule(root);
        jobSystem.Wait(root);
        
        jobSystem.Shutdown();
        
        REQUIRE(counter == correct);
    }
    
    void JobF(Job* job, void* data)
    {
        static std::atomic<int32> i(0);
        ++i;
        Debugf(Info, "JobF %p thread id %llu", job, Threading::GetThreadID());
        uint64 spinCount = 1024*1024*10;
        while (spinCount-- > 0) {}
    }
    
    struct JobTest
    {
        AtomicVar<int32>* counter;
        int32 i;
    };
    
    TEST_CASE(CreateJobTest, "Test for creating jobs and running them")
    {
        JobSystem jobSystem;
        jobSystem.Initialize(Threading::NumConcurrentThreadsSupported() - 1);
        
        const JobId root = jobSystem.CreateJob(JobF);
        AtomicVar<int32> counter(0);
        for (int32 i = 0; i < 32; ++i)
        {
            JobTest jobTest;
            jobTest.i = i;
            jobTest.counter = &counter;
            const JobId job = jobSystem.CreateJobAsChild(root,
                                                         [](Job* job, void* data)
                                                         {
                                                             JobTest* val = job->GetTypedJobData<JobTest>();
                                                             val->counter->Increment(1);
															 uint64 spinCount = MathUtils::RandomFromRange<uint64>(1024, 1024 * 1024);
															 while (spinCount-- > 0) {}
                                                             //Debugf(Info, "JobF %p value %d thread id %llu", job, val->i, Threading::GetThreadID());
                                                         },
                                                         &jobTest,
                                                         sizeof(JobTest));
            jobSystem.Schedule(job);
        }
        
        jobSystem.Schedule(root);
        jobSystem.Wait(root);
        
        jobSystem.Shutdown();
        REQUIRE(counter == 32);
    }
}
