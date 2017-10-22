
#ifndef BINARY_READER_HPP
#define BINARY_READER_HPP


#include "../Logger/Logger.hpp"
#include "../CoreUtils/Common.hpp"
#include "../OS/FileSystem.hpp"


namespace Ryujin
{
    class CORE_API BinaryFileReader : OnlyRValueCopyable
    {
    private:
        FileHandle file;
        
    public:
        BinaryFileReader();
        BinaryFileReader(const char* filename);
        ~BinaryFileReader();
        
        bool Open(const char* filename, FileMode fileMode = FileMode::FM_Read);
        void SetPosition(const PtrSize offset);
        void Close();
        
        FORCEINLINE bool IsOpen() const { return file != nullptr; }
        
        template<typename T>
        FORCEINLINE void NextFrom(T* outBuffer, int32 count, PtrSize offset)
        {
            SetPosition(offset);
            Next(outBuffer, count);
        }
        
        template<typename T>
        FORCEINLINE void Next(T* outBuffer, int32 count)
        {
            fread(outBuffer, sizeof(T), count, file);
        }
    };
    
    class CORE_API BinaryReader
    {
    private:
        ubyte* data;
        const ubyte* readPtr;
        PtrSize size;
        bool bOwnsBinaryData;
        
    public:
        BinaryReader(const char* filename);
        BinaryReader(const void* inData, PtrSize inSize, bool bTakeOwnershipOfData = false);
        ~BinaryReader();
        
        void* NextRawData(PtrSize dataSize);
        
        template<typename T>
        FORCEINLINE T Next()
        {
            T next = *(T*)(readPtr);
            readPtr += sizeof(T);
            
            return next;
        }
        
        template<typename T>
        FORCEINLINE T* NextArray(int32 count)
        {
            T* next = (T*)(readPtr);
            readPtr += sizeof(T) * count;
            
            return next;
        }
        
        FORCEINLINE ubyte* GetData() const { return data; }
        FORCEINLINE PtrSize GetSize() const { return size; }
        FORCEINLINE PtrSize GetPosition() const
        {
            return (readPtr - data);
        }
        
        FORCEINLINE void SetPosition(PtrSize offset)
        {
            RYUJIN_ASSERT(offset <= size, "Offset is too large");
            
            readPtr = (data + offset);
        }
    };
}



#endif
