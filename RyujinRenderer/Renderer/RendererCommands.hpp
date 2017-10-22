
#ifndef RENDERER_COMMANDS_HPP
#define RENDERER_COMMANDS_HPP


#include "Renderer.hpp"


namespace Ryujin
{
    namespace RendererBackendDispatchFunctions
    {
        //Forward declare dispatch functions
        static void Draw(Renderer* renderer, const void* data);
        
        static void CopyConstBufferData(Renderer* renderer, const void* data);
        
    }
    typedef void (*BackendDispatchFunction)(Renderer*, const void*);
    
    namespace RendererCommands
    {
        #define DECLARE_RENDER_COMMAND(name, code) \
                struct name \
                { \
                    static const BackendDispatchFunction DispatchFunction; \
                    code \
                }; \
                RYUJIN_STATIC_ASSERT(std::is_pod<name>::value == true, CONCAT_STR_WITH_SPACE(name, must be a POD.));
        
        DECLARE_RENDER_COMMAND(Draw,
                               uint32 vertexCount;
                               uint32 startVertex;
                               
                               VertexDescriptor vertexDescriptor;
                               VertexBufferHandle vertexBuffer;
                               IndexBufferHandle indexBuffer;
                               );
        
        DECLARE_RENDER_COMMAND(DrawIndexed,
                               uint32 indexCount;
                               uint32 startIndex;
                               uint32 baseVertex;
                               bool bUse16BitIndexType;
                    
                               VertexDescriptor vertexDescriptor;
                               VertexBufferHandle vertexBuffer;
                               IndexBufferHandle indexBuffer;
                               );
        
        DECLARE_RENDER_COMMAND(DrawInstanced,);
        
        DECLARE_RENDER_COMMAND(CopyConstantBufferData,
                               ConstantBufferHandle buffer;
                               void* data;
                               PtrSize dataSize;
                               );
    }
    
    namespace RendererBackendDispatchFunctions
    {
        static void CreateVertexBuffer(Renderer* renderer, const void* data)
        {
            
        }
        
        static void Draw(Renderer* renderer, const void* data)
        {
            const RendererCommands::Draw* draw = reinterpret_cast<const RendererCommands::Draw*>(data);
            
            Debugf(Renderer, "Draw command: ", draw->vertexCount, " ", draw->startVertex);
        }
        
        static void CopyConstBufferData(Renderer* renderer, const void* data)
        {
            const RendererCommands::CopyConstantBufferData* typedData = reinterpret_cast<const RendererCommands::CopyConstantBufferData*>(data);
            Debugf(Renderer, "Copy command: ", typedData->dataSize);
        }
    }
}



#endif