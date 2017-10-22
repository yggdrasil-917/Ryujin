
#include "RendererCommands.hpp"


namespace Ryujin
{
    namespace RendererCommands
    {
        const BackendDispatchFunction Draw::DispatchFunction = &RendererBackendDispatchFunctions::Draw;
        const BackendDispatchFunction CopyConstantBufferData::DispatchFunction = &RendererBackendDispatchFunctions::CopyConstBufferData;
    }
}