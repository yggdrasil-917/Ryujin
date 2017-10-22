
#include "Pupper.hpp"



namespace Ryujin
{
    void Pup(Pupper* p, char& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, uint8& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, uint16& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, uint32& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, uint64& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, int8& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, int16& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, int32& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, int64& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, float& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, double& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    void Pup(Pupper* p, bool& value, const VarInfo& inVarInfo)
    {
        p->Pup(value, inVarInfo);
    }
    
    
    
    void Pup(Pupper* p, PuppableObjectBase& obj, const VarInfo& inVarInfo)
    {
        obj.PackUnpack(p, inVarInfo);
    }
}