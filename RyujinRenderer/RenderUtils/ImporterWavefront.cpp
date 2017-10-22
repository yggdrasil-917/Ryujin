
#include "ImporterWavefront.hpp"

#include "RyujinCore/Logger/Logger.hpp"

#include "RyujinCore/Containers/String.hpp"

#include "RyujinCore/OS/FileSystem.hpp"
#include "RyujinCore/OS/Application.hpp"

#if PLATFORM_WINDOWS
#pragma warning (push)
#pragma warning (disable: 6031) // ignore warnings from sscanf return value being ignored
#endif

namespace Ryujin
{
    struct Face
    {
        uint32 v1, vt1, vn1;
        uint32 v2, vt2, vn2;
        uint32 v3, vt3, vn3;
        
        Face()
        {
            v1 = vt1 = vn1 = v2 = vt2 = vn2 = v3 = vt3 = vn3 = 0;
        }
    };

    struct VertexIndex
    {
        uint32 pos;
        uint32 uv;
        uint32 normal;
        
        bool operator ==(const VertexIndex& other) const
        {
            return pos == other.pos && uv == other.uv && normal == other.normal;
        }
        
        uint32 Hash() const
        {
            return (pos * 0xc761c23c + uv) * 0xb55a4f09 + normal;
        }
    };
    
    struct VertexIndexHash
    {
        uint64 operator ()(const VertexIndex& key, uint32 tableSize) const
        {
            return key.Hash() % tableSize;
        }
    };

    struct ObjVertex
    {
        Vector3 pos;
        Vector3 uv;
        Vector3 normal;
        bool bProcessed;
        
        ObjVertex() : bProcessed(false) {}
    };
    
#define USE_ALPHA_CUTOUT    1
#define USE_ALPHA_BLENDING  2

    bool ImporterWavefront::LoadStaticMesh(const char* filename, DynamicArray<StaticMeshVertex> &outVerts, DynamicArray<uint32> &outIndices, VertexLayout& outVertexLayout, DynamicArray<SubMesh>& outMeshes)
    {
        FileHandle file = File::Open(filename, FileMode::FM_Read, false);
        if (!file)
            return false;
        
        char line[255];
        DynamicArray<Vector3> verts(1000);
        DynamicArray<Vector2> texcoords(1000);
        DynamicArray<Vector3> normals(1000);
        DynamicArray<Face> faces(1000);
        HashTable<uint32, ObjMaterial> materialMap;
        int32 currentSubmesh = 0;
        uint32 startIndex = NumericLimit<int32>::Max();
        uint32 endIndex = 0;
        uint32 elementCount = 0;
        uint32 indexOffset = 0;
        bool bHasPosition = false;
        bool bHasUv0 = false;
        bool bHasNormal = false;
        // These are not supported yet
        //bool bHasUv1 = false;
        //bool bHasTangent = false;
        //bool bHasVertexColor = false;
        
        while (File::ReadLine(line, 255, file))
        {
            if (strlen(line) <= 1)	// skip if it is an empty line
                continue;
            
            if (line[0] == '#') // comment line
                continue;
            
            if (line[0] == 'v')
            {
                if (line[1] == 't')
                {
                    Vector2 t;
                    int matches = sscanf(line, "vt %f %f", &t.x, &t.y);
                    if (matches == 2)
                    {
                        #if WITH_STBI // STBI defaults to a flipped texture
                        t.y = 1.0f - t.y;
                        #endif
                        texcoords.Add(t);
                        bHasUv0 = true;
                    }
                }
                else if (line[1] == 'n')
                {
                    Vector3 n;
                    int matches = sscanf(line, "vn %f %f %f", &n.x, &n.y, &n.z);
                    if (matches == 3)
                    {
                        normals.Add(n);
                        bHasNormal = true;
                    }
                }
                else
                {
                    Vector3 v;
                    int matches = sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
                    if (matches == 3)
                    {
                        verts.Add(v);
                        bHasPosition = true;
                    }
                }
            }
            else if (line[0] == 'f')
            {
                Face f, f2;
                bool addF2 = false;
                int matches = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &f.v1, &f.vt1, &f.vn1, &f.v2, &f.vt2, &f.vn2, &f.v3, &f.vt3, &f.vn3);
                if (matches != 9) // perhaps v//vn?
                {
                    matches = sscanf(line, "f %d//%d %d//%d %d//%d", &f.v1, &f.vn1, &f.v2, &f.vn2, &f.v3, &f.vn3);
                    if (matches != 6)
                    {
                        matches = sscanf(line, "f %d/%d %d/%d %d/%d", &f.v1, &f.vt1, &f.v2, &f.vt2, &f.v3, &f.vt3); // then maybe v/vt/
                        if (matches != 6)
                        {
                            matches = sscanf(line, "f %d/%d %d/%d %d/%d %d/%d", &f.v1, &f.vt1, &f.v2, &f.vt2, &f.v3, &f.vt3, &f2.v3, &f2.vt3); // then maybe v/vt/
                            if (matches == 8)
                            {
                                addF2 = true;
                                --f2.v3; --f2.vt3;
                                f2.v1 = f.v1 - 1; f2.vt1 = f.vt1 - 1;
                                f2.v2 = f.v3 - 1; f2.vt2 = f.vt3 - 1;
                            }
                        }
                    }
                }
                
                switch (matches)
                {
                    case 9:
                    case 6:
                        elementCount += 3;
                        break;
                    case 8:
                        elementCount += 6;
                        break;
                }
                
                // decrement all the values by 1 since the index starts at 1 instead of 0
                --f.v1; --f.vt1; --f.vn1; --f.v2; --f.vt2; --f.vn2; --f.v3; --f.vt3; --f.vn3;
                faces.Add(f);
                startIndex = MathUtils::Min(f.v1, MathUtils::Min(f.v2, f.v3));
                endIndex = MathUtils::Max(f.v1, MathUtils::Max(f.v2, f.v3));
                if (addF2)
                {
                    faces.Add(f2);
                    startIndex = MathUtils::Min(f2.v1, MathUtils::Min(f2.v2, f2.v3));
                    endIndex = MathUtils::Max(f2.v1, MathUtils::Max(f2.v2, f2.v3));
                }
            }
            else if (line[0] == 'm')
            {
                char matName[32];
                if (sscanf(line, "mtllib %s", matName) > 0)
					LoadMaterial(*String::Printf("%sContent/Materials/%s", AppInfo::GetResourcesDir(), matName), materialMap);
            }
            else if (line[0] == 'u') // usemtl
            {
                if (outMeshes.GetSize() > 0)
                {
                    outMeshes[currentSubmesh].startIndex = startIndex;
                    outMeshes[currentSubmesh].endIndex = endIndex;
                    outMeshes[currentSubmesh].elementCount = elementCount;
                    outMeshes[currentSubmesh].indexOffset = indexOffset;
                    
                    startIndex = NumericLimit<int32>::Max();
                    endIndex = -1;
                    elementCount = 0;
                    indexOffset += elementCount * sizeof(uint32);
                }
                
                SubMesh newSubmesh = {};
                char matName[32];
                if (materialMap.Count() > 0 && sscanf(line, "usemtl %s", matName) > 0)
                {
                    StringHash hash(matName);
                    HashTable<uint32, ObjMaterial>::Iterator it(materialMap);
                    uint32 matId = 0;
                    while (it)
                    {
                        if (it->key == hash.GetHash())
                        {
                            newSubmesh.material = matId;
                            break;
                        }
                        else
                        {
                            ++matId;
                        }
                        ++it;
                    }
                }
                
                outMeshes.Add(newSubmesh);
                currentSubmesh = outMeshes.GetSize() - 1;
            }
        }
        
