

#include "TextFilePupper.hpp"


#include <cstdlib>


namespace Ryujin
{
    TextFilePupper::TextFilePupper(FileHandle inFileHandle, Pupper::IoMode inIoMode)
    : Pupper(inIoMode),
      fileHandle(inFileHandle)
    {
        
    }
    
    void TextFilePupper::Pup(char& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<char>(this, value, FormatSpecType::Char, inVarInfo, line);
        if (line.Length() > 0)
        {
            value = line[0];
        }
    }
    
    void TextFilePupper::Pup(uint8& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<uint8>(this, value, FormatSpecType::Uint8, inVarInfo, line);
        if (line.Length() > 0)
        {
            const char* str = *line;
            char* end;
            value = (uint8)(std::strtoul(str, &end, 10));
        }
    }
    
    void TextFilePupper::Pup(uint16& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<uint16>(this, value, FormatSpecType::Uint16, inVarInfo, line);
        if (line.Length() > 0)
        {
            const char* str = *line;
            char* end;
            value = (uint16)(std::strtoul(str, &end, 10));
        }
    }
    
    void TextFilePupper::Pup(uint32& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<uint32>(this, value, FormatSpecType::Uint32, inVarInfo, line);
        if (line.Length() > 0)
        {
            const char* str = *line;
            char* end;
            value = (uint32)(std::strtoul(str, &end, 10));
        }
    }
    
    void TextFilePupper::Pup(uint64& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<uint64>(this, value, FormatSpecType::Uint64, inVarInfo, line);
        if (line.Length() > 0)
        {
            const char* str = *line;
            char* end;
            value = (uint64)(std::strtoul(str, &end, 10));
        }
    }
    
    void TextFilePupper::Pup(int8& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<int8>(this, value, FormatSpecType::Int8, inVarInfo, line);
        if (line.Length() > 0)
        {
            const char* str = *line;
            char* end;
            value = (int8)(std::strtol(str, &end, 10));
        }
    }
    
    void TextFilePupper::Pup(int16& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<int16>(this, value, FormatSpecType::Int16, inVarInfo, line);
        if (line.Length() > 0)
        {
            const char* str = *line;
            char* end;
            value = (int16)(std::strtol(str, &end, 10));
        }
    }
    
    void TextFilePupper::Pup(int32& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<int32>(this, value, FormatSpecType::Int32, inVarInfo, line);
        if (line.Length() > 0)
        {
            const char* str = *line;
            char* end;
            value = (int32)(std::strtol(str, &end, 10));
        }
    }
    
    void TextFilePupper::Pup(int64& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<int64>(this, value, FormatSpecType::Int64, inVarInfo, line);
        if (line.Length() > 0)
        {
            const char* str = *line;
            char* end;
            value = (int64)(std::strtol(str, &end, 10));
        }
    }
    
    void TextFilePupper::Pup(float& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<float>(this, value, FormatSpecType::Float, inVarInfo, line);
        if (line.Length() > 0)
        {
            const char* str = *line;
            char* end;
            value = std::strtof(str, &end);
        }
    }
    
    void TextFilePupper::Pup(double& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<double>(this, value, FormatSpecType::Double, inVarInfo, line);
        if (line.Length() > 0)
        {
            const char* str = *line;
            char* end;
            value = std::strtod(str, &end);
        }
    }
    
    void TextFilePupper::Pup(bool& value, const Ryujin::VarInfo &inVarInfo)
    {
        String line;
        PupText<bool>(this, value, FormatSpecType::Bool, inVarInfo, line);
        if (line.Length() > 0)
        {
            value = (line == "true" || line == "TRUE");
        }
    }

}
