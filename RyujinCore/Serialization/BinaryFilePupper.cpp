

#include "BinaryFilePupper.hpp"


namespace Ryujin
{
    BinaryFilePupper::BinaryFilePupper(FileHandle inFileHandle, Pupper::IoMode inIoMode)
    : Pupper(inIoMode),
      fileHandle(inFileHandle)
    {
        
    }
    
    void BinaryFilePupper::Pup(char& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(uint8& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(uint16& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(uint32& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(uint64& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(int8& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(int16& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(int32& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(int64& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(float& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(double& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
    
    void BinaryFilePupper::Pup(bool& value, const Ryujin::VarInfo &inVarInfo)
    {
        PupBytes(this, value);
    }
}