        outMeshes[currentSubmesh].startIndex = startIndex;
        outMeshes[currentSubmesh].endIndex = endIndex;
        outMeshes[currentSubmesh].elementCount = elementCount;
        outMeshes[currentSubmesh].indexOffset = indexOffset;
        
        File::Close(file);
        
        // build the interleaved buffers
        HashTable<VertexIndex, uint32, VertexIndexHash> indexMap(faces.GetSize() * 3);
        if (verts.GetSize() > outVerts.GetSize())
            outVerts.Resize(verts.GetSize());
        if (faces.GetSize() * 3 > outIndices.GetSize())
            outIndices.Resize(faces.GetSize() * 3);
        
        uint32 index = 0;
        for (uint32 i = 0; i < faces.GetSize(); ++i)
        {
            VertexIndex vi;
            vi.pos = faces[i].v1; vi.uv = faces[i].vt1;
            vi.normal = faces[i].vn1 == (uint32)-1 ? vi.pos : faces[i].vn1;
            
            uint32 mapIndex = 0;
            if (!indexMap.TryGetValue(vi, mapIndex))
            {
                StaticMeshVertex v;
                v.position = verts[faces[i].v1];
                v.uv = texcoords[faces[i].vt1];
                
                if (!normals.IsEmpty())
                {
                    uint32 normalIndex = faces[i].vn1 == (uint32)-1 ? faces[i].v1 : faces[i].vn1;
                    v.normal = normals[normalIndex];
                    bHasNormal = true;
                }
                else
                {
                    v.normal = Vector3(0);
                    bHasNormal = false;
                }
                
                outIndices.Add(index++);
                outVerts.Add(v);
                
                indexMap.Add(vi, index);
            }
            else
            {
                outIndices.Add(mapIndex);
            }
            
            vi.pos = faces[i].v2; vi.uv = faces[i].vt2;
            vi.normal = faces[i].vn2 == (uint32)-1 ? vi.pos : faces[i].vn2;
            if (!indexMap.TryGetValue(vi, mapIndex))
            {
                StaticMeshVertex v;
                v.position = verts[faces[i].v2];
                v.uv = texcoords[faces[i].vt2];
                
                if (!normals.IsEmpty())
                {
                    uint32 normalIndex = faces[i].vn2 == (uint32)-1 ? faces[i].v2 : faces[i].vn2;
                    v.normal = normals[normalIndex];
                }
                else
                {
                    v.normal = Vector3(0);
                }
                
                outIndices.Add(index++);
                outVerts.Add(v);
                
                indexMap.Add(vi, index);
            }
            else
            {
                outIndices.Add(mapIndex);
            }
            
            vi.pos = faces[i].v3; vi.uv = faces[i].vt3;
            vi.normal = faces[i].vn3 == (uint32)-1 ? vi.pos : faces[i].vn3;
            if (!indexMap.TryGetValue(vi, mapIndex))
            {
                StaticMeshVertex v;
                v.position = verts[faces[i].v3];
                v.uv = texcoords[faces[i].vt3];
                
                if (!normals.IsEmpty())
                {
                    uint32 normalIndex = faces[i].vn3 == (uint32)-1 ? faces[i].v3 : faces[i].vn3;
                    v.normal = normals[normalIndex];
                }
                else
                {
                    v.normal = Vector3(0);
                }
                
                outIndices.Add(index++);
                outVerts.Add(v);
                
                indexMap.Add(vi, index);
            }
            else
            {
                outIndices.Add(mapIndex);
            }
            
        }
        
