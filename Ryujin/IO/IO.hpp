
#pragma once


#include "IOBase.hpp"

#include "RyujinCore/Containers/FixedArray.hpp"


namespace Ryujin
{
#define MAX_IO_REQUESTS 256
#define IO_MASK         (MAX_IO_REQUESTS - 1u)
    RYUJIN_STATIC_ASSERT((MAX_IO_REQUESTS & IO_MASK) == 0, "MAX_IO_REQUESTS must be a power of two");
    
    struct IoRequestItem
    {
        IoRequest ioRequest;
        AsyncLoadSuccess success;
        AsyncLoadFailed failed;
    };
    
    class IoManager
    {
    private:
        FixedArray<IoRequestId, MAX_IO_REQUESTS> ioCancelRequests;
        std::atomic<int32> cancelRequestCount;
        FixedArray<IoRequestItem, MAX_IO_REQUESTS> ioRequests;
        std::atomic<int32> currentRequest;
        std::atomic<int32> requestCount;
        std::atomic<bool> bRequestedShutdown;
        Thread ioThread;
        Mutex queueMutex;
        ConditionVariable cv;
        
    public:
        IoManager();
        ~IoManager();
        
        void Initialize();
        void Shutdown();
        
        // Start async load with callbacks
        void AsyncLoad(const char* filename, void* caller, AsyncLoadSuccess success, AsyncLoadFailed failed);
        void AsyncLoadBatch(const DynamicArray<String>& filenames, AsyncLoadBatchSuccess success, AsyncLoadFailed failed);
        
        // Start async loading file
        // returns a ref to the io request
        const IoRequestId AsyncLoad(const char* filename);
        IoRequest* WaitForLoad(const IoRequestId ioRequestId);
        
        void AsyncCancel(const IoRequestId requestToCancel);
        
        IoRequest* HasIoRequestFinished(const IoRequestId ioRequest);
        
        FORCEINLINE int32 GetPendingRequestsCount() const { return requestCount.load(); }
    };
}
