
#ifndef BINARY_WRITER
#define BINARY_WRITER


#include "../Logger/Logger.hpp"
#include "../CoreUtils/Common.hpp"
#include "../OS/FileSystem.hpp"


namespace Ryujin
{
    class CORE_API BinaryWriter : NonCopyable
    {
    private:
        FileHandle handle;
        
    public:
        BinaryWriter();
        BinaryWriter(const char* filename);
        ~BinaryWriter();
        
        void Open(const char* filename, FileMode fileMode = FileMode::FM_AppendUpdate);
        void Close();
        
        template<typename T>
        void Write(T value)
        {
            RYUJIN_ASSERT(handle, "No file is open for write");
            
            File::Write(&value, sizeof(T), 1, handle);
        }
        
        template<typename T>
        void WriteArray(T* values, int32 count)
        {
            RYUJIN_ASSERT(handle, "No file is open for write");
            
            File::Write(values, sizeof(T), count, handle);
        }
    };
}



#endif