
#include "ResourceTypes.hpp"


#include "RyujinRenderer/RenderUtils/ImporterWavefront.hpp"
#include "RyujinRenderer/RenderUtils/ImporterRmf.hpp"

#include "RyujinRenderer/RenderUtils/Image.hpp"

namespace Ryujin
{
    MeshResource::MeshResource(DynamicArray<StaticMeshVertex>& inVerts, DynamicArray<uint32>& inIndices, DynamicArray<SubMesh>& inMeshes, AABB& inBbox)
    :
    Resource_Old()
    {
        verts = inVerts;
        indices = inIndices;
        bbox = inBbox;
    }
    
    MeshResource::~MeshResource()
    {
        
    }
    
    TextureResource::TextureResource(ubyte* data, int16 inBaseMipWidth, int16 inBaseMipHeight, TextureType inType, PixelFormat inFormat)
    {
        baseMipWidth = inBaseMipWidth;
        baseMipHeight = inBaseMipHeight;
        type = inType;
        pixelFormat = inFormat;
        
        // TODO: write functions to produce these values
        int32 bytesPerChannel = 8;
        int32 numChannels = 4;
        
        if (MathUtils::IsPowerOfTwo(baseMipWidth) && MathUtils::IsPowerOfTwo(baseMipHeight))
        {
            numMips = MathUtils::Log2(MathUtils::Max<int16>(baseMipWidth, baseMipHeight));
            PtrSize totalBytes = 0;
            for (int8 mip = 0; mip < numMips; ++mip)
            {
                mipDataJumpTable.Add(totalBytes);
                
                int16 currentWidth = MathUtils::Max<int16>(baseMipWidth >> mip, 1);
                int16 currentHeight = MathUtils::Max<int16>(baseMipHeight >> mip, 1);
                totalBytes += bytesPerChannel * numChannels * currentHeight * currentWidth;
            }
            
            mipData.Resize((uint32)totalBytes);
            Memory::MemCpy(mipData.GetData(), data, totalBytes);
        }
        else
        {
            // if the texture is not power of two then we can't make lower mips
            numMips = 1;
            mipData.Resize(bytesPerChannel * numChannels * baseMipWidth * baseMipHeight);
            Memory::MemCpy(mipData.GetData(), data, bytesPerChannel * numChannels * baseMipWidth * baseMipHeight);
        }
    }
    
    
    MeshResource* MeshResourceLoader::Load(const String &file)
    {
        const uint32 length = file.Length();
        if (length == 0)
            return nullptr;
        
        DynamicArray<StaticMeshVertex> verts;
        DynamicArray<uint32> indices;
        DynamicArray<SubMesh> meshes;
        AABB bbox;
        VertexLayout vertexLayout;
        
        if (file.EndsWith(".obj"))
        {
            ImporterWavefront importer;
            if (importer.LoadStaticMesh(*file, verts, indices, vertexLayout, meshes))
            {
                return RYUJIN_NEW MeshResource(verts, indices, meshes, bbox);
            }
        }
        else if (file.EndsWith(".asset"))
        {
            ImporterRmf importer;
//            if (importer.LoadStaticMesh(*file, verts, indices, vertexLayout, meshes))
            {
                return RYUJIN_NEW MeshResource(verts, indices, meshes, bbox);
            }
        }
        
        RYUJIN_ASSERT(0, *String::Printf("Unknown mesh format encountered loading %s", *file));
        return nullptr;
    }
    
    
    
    TextureResource* TextureResourceLoader::Load(const String& file)
    {
        Image image;
        if (!image.LoadImageFromDisk(*file))
            return nullptr;
        
        TextureResource* resource = RYUJIN_NEW TextureResource((ubyte*)image.GetImageData(), image.GetWidth(), image.GetHeight(), TextureType::TT_Type2D, PixelFormat::PF_BGRA8Unorm);
        
        return resource;
    }
}










