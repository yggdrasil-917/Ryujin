
#ifndef RESOURCE_TYPES_HPP
#define RESOURCE_TYPES_HPP

#include "RyujinRenderer/Renderer/Model.hpp"

#include "RyujinCore/Containers/DynamicArray.hpp"
#include "RyujinCore/Containers/String.hpp"
#include "RyujinCore/CoreUtils/StringHash.hpp"
#include "RyujinCore/Math/MathUtils.hpp"

namespace Ryujin
{
    enum class ResourceStatus : uint8
    {
        Invalid,
        Pending,
        Valid,
        Failed,
    };
    
    using ResourceHandle = Handle<16, 16>;
    
    class Resource_Old
    {
    protected:
        ResourceHandle handle;
        ResourceStatus status;
        String filename;
        StringHash resourceHash;
        
        int32 refCount;
        
    public:
        Resource_Old()
        : resourceHash(""), refCount(1)
        {}
        
        VIRTUAL ~Resource_Old() {}
        
        FORCEINLINE void SetResourceHandle(ResourceHandle inHandle) { handle = inHandle; }
        FORCEINLINE ResourceHandle GetResourceHandle() const { return handle; }
        
        FORCEINLINE String GetFilename() const { return filename; }
        
        FORCEINLINE void SetResourceHash(const String& filename, const String& path)
        {
            this->filename = filename;
            resourceHash = StringHash(*(path + filename));
        }
        
        FORCEINLINE const StringHash& GetResourceHash() const { return resourceHash; }
        
        FORCEINLINE void IncrementRefCount() { ++refCount; }
        FORCEINLINE void DecrementRefCount() { --refCount; }
        FORCEINLINE int32 GetRefCount() const { return refCount; }
    };
    
    
    class MeshResource : public Resource_Old
    {
    protected:
        DynamicArray<StaticMeshVertex> verts;
        DynamicArray<uint32> indices;
        DynamicArray<SubMesh> meshes;
        AABB bbox;
        
    public:
        MeshResource(DynamicArray<StaticMeshVertex>& inVerts, DynamicArray<uint32>& inIndices, DynamicArray<SubMesh>& inMeshes, AABB& inBbox);
        VIRTUAL ~MeshResource();
        
        FORCEINLINE const DynamicArray<StaticMeshVertex>& GetVerts() const { return verts; }
        FORCEINLINE const DynamicArray<uint32>& GetIndices() const { return indices; }
        FORCEINLINE const DynamicArray<SubMesh>& GetMeshes() const { return meshes; }
        FORCEINLINE const AABB& GetBoundingBox() const { return bbox; }
    };
    
    
    class TextureResource : public Resource_Old
    {
    protected:
        
        int16 baseMipWidth;
        int16 baseMipHeight;
        int8 numMips;
        TextureType type;
        PixelFormat pixelFormat;
        DynamicArray<PtrSize> mipDataJumpTable;
        DynamicArray<ubyte> mipData;
        
    public:
        TextureResource(ubyte* data, int16 inBaseMipWidth, int16 inBaseMipHeight, TextureType inType, PixelFormat inFormat);
        
        FORCEINLINE int16 GetWidth(int8 mipIndex) const
        {
            RYUJIN_ASSERT(mipIndex < numMips, *String::Printf("Mip index is out of bounds %d/%d", mipIndex, numMips));
            return baseMipWidth >> mipIndex;
        }
        
        FORCEINLINE int16 GetHeight(int8 mipIndex) const
        {
            RYUJIN_ASSERT(mipIndex < numMips, *String::Printf("Mip index is out of bounds %d/%d", mipIndex, numMips));
            return baseMipHeight >> mipIndex;
        }
        
        FORCEINLINE ubyte* GetMipData(int8 mipIndex) const
        {
            return mipData.GetTypedData() + mipDataJumpTable[mipIndex];
        }
        
        FORCEINLINE int8 GetNumMips() const { return numMips; }
        FORCEINLINE TextureType GetType() const { return type; }
        FORCEINLINE PixelFormat GetPixelFormat() const { return pixelFormat; }
    };
    
    
    /////////////////////////////////////////
    ////////// Resource loaders
    // TODO: Make these return stack allocated resources and have the resource object implement move ctor
    class MeshResourceLoader
    {
    public:
        static MeshResource* Load(const String& file);
    };
    
    class TextureResourceLoader
    {
    public:
        static TextureResource* Load(const String& file);
    };
}



#endif
