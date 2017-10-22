

#include "BinaryReader.hpp"


namespace Ryujin
{
    BinaryFileReader::BinaryFileReader()
    : file(nullptr)
    {
        
    }
    
    BinaryFileReader::BinaryFileReader(const char* filename)
    {
        Open(filename);
    }
    
    BinaryFileReader::~BinaryFileReader()
    {
        Close();
    }
    
    bool BinaryFileReader::Open(const char* filename, FileMode fileMode)
    {
        file = File::Open(filename, fileMode, true);
        return file != nullptr;
    }
    
    void BinaryFileReader::SetPosition(const PtrSize offset)
    {
        if (file)
        {
            fseek(file, (long)offset, SEEK_SET);
        }
    }
    
    void BinaryFileReader::Close()
    {
        if (file)
        {
            File::Close(file);
            file = nullptr;
        }
    }
    
    BinaryReader::BinaryReader(const char* filename)
    {
        FileHandle file = File::Open(filename, FileMode::FM_Read, true);
        if (file)
        {
            bOwnsBinaryData = true;
            size = File::FileSize(file);
            data = RYUJIN_NEW ubyte[size];
            File::Read(data, sizeof(ubyte), size, file);
            readPtr = data;
        }
        else
        {
            bOwnsBinaryData = false;
        }
    }
    
    BinaryReader::BinaryReader(const void* inData, PtrSize inSize, bool bTakeOwnershipOfData)
    :
    data((ubyte*)inData),
    size(inSize),
    bOwnsBinaryData(bTakeOwnershipOfData)
    {
        readPtr = data;
    }
    
    BinaryReader::~BinaryReader()
    {
        if (bOwnsBinaryData)
		{
			delete[] data;
            bOwnsBinaryData = false;
        }
        
        data = nullptr;
        readPtr = nullptr;
        size = 0;
    }
    
    void* BinaryReader::NextRawData(PtrSize dataSize)
    {
        void* next = (void*)readPtr;
        readPtr += dataSize;
        
        return next;
    }
}
