#pragma once


#include "../Common/ResourceDescriptors.hpp"

#include "stb_image.h"
#include "Image.hpp"


namespace Ryujin
{
    namespace TextureUtils
    {
#define UNCOOKED_TEXTURE_PATH "../../../Content/Textures/"
        
        
        static int32 GetBytesPerPixel(PixelFormat format)
        {
            if (format >= PixelFormat::PF_A8Unorm && format <= PixelFormat::PF_R8Sint)
            {
                return 1;
            }
            else if (format >= PixelFormat::PF_R16Unorm && format <= PixelFormat::PF_RG8Sint)
            {
                return 2;
            }
            else if (format >= PixelFormat::PF_R32Uint && format <= PixelFormat::PF_RGB9E5Float)
            {
                return 4;
            }
            else if (format >= PixelFormat::PF_RG32Uint && format <= PixelFormat::PF_RGBA16Float)
            {
                return 8;
            }
            else if (format >= PixelFormat::PF_RGBA32Uint && format <= PixelFormat::PF_RGBA32Float)
            {
                return 16;
            }
            
            return 0;
        }
        
        static bool IsDepthFormat(PixelFormat format)
        {
            return format >= PixelFormat::PF_Depth32Float && format <= PixelFormat::PF_Depth32Float_Stencil8;
        }
        
        static bool IsStencilFormat(PixelFormat format)
        {
            return format >= PixelFormat::PF_Depth32Float_Stencil8 && format <= PixelFormat::PF_Stencil8;
        }
        
        static bool LoadCubemap(const char* baseDir, Image (&faces)[6], const char* fileExtension)
        {
            const char* filenames[6] =
            {
                "posx",
                "negx",
                "posy",
                "negy",
                "posz",
                "negz"
            };
            for (int32 i = 0; i < 6; ++i)
            {
                // Need to add support for looking at resource dir first
                String filepath = String::Printf("%s%s/%s%s", UNCOOKED_TEXTURE_PATH, baseDir, filenames[i], fileExtension);
                if (faces[i].LoadImageFromDisk(*filepath, STBI_rgb_alpha) == false)
                {
                    return false;
                }
            }
            return true;
        }
        
        // Make an RGBA texture with checkerboard pattern
        static uint8* CreateCheckerBoardTexture(uint32 width, uint32 height)
        {
            uint8* data = RYUJIN_NEW uint8[width * height * 4];
            for (uint32 i = 0; i < width; ++i)
            {
                for (uint32 j = 0; j < height; ++j)
                {
                    uint8 c = (((i & 0x8) == 0) ^ ((j & 0x8)  == 0)) * 255;
                    uint32 index = 4 * (i * height + j);
                    data[index] = c;
                    data[index + 1] = c;
                    data[index + 2] = c;
                    data[index + 3] = 255;
                }
            }
            
            return data;
        }
    }
}
