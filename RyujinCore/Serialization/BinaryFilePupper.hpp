
#pragma once


#include "Pupper.hpp"



namespace Ryujin
{
    struct CORE_API BinaryFilePupper : public Pupper
    {
        BinaryFilePupper(FileHandle inFileHandle, Pupper::IoMode inIoMode);
        
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
    
    template<typename T>
    void PupBytes(BinaryFilePupper* pup, T& val)
    {
        switch (pup->ioMode)
        {
            case Pupper::IoMode::Read:
                File::Read(&val, sizeof(T), 1, pup->fileHandle);
                break;
                
            case Pupper::IoMode::Write:
                File::Write((void*)&val, sizeof(T), 1, pup->fileHandle);
                break;
        }
    }
}
