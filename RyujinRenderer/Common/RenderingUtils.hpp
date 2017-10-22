#pragma once


#include "ResourceDescriptors.hpp"
#include "RyujinCore/CoreUtils/CRC.hpp"


namespace Ryujin
{
    namespace GfxUtils
    {
        FORCEINLINE uint64 HashDepthStencilState(CompareFunction compareFunc, bool bWriteEnabled,
                                                 CompareFunction frontStencilComp = CF_Never, StencilOperation frontStencilFailure = SO_Keep,
                                                 StencilOperation frontDepthFailure = SO_Keep, StencilOperation frontDepthStencilPass = SO_Keep,
                                                 uint32 frontReadMask = 0xffffffff, uint32 frontWriteMask = 0xffffffff,
                                                 CompareFunction backStencilComp = CF_Never, StencilOperation backStencilFailure = SO_Keep,
                                                 StencilOperation backDepthFailure = SO_Keep, StencilOperation backDepthStencilPass = SO_Keep,
                                                 uint32 backReadMask = 0xffffffff, uint32 backWriteMask = 0xffffffff)
        {
            DepthStencilDescriptor ds;
            ds.depth = { compareFunc, bWriteEnabled };
            ds.frontfaceStencil = { frontStencilFailure, frontDepthFailure, frontDepthStencilPass, frontStencilComp, frontReadMask, frontWriteMask, 255.0f, 255.0f };
            ds.backfaceStencil = { backStencilFailure, backDepthFailure, backDepthStencilPass, backStencilComp, backReadMask, backWriteMask, 255.0f, 255.0f };
            
            return CRC::MemCrc(reinterpret_cast<uint8*>(&ds), sizeof(ds));
        }
        
        FORCEINLINE uint64 HashSampler(SamplerDescriptor& desc)
        {
            return CRC::MemCrc(reinterpret_cast<uint8*>(&desc), sizeof(desc));
        }
    }
}
