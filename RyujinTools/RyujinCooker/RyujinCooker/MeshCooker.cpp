

#include "MeshCooker.hpp"

#include "Ryujin/Ryujin/Versions.hpp"
#include "Ryujin/ResourceManagers/ResourceTypes.hpp"
#include "RyujinRenderer/Renderer/Model.hpp"
#include "RyujinRenderer/RenderUtils/VertexWriter.hpp"
#include "RyujinRenderer/RenderUtils/ImporterRmf.hpp"
#include "RyujinCore/CoreUtils/BinaryWriter.hpp"
#include "RyujinCore/CoreUtils/BinaryReader.hpp"


namespace Ryujin
{
    void MeshCooker::QueueMeshToCook(const Ryujin::String &meshSourceFile)
    {
        CookableItem item;
        item.targetFilename = String::Printf("%s%s", COOKED_CONTENT_DIR, *meshSourceFile);
        item.sourceFilename = String::Printf("%s%s", MODEL_CONTENT_DIR, *meshSourceFile);
        if (item.targetFilename.EndsWith(".obj"))
        {
            int32 index = item.targetFilename.Find(".obj");
            RYUJIN_ASSERT(index != -1, "Failed to find substring index for obj file");
            item.targetFilename.Replace(index, 6, ".asset");
        }
        meshesToCook.AddUnique(item);
    }
    
    void MeshCooker::RemoveAllMeshes()
    {
        meshesToCook.RemoveAll();
    }
    
    void MeshCooker::CookQueuedMeshes()
    {
        SCOPED_PROFILE_EVENT(CookingMeshes, PC_Cooker, 255, 255, 0);
        
        for (const auto& mesh : meshesToCook)
        {
            Debugf(Info, "Cooking mesh ", *mesh.sourceFilename);
            CookMesh(mesh, true);
        }
    }
    
    void MeshCooker::CookMesh(const CookableItem& mesh, bool bInterleaveVertexData)
    {
        if (File::IsFileNewerThan(*mesh.targetFilename, *mesh.sourceFilename))
        {
            // Check file version number with mesh cooker version
            BinaryFileReader br(mesh.targetFilename);
            int32 fileVersionId = -1;
            br.Next(&fileVersionId, 1);
            if (fileVersionId == MESH_COOKER_VERSION)
            {
                Debugf(Info, *mesh.sourceFilename, " has already been cooked... skipping");
                return;
            }
        }
        
        DynamicArray<StaticMeshVertex> verts;
        DynamicArray<uint32> indices;
        DynamicArray<SubMesh> meshes;
        VertexLayout vertexLayout;
        
        if (mesh.sourceFilename.EndsWith(".obj"))
        {
            ImporterWavefront importer;
            if (!importer.LoadStaticMesh(*mesh.sourceFilename, verts, indices, vertexLayout, meshes))
            {
                Warnf(Info, "Failed to load mesh ", *mesh.sourceFilename);
            }
        }
        else
        {
            Warnf(Info, "No mesh importer specified for ", *mesh.sourceFilename);
            return;
        }
        
        uint32 maxUint16 = NumericLimit<uint16>::Max();
        bool bIs16BitIndexBuffer = true;
        for (const uint16 i : indices)
        {
//            Debugf(Info, i);
            if (i >= maxUint16)
            {
                bIs16BitIndexBuffer = false;
                break;
            }
        }
        
        DynamicArray<uint16> shortIndices;
        if (bIs16BitIndexBuffer)
        {
            Debugf(Info, "Building 16 bit index buffer");
            shortIndices.Resize(indices.GetSize());
            for (const auto& i : indices)
            {
                shortIndices.Add(i);
            }
        }
        
        BinaryWriter bw(*mesh.targetFilename);
        
        CookedMeshHeader meshHeader;
        vertexLayout.SetInterleavedVertexDataFlag(bInterleaveVertexData);
        meshHeader.meshCookerVersion = MESH_COOKER_VERSION;
        meshHeader.vertexLayout = vertexLayout;
        meshHeader.vertCount = verts.GetSize();
        meshHeader.indexCount = indices.GetSize();
        meshHeader.meshCount = meshes.GetSize();
        meshHeader.bHas16BitIndices = bIs16BitIndexBuffer;
        
        uint8* binaryBlob = (uint8*)Memory::Malloc(vertexLayout.GetByteSize() * verts.GetSize() * sizeof(uint8));
        uint8* iterator = binaryBlob;
        if (bInterleaveVertexData)
        {
            uint8* vertData = static_cast<uint8*>(verts.GetData());
            int32 vertIndex = 0;
            int32 stride = sizeof(StaticMeshVertex);
            while (vertIndex < meshHeader.vertCount)
            {
                for (int32 i = 0; i < vertexLayout.GetNumComponents(); ++i)
                {
                    iterator = VertexWriter::Write(iterator,
                                                   (vertData + vertexLayout.GetComponentByteOffset(i)),
                                                   vertexLayout.GetComponentByteSize(i));
                }
                vertData += stride;
                ++vertIndex;
            }
        }
        else
        {
            int32 byteOffset = 0;
            for (int32 i = 0; i < vertexLayout.GetNumComponents(); ++i)
            {
                uint8* vertData = static_cast<uint8*>(verts.GetData());
                int32 vertIndex = 0;
                int32 stride = sizeof(StaticMeshVertex);
                while (vertIndex < meshHeader.vertCount)
                {
                    iterator = VertexWriter::Write(iterator,
                                                   (vertData + vertexLayout.GetComponentByteOffset(i)),
                                                   vertexLayout.GetComponentByteSize(i));
                    vertData += (stride * vertIndex);
                    ++vertIndex;
                }
                
                // Need to update vertex layout component byte offset to reflect the tightly packed nature of it
                meshHeader.vertexLayout.UpdateComponentByteOffset(i, byteOffset);
                byteOffset += meshHeader.vertCount * meshHeader.vertexLayout.GetComponentByteSize(i);
            }
        }
        
        // Write to disk
        bw.Write<CookedMeshHeader>(meshHeader);
        bw.WriteArray<uint8>(binaryBlob, vertexLayout.GetByteSize() * verts.GetSize());
        
        if (bIs16BitIndexBuffer)
            bw.WriteArray(shortIndices.GetTypedData(), meshHeader.indexCount);
        else
            bw.WriteArray<uint32>(indices.GetTypedData(), meshHeader.indexCount);
        bw.WriteArray<SubMesh>(meshes.GetTypedData(), meshHeader.meshCount);
        
        Memory::Free(binaryBlob);
    }
}








