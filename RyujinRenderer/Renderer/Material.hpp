#pragma once


#include "RendererCommon.hpp"
#include "RyujinCore/Containers/HashTable.hpp"
#include "../RenderUtils/Color.hpp"

namespace Ryujin
{
    struct ParamBlock
    {
        float roughness;
        float metallic;
        float specular;
        Color baseColor;
        
        ParamBlock()
        : roughness(1.0f),
        metallic(0.0f),
        specular(0.0f),
        baseColor(Color())
        {
            
        }
    };
    
    class RENDER_API MaterialDescriptor
    {
    public:
        const char* shaderFile;
        const char* vertexFunc;
        const char* fragmentFunc;
        const char* geometryFunc;
        const char* domainFunc;
        const char* hullFunc;
        String preprocessorList;
        VertexDescriptor vertexDesc;
        
        MaterialDescriptor();
        
        uint64 Hash() const;
    };
    
    class RENDER_API Material
    {
    public:
        struct ParamMapPair
        {
            String name;
            uint32 offset;
        };
        
        friend class MaterialInstance;
        
    private:
        ConstantBufferPtr vertexCB;
        ConstantBufferPtr fragmentCB;
        uint8* fragmentData;
        StringMap<uint32> paramToOffset;
        BlendMode blendMode;
        
        uint64 matId;
        
    public:
        Material();
        ~Material();
        
        void InitializeParamMap(const DynamicArray<ParamMapPair>& mapDesc);
        
        void UpdateFragmentCB(void* data, uint64 size)
        {
            fragmentCB->Update(data, size);
        }
        
        FORCEINLINE void SetFragmentCB(ConstantBufferPtr buffer) { fragmentCB = buffer; }
        FORCEINLINE const ConstantBufferPtr& GetFragmentCB() const { return fragmentCB; }
        
        template<typename T>
        bool SetParam(const char* name, T value)
        {
            uint32 offset = 0;
            if (paramToOffset.TryGetValue(name, offset))
            {
                Memory::MemCpy(fragmentData + offset, value, sizeof(value));
                return true;
            }
            
            return false;
        }
        
        FORCEINLINE uint64 GetMaterialId() const { return matId; }
    };
    
    class RENDER_API MaterialInstance
    {
    protected:
        Material* parent;
        
    public:
    };
}
