
#include "Material.hpp"
#include "RyujinCore/CoreUtils/CRC.hpp"


namespace Ryujin
{
    MaterialDescriptor::MaterialDescriptor()
    {
        shaderFile = "";
        vertexFunc = "";
        fragmentFunc = "";
        geometryFunc = "";
        domainFunc = "";
        hullFunc = "";
    }
    
    uint64 MaterialDescriptor::Hash() const
    {
        return CRC::MemCrc((uint8*)(this), sizeof(MaterialDescriptor));
    }
    
    Material::Material()
    {
        
    }
    
    Material::~Material()
    {
        fragmentCB.SafeRelease();
        vertexCB.SafeRelease();
    }
    
    void Material::InitializeParamMap(const DynamicArray<ParamMapPair>& mapDesc)
    {
        for (auto& pair : mapDesc)
        {
            paramToOffset.Add(pair.name, pair.offset);
        }
    }
}
