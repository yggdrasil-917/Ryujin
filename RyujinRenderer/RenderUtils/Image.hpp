#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "../Renderer/RendererBase.hpp"

namespace Ryujin
{
	class RENDER_API Image
	{
	private:
		int32 width;
		int32 height;
		int32 numComponents; // # of 8-bit components per pixel

		uint8 *data;

	public:
		Image() : data(nullptr) {}
		~Image();

		bool LoadImageFromDisk(const char *filepath, int32 reqComp = 0);
        
        void SaveTGA(const char* filename);

		FORCEINLINE int32 GetWidth() const { return width; }
		FORCEINLINE int32 GetHeight() const { return height; }
        FORCEINLINE int32 GetBytesPerPixel() const { return numComponents; }
		FORCEINLINE int32 GetNumComponents() const { return numComponents; }
		FORCEINLINE uint8* GetImageData() const { return data; }
	};



}


#endif
