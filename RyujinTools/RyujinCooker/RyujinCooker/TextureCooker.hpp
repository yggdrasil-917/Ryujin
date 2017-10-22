
#ifndef TEXTURE_COOKER_HPP
#define TEXTURE_COOKER_HPP


#include "CookerCommon.hpp"

#include "RyujinCore/Containers/String.hpp"
#include "RyujinCore/CoreUtils/BinaryWriter.hpp"


namespace Ryujin
{
    class TextureCooker
    {
    private:
        DynamicArray<CookableItem> texturesToCook;
        
    public:
        TextureCooker() {}
        ~TextureCooker();
        
        
        void QueueTextureToCook(const String& textureSourceFile);
        void CookQueuedTextures();
        
        void CookTexture(const CookableItem& texture);
    };
}



#endif