        // Setup the vertex layout struct
        outVertexLayout.Add(VertexAttr::Position, VertexAttrType::Float3, offsetof(StaticMeshVertex, position));
        if (bHasUv0)
            outVertexLayout.Add(VertexAttr::UV0, VertexAttrType::Float2, offsetof(StaticMeshVertex, uv));
        if (bHasNormal)
            outVertexLayout.Add(VertexAttr::Normal, VertexAttrType::Float3, offsetof(StaticMeshVertex, normal));
        outVertexLayout.Finish();
        
        return true;
    }
    
    void ImporterWavefront::LoadMaterial(const char *materialFile, HashTable<uint32, ObjMaterial> &materialMap)
    {
        FileHandle handle = File::Open(materialFile, FileMode::FM_Read, false);
        if (handle == nullptr)
            return;
        
        char line[255];
        StringHash currentMatHash("");
        ObjMaterial* currentMat = nullptr;
        while (File::ReadLine(line, 255, handle))
        {
            if (strlen(line) <= 1)	// skip if it is an empty line
                continue;
            
            if (line[0] == 'n') //newmtl
            {
                ObjMaterial mat;
                Memory::MemSet(&mat, 0, sizeof(ObjMaterial));
				if (sscanf(line, "newmtl %s", mat.name) > 0)
				{
					currentMatHash = StringHash(mat.name);
					materialMap.Add(currentMatHash.GetHash(), mat);
					currentMat = &materialMap[currentMatHash.GetHash()];
				}
            }
            else if (line[0] == 'N') // Ns
            {
                sscanf(line, "Ns %f", &currentMat->shininess);
            }
            else if (line[0] == 'd') // d
            {
                sscanf(line, "d %f", &currentMat->alpha);
                if (currentMat->alpha != 1.0f)
                    currentMat->alphaMethod = USE_ALPHA_BLENDING;
            }
            else if (line[0] == 'a') // alphaMethod
            {
                sscanf(line, "alphaMethod %d", &currentMat->alphaMethod);
            }
            else if (line[0] == 'K' && line[1] == 'a') // Ka
            {
                sscanf(line, "Ka %f %f %f", &currentMat->ka[0], &currentMat->ka[1], &currentMat->ka[2]);
            }
            else if (line[0] == 'K' && line[1] == 'd') // Kd
            {
                sscanf(line, "Ka %f %f %f", &currentMat->kd[0], &currentMat->kd[1], &currentMat->kd[2]);
            }
            else if (line[0] == 'K' && line[1] == 's') // Ks
            {
                sscanf(line, "Ka %f %f %f", &currentMat->ks[0], &currentMat->ks[1], &currentMat->ks[2]);
            }
            else if (line[0] == 'm' && line[4] == 'K' && line[5] == 'd') // map_Kd
            {
                sscanf(line, "map_Kd %s", currentMat->mapKd);
            }
            else if (line[0] == 'm' && line[4] == 'b' && line[5] == 'u') // map_bump
            {
                sscanf(line, "map_bump %s", currentMat->mapBump);
            }
            else if(line[0] == 'b') // bump
            {
				if (currentMat)
					sscanf(line, "bump %s", currentMat->mapBump);
            }
        }
        
        Debugf(Info, "Total materials loaded ", materialMap.Count());
    }
}



#if PLATFORM_WINDOWS
#pragma warning (pop)
#endif





