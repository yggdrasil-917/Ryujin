

#include "IO.hpp"

#include "RyujinCore/OS/FileSystem.hpp"
#include "RyujinCore/Allocators/GlobalAllocators.hpp"

namespace Ryujin
{
    IoManager::IoManager()
    : cancelRequestCount(0),
      currentRequest(0),
      requestCount(0),
      bRequestedShutdown(false)
    {
        
    }
    
    
    IoManager::~IoManager()
    {
        
    }
    
    void IoManager::Initialize()
    {
        ioThread = (
                           [this]()
                           {
                               while (true)
                               {
                                   ScopedLock lock(this->queueMutex);
                                   cv.Wait(lock, [this]() { return this->bRequestedShutdown || this->GetPendingRequestsCount() != 0; });
                                   
                                   if (this->bRequestedShutdown)
                                       return;
                                   
                                   int32 cancelIndex = 0;
                                   int32 cancelRequests = this->cancelRequestCount.load();
                                   int32 cancelRequestsIgnored = 0;
                                   while (cancelRequests > 0 && cancelIndex < cancelRequests)
                                   {
                                       IoRequestId& id = this->ioCancelRequests[cancelIndex & IO_MASK];
                                       if (this->ioRequests[id].ioRequest.status == IoStatus::Pending)
                                       {
                                           this->ioRequests[id].ioRequest.status = IoStatus::Cancelled;
                                           Debugf(IO, "Cancelled IO request for %s", *this->ioRequests[id].ioRequest.filename);
                                       }
                                       else
                                       {
                                           Debugf(IO, "Ignoring cancel request for %s as it already finished or failed", *this->ioRequests[id].ioRequest.filename);
                                           ++cancelRequestsIgnored;
                                       }
                                       ++cancelIndex;
                                   }
                                   if (cancelRequests > 0)
                                   {
                                       this->cancelRequestCount.fetch_sub(cancelRequests);
                                       this->requestCount.fetch_sub(cancelRequests - cancelRequestsIgnored);
                                   }
                                   
                                   uint32 index = this->currentRequest.fetch_add(1) - 1u;
                                   IoRequestItem& item = this->ioRequests[index & IO_MASK];
                                   if (item.ioRequest.status == IoStatus::Pending)
                                   {
                                       if (item.ioRequest.ioAction)
                                       {
                                           item.ioRequest.ioAction(*this, item);
                                       }
                                       else
                                       {
                                           // request was probably not finished being setup so we defer processing it
                                           this->currentRequest.fetch_sub(1);
                                           Debugf(IO, "Deferring io request");
                                       }
                                   }
                               }
                           }
        );
    }
    
    void IoManager::Shutdown()
    {
        bRequestedShutdown = true;
        cv.UnblockAllThreads();
        ioThread.Finish();
        
        for (auto& request : ioRequests)
        {
            SAFE_DELETE_ARRAY(request.ioRequest.data);
        }
    }
    
    void IoManager::AsyncLoad(const char *filename, void* caller, AsyncLoadSuccess success, AsyncLoadFailed failed)
    {
        ++requestCount;
        IoRequestId slot = requestCount.load() - 1u;
        IoRequestItem& item = ioRequests[slot & IO_MASK];
        item.ioRequest.status = IoStatus::Pending;
        item.ioRequest.filename = filename;
        item.ioRequest.caller = caller;
        item.ioRequest.ioAction = [](IoManager& ioManager, IoRequestItem& ioRequest)
        {
            IoRequest& request = ioRequest.ioRequest;
            FileHandle file = File::Open(*request.filename, FileMode::FM_Read, true);
            if (!file)
            {
                request.status = IoStatus::Failed;
                char err[512];
                String::Sprintf(err, "AsyncLoad failed to open \"%s\"", *request.filename);
                ioRequest.failed(err);
                return;
            }
            
            PtrSize fileSize = File::FileSize(file);
            uint8* data = RYUJIN_NEW uint8[fileSize];
            PtrSize bytesRead = File::Read(data, sizeof(uint8), fileSize / sizeof(uint8), file);
            if (bytesRead != fileSize)
            {
                SAFE_DELETE_ARRAY(data);
                request.status = IoStatus::Failed;
                char err[512];
                String::Sprintf(err, "AsyncLoad failed to read \"%s\"", *request.filename);
                ioRequest.failed(err);
                return;
            }
            
            request.status = IoStatus::Succeeded;
            IoResult ioResult(*request.filename, request.caller, data, fileSize);
            ioRequest.success(ioResult);
            ioManager.requestCount.fetch_sub(1);
        };
        
        item.success = success;
        item.failed = failed;
        
        cv.UnblockOneThread();
    }
    
    void IoManager::AsyncLoadBatch(const DynamicArray<String>& filenames, AsyncLoadBatchSuccess success, AsyncLoadFailed failed)
    {
        
    }
    
    const IoRequestId IoManager::AsyncLoad(const char* filename)
    {
        ++requestCount;
        IoRequestId slot = requestCount.load() - 1u;
        IoRequestItem& item = ioRequests[slot & IO_MASK];
        item.ioRequest.status = IoStatus::Pending;
        item.ioRequest.filename = filename;
        item.ioRequest.ioAction = [](IoManager& ioManager, IoRequestItem& ioRequest)
        {
            IoRequest& request = ioRequest.ioRequest;
            FileHandle file = File::Open(*request.filename, FileMode::FM_Read, true);
            if (!file)
            {
                request.status = IoStatus::Failed;
                char err[512];
                String::Sprintf(err, "AsyncLoad failed to open \"%s\"", *request.filename);
                return;
            }
            
            PtrSize fileSize = File::FileSize(file);
            uint8* data = RYUJIN_NEW uint8[fileSize];
            PtrSize bytesRead = File::Read(data, sizeof(uint8), fileSize / sizeof(uint8), file);
            if (bytesRead != fileSize)
            {
                SAFE_DELETE_ARRAY(data);
                request.status = IoStatus::Failed;
                char err[512];
                String::Sprintf(err, "AsyncLoad failed to read \"%s\"", *request.filename);
                return;
            }
            
            ioManager.requestCount.fetch_sub(1);
            request.status = IoStatus::Succeeded;
            request.data = data;
            request.dataSize = fileSize;
        };
        
        item.success = nullptr;
        item.failed = nullptr;
        
        cv.UnblockOneThread();
        return slot;
    }
    
    IoRequest* IoManager::WaitForLoad(const IoRequestId ioRequestId)
    {
        IoRequest* request;
        while (!(request = HasIoRequestFinished(ioRequestId)))
        {
            Threading::YieldThread();
        }
        
        return request;
    }
    
    IoRequest* IoManager::HasIoRequestFinished(const IoRequestId ioRequest)
    {
        IoRequestItem& item = ioRequests[ioRequest & IO_MASK];
        if (item.ioRequest.status != IoStatus::Pending)
        {
            return &item.ioRequest;
        }
        
        return nullptr;
    }
    
    void IoManager::AsyncCancel(const IoRequestId requestToCancel)
    {
        ++cancelRequestCount;
        IoRequestId slot = cancelRequestCount.load() - 1u;
        IoRequestId& item = ioCancelRequests[slot & IO_MASK];
        if (ioRequests[item].ioRequest.status != IoStatus::Pending)
        {
            cancelRequestCount.fetch_sub(1);
            return;
        }
        item = requestToCancel;
    }
}




