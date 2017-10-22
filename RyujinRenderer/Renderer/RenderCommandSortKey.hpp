
#ifndef RENDER_COMMAND_SORT_KEY_HPP
#define RENDER_COMMAND_SORT_KEY_HPP



#include "RyujinCore/Containers/DynamicArray.hpp"


namespace Ryujin
{
    // http://aras-p.info/blog/2014/01/16/rough-sorting-by-depth/
    FORCEINLINE uint32 FloatFlip(uint32 f)
    {
        uint32 mask = -int32(f >> 31) | 0x80000000;
        return f ^ mask;
    }
    
    FORCEINLINE uint32 DepthToBits (float depth, uint32 numHighestBitsToTake = 10)
    {
        union { float f; uint32 i; } f2i;
        f2i.f = depth;
        f2i.i = FloatFlip(f2i.i); // flip bits to be sortable
        uint32 b = f2i.i >> (sizeof(uint32) * 8 - numHighestBitsToTake); // take highest numHighestBitsToTake bits
        
        return b;
    }
    
    
    enum RenderPhase
    {
        RP_PreDraw = 0,
        RP_Draw,
        RP_PostDraw
    };
    
    enum ERenderFeature
    {
        RF_Prepass = 0,
        RF_ForwardLighting,
        RF_SSAO,
        RF_PostProcess,
        
        RF_RenderFeatureCount
    };
    
    // TODO: Looking into encapsulating this info inside the PSO
    enum TranslucencyType
    {
        TT_Opaque,
        TT_Masked,
        TT_Translucent
    };
    
    
    using CommandPacket = void*;
    
    template<typename Key>
    struct SortCommand
    {
        Key key;
        //        PtrSize offset;
        CommandPacket command;
        
        // Compare operators and division operator only used for radix sort
        bool operator>(const SortCommand& right) const
        {
            return key > right.key;
        }
        
        bool operator<(const SortCommand& right) const
        {
            return key < right.key;
        }
        
        Key operator/(const Key denom) const
        {
            return key / denom;
        }
    };
    
    template<typename Key>
    static bool SortCommandCompare(SortCommand<Key> a, SortCommand<Key> b)
    {
        return a.key < b.key;
    }
    
    enum class SortKeyType : uint8
    {
        SortByDepthFrontToBack = 0,
        SortByDepthBackToFront,
    };
    
    struct SortKeyElement
    {
        int16 bitPosition;
        int16 value;
    };
    
    using CustomSortKeyDescriptor = DynamicArray<SortKeyElement>;
    
    // values used for 16 bit sort key
#define BASE_RENDER_PHASE_BITS 1
#define BASE_RENDER_PASS_BITS 2
#define BASE_DEPTH_BITS 8
#define BASE_PIPELINE_STATE_BITS 5
    
    template<typename KeyType>
    class SortKeyFactory
    {
    private:
        static const KeyType mask = (KeyType)-1;
        static const uint8 keyBits = sizeof(KeyType) * 8;
        static const uint8 bitCountModifier = keyBits / 32;
        static const uint8 renderPhaseBits = BASE_RENDER_PHASE_BITS * bitCountModifier;
        static const uint8 renderPassBits = BASE_RENDER_PASS_BITS * bitCountModifier;
        static const uint8 depthBits = BASE_DEPTH_BITS * bitCountModifier;
        static const uint8 pipelineStateBits = BASE_PIPELINE_STATE_BITS * bitCountModifier;
        static const uint8 renderPassPosition = keyBits - renderPhaseBits;
        static const uint8 depthPosition = renderPassPosition - renderPassBits;
        static const uint8 pipelineStatePosition = depthPosition - depthBits;
        static const uint8 pipelineStatePositionPreDepth = renderPassPosition - renderPassBits;
        static const uint8 depthPositionPostPipelineState = pipelineStatePositionPreDepth - depthBits;
        
        
        
        static KeyType GenerateFrontToBackKey(uint32 renderPhase, uint32 renderPass, float depth, uint32 pipelineStateId, bool bReduceStateChange)
        {
            const uint32 depthToBits = DepthToBits(depth);
            
            KeyType key = 0;
            if (bReduceStateChange)
            {
                key =   ((KeyType(renderPhase) << (keyBits - renderPhaseBits)) & mask) |
                        ((KeyType(renderPass) << (renderPassPosition)) & mask) |
                        ((KeyType(pipelineStateId) << (pipelineStatePositionPreDepth)) & mask) |
                        ((KeyType(depthToBits) << (depthPositionPostPipelineState)) & mask);
            }
            else
            {
                key =   ((KeyType(renderPhase) << (keyBits - renderPhaseBits)) & mask) |
                        ((KeyType(renderPass) << (renderPassPosition)) & mask) |
                        ((KeyType(depthToBits) << (depthPosition)) & mask) |
                        ((KeyType(pipelineStateId) << (pipelineStatePosition)) & mask);
            }
            
            return key;
        }
        
        static KeyType GenerateBackToFrontKey(uint32 renderPhase, uint32 renderPass, float depth, uint32 pipelineStateId, bool bReduceStateChange)
        {
            const uint32 depthToBits = DepthToBits(1.0f - depth);
            
            KeyType key = 0;
            if (bReduceStateChange)
            {
                key =   ((KeyType(renderPhase) << (keyBits - renderPhaseBits)) & mask) |
                        ((KeyType(renderPass) << (renderPassPosition)) & mask) |
                        ((KeyType(pipelineStateId) << (pipelineStatePositionPreDepth)) & mask) |
                        ((KeyType(depthToBits) << (depthPositionPostPipelineState)) & mask);
            }
            else
            {
                key =   ((KeyType(renderPhase) << (keyBits - renderPhaseBits)) & mask) |
                        ((KeyType(renderPass) << (renderPassPosition)) & mask) |
                        ((KeyType(depthToBits) << (depthPosition)) & mask) |
                        ((KeyType(pipelineStateId) << (pipelineStatePosition)) & mask);
            }
            
            return key;
        }
        
    public:
        
        static KeyType GenerateDrawSortKey(SortKeyType keyType, uint32 renderPhase, uint32 renderPass, float depth, uint32 pipelineStateId, bool bReduceStateChange = false)
        {
            switch (keyType)
            {
                // opaque and everything else
                case SortKeyType::SortByDepthFrontToBack:
                default:
                    return GenerateFrontToBackKey(renderPhase, renderPass, depth, pipelineStateId, bReduceStateChange);
                    
                // Useful for transparent geometry
                case SortKeyType::SortByDepthBackToFront:
                    return GenerateBackToFrontKey(renderPhase, renderPass, depth, pipelineStateId, bReduceStateChange);
            }
        }
        
        
        static KeyType GenerateCustomSortKey(const CustomSortKeyDescriptor& keyDesc)
        {
            KeyType key = 0;
            for (CustomSortKeyDescriptor::ConstIterator it = keyDesc.Begin(); it != keyDesc.end(); ++it)
            {
                key |= ( KeyType((*it).value) << (*it).bitPosition ) & mask;
            }
            
            return key;
        }
    };
    
    using SortKeyFactoryU64 = SortKeyFactory<uint64>;
    using SortKeyFactoryU32 = SortKeyFactory<uint32>;
}


#endif
