#include "Image.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb.h"
#include "stb_image.h"
#include "RyujinCore/Allocators/Memory.hpp"


namespace Ryujin
{
    Image::~Image()
    {
        if (data)
        {
            stbi_image_free(data);
        }
    }

    bool Image::LoadImageFromDisk(const char *filepath, int32 reqComp)
    {
        if (filepath == nullptr)
            return false;

        data = stbi_load(filepath, &width, &height, &numComponents, reqComp);

        return data != nullptr;
    }


    void Image::SaveTGA(const char *filename)
    {
        FILE* fp = fopen(filename, "wb");
        if (!fp)
            return;
        
        byte a[32];
        
        int32 i = 0;
        a[i++]= 0;
        a[i++]= 0;
        a[i++]= 2;		/* uncompressed RGB	*/
        a[i++]= 0; a[i++]= 0;
        a[i++]= 0; a[i++]= 0;
        a[i++]= 0;
        a[i++]= 0; a[i++]=0;
        a[i++]= 0; a[i++]=0;
        a[i++]= width & 0xFF; a[i++]= width >> 8;
        a[i++]= height & 0xFF; a[i++]= height >> 8;
        a[i++]= 32;		/* 32 bits/pixel		*/
        a[i++]= 0x20;	/* origin at top/left	*/
        
        fwrite(&a[0], sizeof(byte), sizeof(a), fp);
        fwrite(&data[0], sizeof(uint8), width * height * numComponents, fp);
        
        fclose(fp);
    }

}


