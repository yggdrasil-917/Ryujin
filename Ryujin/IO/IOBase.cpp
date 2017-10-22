
#include "IOBase.hpp"



namespace Ryujin
{
    IoRequest::IoRequest()
    : status(IoStatus::Invalid),
      data(nullptr),
      dataSize(0),
      caller(nullptr),
      ioAction(nullptr)
    {
        
    }
    
    IoRequest::~IoRequest()
    {
        SAFE_DELETE(data);
        status = IoStatus::Invalid;
    }
    
    IoResult::IoResult(const char* inFilename, void* inCaller, uint8* inData, PtrSize inDataSize)
    : filename(inFilename),
      binaryBlob(BinaryReader(inData, inDataSize, true)),
      caller(inCaller)
    {
        
    }
    
    IoResult::~IoResult()
    {
        
    }
}
