
#include "BinaryWriter.hpp"
#include "../Containers/String.hpp"


namespace Ryujin
{
    BinaryWriter::BinaryWriter()
    : handle(nullptr)
    {
    }
    
    BinaryWriter::BinaryWriter(const char* filename)
    {
        Open(filename);
    }
    
    BinaryWriter::~BinaryWriter()
    {
        Close();
    }
    
    
    void BinaryWriter::Open(const char* filename, FileMode fileMode)
    {
        handle = File::Open(filename, fileMode, true);
        RYUJIN_ASSERT(handle, *String::Printf("Failed to open file %s", filename));
    }
    
    void BinaryWriter::Close()
    {
        if (handle)
        {
            File::Close(handle);
            handle = nullptr;
        }
    }
}