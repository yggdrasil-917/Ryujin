
#pragma once


#include "RyujinCore/Containers/String.hpp"
#include "RyujinCore/CoreUtils/BinaryReader.hpp"


namespace Ryujin
{
    enum class IoStatus : uint8
    {
        Invalid = 0,
        Pending,
        Failed,
        Cancelled,
        Succeeded,
    };
    
    typedef void (*IoAction)(class IoManager& ioManager, struct IoRequestItem& ioRequest);
    
    struct IoRequest
    {
        IoStatus status;
        uint8* data;
        PtrSize dataSize;
        String filename;
        void* caller;
        
        IoAction ioAction;
        
        IoRequest();
        ~IoRequest();
    };
    
    using IoRequestId = uint32;
    
    struct IoResult
    {
        String filename;
        BinaryReader binaryBlob;
        void* caller;
        
        IoResult(const char* inFilename, void* inCaller, uint8* inData, PtrSize inDataSize);
        ~IoResult();
    };
    
    typedef void (*AsyncLoadSuccess)(const IoResult& result);
    typedef void (*AsyncLoadBatchSuccess)(DynamicArray<IoResult>& result);
    typedef void (*AsyncLoadFailed)(const char* errorMsg);
}