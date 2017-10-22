
#pragma once


#include "../OS/FileSystem.hpp"


namespace Ryujin
{
    struct VarInfo
    {
        String name;
        
        VarInfo(String&& inName)
        : name(inName)
        {
            
        }
        
        VIRTUAL ~VarInfo() {}
    };
    
    
    struct CORE_API Pupper
    {
        enum class IoMode : uint8
        {
            Read,
            Write
        };
        
        IoMode ioMode;
        
        Pupper(IoMode inIoMode)
        : ioMode(inIoMode)
        {
            
        }
        
        VIRTUAL ~Pupper() {}
        
        VIRTUAL void Pup(char& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(uint8& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(uint16& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(uint32& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(uint64& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(int8& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(int16& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(int32& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(int64& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(float& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(double& value, const VarInfo& inVarInfo) = 0;
        VIRTUAL void Pup(bool& value, const VarInfo& inVarInfo) = 0;
        
        template<typename T>
        void PupArray(T* array, int32& count, const VarInfo& inVarInfo)
        {
            Pup(count, inVarInfo.name + ".length");
            
            if (ioMode == IoMode::Read)
            {
                array = RYUJIN_NEW T[count];
            }
            
            for (int32 i = 0; i < count; ++i)
            {
                Pup(array[i], String::Printf("%s.[%d]", *inVarInfo.name, i));
            }
        }
    };
    
    
    extern CORE_API void Pup(Pupper* p, char& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, uint8& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, uint16& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, uint32& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, uint64& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, int8& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, int16& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, int32& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, int64& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, float& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, double& value, const VarInfo& inVarInfo);
    extern CORE_API void Pup(Pupper* p, bool& value, const VarInfo& inVarInfo);
    
    
    class CORE_API PuppableObjectBase
    {
    public:
        CORE_API friend void Pup(Pupper* p, PuppableObjectBase& obj, const VarInfo& inVarInfo);
        
    protected:
        VIRTUAL void PackUnpack(Pupper* p, const VarInfo& inVarInfo) {}
    };
    
    
    template<typename T>
    class TPuppableObject : public PuppableObjectBase
    {
    public:
        TPuppableObject(T& inDerived)
        : derived(inDerived)
        {
            
        }
        
    protected:
        T& derived;
        
    protected:
        void PackUnpack(Pupper* p, const VarInfo& inVarInfo) OVERRIDE FINAL
        {
            Pup(p, derived, inVarInfo);
        }
    };
}
