#pragma once

#include "RendererCommon.hpp"

#include "RyujinCore/Containers/HashTable.hpp"


namespace Ryujin
{
    struct RenderTargetPoolItemDescriptor
    {
        PixelFormat format;
        uint16 width;
        uint16 height;
        uint16 depth = 1;
        uint8 arrayLength = 1;
        uint32 nameHash;
    };
    
    struct RenderTargetPoolItem
    {
        TextureBase* texture;
        String name;
        
        ~RenderTargetPoolItem()
        {
            delete texture;
            texture = nullptr;
        }
    };
    
    class RENDER_API RenderTargetPool
    {
    private:
        HashTable<uint64, RenderTargetPoolItem*> renderTargets;
        
    public:
        static RenderTargetPool* Instance();
        
        bool Initialize(uint16 width, uint16 height);
        void Shutdown();
        
        RenderTargetPoolItem* GetRenderTargetPoolItem(RenderTargetPoolItemDescriptor& desc);
        RenderTargetPoolItem* GetRenderTargetPoolItem(uint64 hash);
    };
}
