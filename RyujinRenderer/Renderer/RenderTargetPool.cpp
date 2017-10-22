#include "RenderTargetPool.hpp"
#include "RenderResources.hpp"

#include "RyujinCore/OS/Application.hpp"
#include "RyujinCore/CoreUtils/CRC.hpp"
#include "RyujinCore/CoreUtils/Json.hpp"
#include "RyujinCore/CoreUtils/StringHash.hpp"


namespace Ryujin
{
    RenderTargetPool* RenderTargetPool::Instance()
    {
        static RenderTargetPool instance;
        return &instance;
    }
    
    bool RenderTargetPool::Initialize(uint16 width, uint16 height)
    {
        if (renderTargets.Count() > 0)
        {
            Shutdown();
        }
        
        UniquePtr<JsonValue> json(Json::ParseFile(*String::Printf("%sConfig/renderer.json", AppInfo::GetResourcesDir())));
        if (!json)
        {
            json = Json::ParseFile("../../../Content/Config/renderer.json");
            if (!json)
                return false;
        }
        
        const JsonObject& root = json->AsObject();
        const auto& rts = root.find("renderTargets2D")->second->AsArray();
        for (const auto& rt : rts)
        {
            const JsonObject& renderTarget = rt->AsObject();
            const auto& it = renderTarget.find("name");
            RYUJIN_ASSERT(it != renderTarget.end(), "No value for \"name\" parameter in renderer.json");
            const char* name = it->second->AsString();
            const auto& it2 = renderTarget.find("scaleFactor");
            uint16 scaleFactor = 1;
            if (it2 != renderTarget.end())
                scaleFactor = (uint16)it2->second->AsNumber();
            const auto& it3 = renderTarget.find("format");
            RYUJIN_ASSERT(it3 != renderTarget.end(), "No value for \"format\" parameter in renderer.json");
            const char* formatStr = it3->second->AsString();
            PixelFormat format = PF_Invalid;
            int32 index = 0;
            while (index < PF_Count)
            {
                ++index;
                if (strcmp(pixelFormatString[index], formatStr) == 0)
                {
                    format = (PixelFormat)(PF_Invalid + index);
                    break;
                }
            }
            
            if (format != PF_Invalid)
            {
                // Create the texture
                TextureDescriptor desc;
                desc.arrayLength = 1;
                desc.pixelFormat = format;
                desc.width = width / scaleFactor;
                desc.height = height / scaleFactor;
                desc.depth = 1;
                desc.mipmapped = false;
                desc.type = TextureType::TT_Type2D;
                desc.usage = TextureUsage::TU_RenderTarget;
                desc.sampleCount = 1;
                Texture* texture = GfxDevice::GetDevice()->NewTexture(desc);
                RYUJIN_ASSERT(texture, *String::Printf("Failed to creaste RT %s", name));
                
                RenderTargetPoolItemDescriptor rtDesc;
                rtDesc.format = format;
                rtDesc.width = desc.width;
                rtDesc.height = desc.height;
                rtDesc.depth = 1;
                rtDesc.arrayLength = 1;
                rtDesc.nameHash = CalculateFNV(name);
                uint64 hash = CRC::MemCrc(reinterpret_cast<uint8*>(&rtDesc), sizeof(rtDesc));
                RenderTargetPoolItem* item = RYUJIN_NEW RenderTargetPoolItem();
                item->texture = texture;
                item->name = name;
                renderTargets.Add(hash, item);
            }
        }
        
        return true;
    }
    
    void RenderTargetPool::Shutdown()
    {
        HashTable<uint64, RenderTargetPoolItem*>::Iterator it(renderTargets);
        while (it)
        {
            delete it->value;
            ++it;
        }
    }
    
    RenderTargetPoolItem* RenderTargetPool::GetRenderTargetPoolItem(RenderTargetPoolItemDescriptor &desc)
    {
        RenderTargetPoolItem* item = nullptr;
        uint64 hash = CRC::MemCrc(reinterpret_cast<uint8*>(&desc), sizeof(desc));
        renderTargets.TryGetValue(hash, item);
        RYUJIN_ASSERT(item, *String::Printf("Failed to find RT %d", desc.nameHash));
        
        return item;
    }
    
    RenderTargetPoolItem* RenderTargetPool::GetRenderTargetPoolItem(uint64 hash)
    {
        RenderTargetPoolItem* item = nullptr;
        renderTargets.TryGetValue(hash, item);
        RYUJIN_ASSERT(item, *String::Printf("Failed to find RT with hash %d", hash));
        
        return item;
    }
}
