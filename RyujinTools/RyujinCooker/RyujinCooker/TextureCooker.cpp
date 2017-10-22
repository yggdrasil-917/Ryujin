

#include "TextureCooker.hpp"

#include "RyujinRenderer/RenderUtils/Image.hpp"
#include "RyujinCore/CoreUtils/BinaryReader.hpp"

#include "Ryujin/Ryujin/Versions.hpp"


namespace Ryujin
{
    TextureCooker::~TextureCooker()
    {
        
    }
    
    void TextureCooker::QueueTextureToCook(const String &textureSourceFile)
    {
        CookableItem item;
        item.targetFilename = COOKED_CONTENT_DIR + textureSourceFile;
        int32 extensionStartIndex = -1;
        for (int32 i = item.targetFilename.Length(); i >= 0; --i)
        {
            if (item.targetFilename[i] == '.')
            {
                extensionStartIndex = i;
                break;
            }
        }
        if (extensionStartIndex >= 0)
        {
            item.targetFilename.Replace(extensionStartIndex, item.targetFilename.Length() - extensionStartIndex, ".asset");
        }
        item.sourceFilename = TEXTURE_CONTENT_DIR + textureSourceFile;
        texturesToCook.AddUnique(item);
    }
    
    void TextureCooker::CookQueuedTextures()
    {
        SCOPED_PROFILE_EVENT(CookingTextures, PC_Cooker, 255, 255, 0);
        
        for (const auto& texture : texturesToCook)
        {
            Debugf(Info, "Cooking texture ", *texture.sourceFilename);
            CookTexture(texture);
        }
    }
    
    
    // TODO: Finalize format for the .asset variant of a texture
    void TextureCooker::CookTexture(const CookableItem& texture)
    {
        Image image;
        if (!image.LoadImageFromDisk(texture.sourceFilename))
        {
            Warnf(Info, "Failed to find texture ", *texture.sourceFilename);
            return;
        }
        
        BinaryWriter bw(*texture.targetFilename);
        
        bw.Write<int32>(TEXTURE_COOKER_VERSION);
        
        bw.WriteArray(image.GetImageData(), image.GetHeight() * image.GetHeight() * image.GetBytesPerPixel());
    }
}
