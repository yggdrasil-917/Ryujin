
#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP


#include "../CoreUtils/Common.hpp"
#include "../Threading/Thread.hpp"
#include "Memory.hpp"


namespace Ryujin
{
    // Fixed size ring buffer that will silently overwrite memory when it wraps around
    class RingBuffer : public OnlyRValueCopyable
    {
    private:
        AtomicVar<PtrSize> writePtr;
        AtomicVar<PtrSize> readPtr;
        PtrSize capacity;
        uint8* buffer;
        
    public:
        RingBuffer(const PtrSize inCapacity)
        {
            writePtr = 0;
            readPtr = 0;
            capacity = inCapacity;
            buffer = (uint8*)DefaultEngineAllocator::Instance().Allocate(inCapacity, 1);
        }
        
        RingBuffer(RingBuffer&& other)
        {
            writePtr = other.writePtr.GetValue();
            capacity = other.capacity;
            buffer = Move(other.buffer);
        }
        
        RingBuffer& operator=(RingBuffer&& other)
        {
            writePtr = other.writePtr.GetValue();
            capacity = other.capacity;
            buffer = Move(other.buffer);
            
            return *this;
        }
        
        ~RingBuffer()
        {
            SAFE_DELETE(buffer);
        }
        
        
        void Write(uint8* data, PtrSize size)
        {
            PtrSize writeLocation = (writePtr.GetValue() + size) % capacity;
            
            Memory::MemCpy((buffer + writeLocation), data, size);
            
            writePtr.Store(writeLocation);
        }
        
        
        uint8* Read(PtrSize size)
        {
            PtrSize readLocation = (readPtr.GetValue() + size) % capacity;
            uint8* data = (buffer + readLocation);
            
            readPtr.Store(readLocation);
            
            return data;
        }
        
        
        FORCEINLINE PtrSize GetCapacity() const { return capacity; }
    };
}



#endif
