#pragma once

#include "../Utils/ExtensionsBase.hpp"

#include "RyujinCore/Containers/HashTable.hpp"
#include "RyujinCore/CoreUtils/StringHash.hpp"
#include "RyujinCore/Serialization/BinaryFilePupper.hpp"

namespace Ryujin
{
#define DECLARE_COMPONENT_TYPE(classname) \
    private: \
        static const ComponentCreatorImpl<classname> creator; \
        static const uint32 componentId; \
    public: \
        static uint32 GetComponentTypeId() { return componentId; } \
        uint32 GetTypeId() OVERRIDE FINAL;
    
#define IMPLEMENT_COMPONENT_TYPE(classname) \
    const ComponentCreatorImpl<classname> classname::creator(#classname); \
    const uint32 classname::componentId = (uint32)CalculateFNV(#classname); \
    uint32 classname::GetTypeId() { return GetComponentTypeId(); }

    
    class EXTENSIONS_API Component : public TPuppableObject<Component>
    {
        friend class RyujinEntity;
    protected:
        class RyujinEntity* entity;
        bool bIsEnabled;
        
    public:
        Component();
        VIRTUAL ~Component() {}
        
        Component(const Component& other);
        Component(Component&& other);
        
        Component& operator=(const Component& other);
        Component& operator=(Component&& other);
        
        VIRTUAL uint32 GetTypeId() = 0;
        VIRTUAL void Serialize(Pupper* p, const VarInfo& inVarInfo);
        
        FORCEINLINE void SetEnabled(bool newValue) { bIsEnabled = newValue; }
        FORCEINLINE bool IsEnabled() const { return bIsEnabled; }
        FORCEINLINE RyujinEntity* GetOwner() const { return entity; }
        
		EXTENSIONS_API friend void Pup(Pupper* p, Component& obj, const VarInfo& inVarInfo);
    };
    
    
    class EXTENSIONS_API ComponentCreator
    {
    public:
        ComponentCreator(const char* className);
        
        VIRTUAL Component* Create() = 0;
    };
    
    template<class T>
    class EXTENSIONS_API ComponentCreatorImpl : public ComponentCreator
    {
    public:
        ComponentCreatorImpl(const char* className)
        : ComponentCreator(className)
        {
            
        }
        
        Component* Create() OVERRIDE FINAL
        {
            return RYUJIN_NEW T();
        }
    };
    
    class EXTENSIONS_API ComponentFactory
    {
    public:
        static Component* Create(const uint32 classId);
        static void Register(const uint32 classId, ComponentCreator* creator);
        
    private:
        static HashTable<uint32, ComponentCreator*>& GetTable();
    };
    
    class EXTENSIONS_API TagComponent : public Component
    {
        DECLARE_COMPONENT_TYPE(TagComponent);
    public:
        String tag;
        
        TagComponent() {}
        TagComponent(const char* tag);
        
        TagComponent(const TagComponent& other);
        TagComponent(TagComponent&& other);
        
        TagComponent& operator=(const TagComponent& other);
        TagComponent& operator=(TagComponent&& other);
        
        void Serialize(Pupper* p, const VarInfo& inVarInfo) OVERRIDE FINAL;
        
        FORCEINLINE const char* GetTag() const { return *tag; }
        
		EXTENSIONS_API friend void Pup(Pupper* p, TagComponent& obj, const VarInfo& inVarInfo);
    };
   
}
