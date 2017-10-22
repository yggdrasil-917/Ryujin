
#ifndef TEXTURE_RESOURCE_MANAGER_HPP
#define TEXTURE_RESOURCE_MANAGER_HPP


#include "ResourceManager.hpp"
#include "RyujinRenderer/Renderer/RendererCommon.hpp"
#include "RyujinRenderer/Renderer/RenderResourceFactory.hpp"

#include "RyujinRenderer/RenderUtils/Image.hpp"
#include "RyujinCore/Math/MathUtils.hpp"

namespace Ryujin
{
    class TextureManager : public ResourceManager<GALTexture>
    {
    protected:
        
        void LoadDefaultAsset() OVERRIDE FINAL
        {
            defaultHandle = Get("default.png");
        }
        
        UniquePtr<GALTexture> Load(const String& filename) OVERRIDE FINAL
        {
            StringHash hash(*(path + filename));
            Image image;
            if (!image.LoadImageFromDisk(hash.ToString()))
                return UniquePtr<GALTexture>(nullptr);
            
            TextureDescriptor desc;
            desc.width = image.GetWidth();
            desc.height = image.GetHeight();
            desc.mipmapLevelCount = (MathUtils::IsPowerOfTwo(desc.width) && MathUtils::IsPowerOfTwo(desc.height)) ?
                                        MathUtils::Log2(MathUtils::Max<uint32>(desc.width, desc.height)) : 1;
            
            return RenderResourceFactory::CreateTexture(desc, image.GetImageData());
        }
        
        
        void Unload(AssetHandle inHandle) OVERRIDE FINAL
        {
            RenderResourceFactory::DestroyTexture(resources.GetObject(inHandle.handle));
        }
    };
}


#endif
