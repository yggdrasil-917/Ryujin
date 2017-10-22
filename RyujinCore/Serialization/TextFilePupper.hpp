
#pragma once

#include "Pupper.hpp"


namespace Ryujin
{
    struct CORE_API TextFilePupper : public Pupper
    {
        TextFilePupper(FileHandle inFileHandle, Pupper::IoMode inIoMode);
        
        FileHandle fileHandle;
        
        void Pup(char& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(uint8& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(uint16& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(uint32& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(uint64& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(int8& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(int16& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(int32& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(int64& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(float& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(double& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
        void Pup(bool& value, const VarInfo& inVarInfo) OVERRIDE FINAL;
    };
    
    static const char* formatSpecifier[] = {
        "%c",
        "%u",
        "%d",
        "%.3f",
        "%s"
    };
    
    enum FormatSpecType
    {
        Char,
        Uint8,
        Uint16,
        Uint32,
        Uint64,
        Int8,
        Int16,
        Int32,
        Int64,
        Float,
        Double,
        Bool
    };
    
    static const char* GetFormatSpecifier(FormatSpecType type)
    {
        switch (type)
        {
            case Char:
                return formatSpecifier[0];
                
            case Uint8:
            case Uint16:
            case Uint32:
            case Uint64:
                return formatSpecifier[1];
                
            case Int8:
            case Int16:
            case Int32:
            case Int64:
                return formatSpecifier[2];
                
            case Float:
            case Double:
                return formatSpecifier[3];
                
            case Bool:
                return formatSpecifier[4];
                
            default:
                return formatSpecifier[2];
        }
    }
    
    template<typename T>
    void PupText(TextFilePupper* pup, T value, FormatSpecType type, const VarInfo& inVarInfo, String& line)
    {
        switch (pup->ioMode)
        {
            case Pupper::IoMode::Read:
            {
                String beginTag = String::Printf("<%s>", *inVarInfo.name);
                String endTag = String::Printf("</%s>", *inVarInfo.name);
                char lineBuffer[512];
                if (File::ReadLine(lineBuffer, 512, pup->fileHandle))
                {
                    int32 offset = 0;
                    if (lineBuffer[0] == '\0')
                    {
                        offset = 1;
                    }
                    line = String::SubStr(lineBuffer + beginTag.Length() + offset, inVarInfo.name.Length());
                }
            }
                break;
                
            case Pupper::IoMode::Write:
            {
                String formatStr = String::Printf("<%s>%s</%s>\n", *inVarInfo.name, GetFormatSpecifier(type), *inVarInfo.name);
                if (type == FormatSpecType::Bool)
                {
                    String str = String::Printf(*formatStr, (value ? "true" : "false"));
                    File::Write(*str, sizeof(char), str.Length() + 1, pup->fileHandle);
                }
                else
                {
                    if (type == FormatSpecType::Char && value == '\0')
                    {
                        String str = String::Printf(*formatStr, '0');
                        File::Write(*str, sizeof(char), str.Length() + 1, pup->fileHandle);
                    }
                    else
                    {
                        String str = String::Printf(*formatStr, value);
                        File::Write(*str, sizeof(char), str.Length() + 1, pup->fileHandle);
                    }
                }
            }
                break;
        }
    }
}
