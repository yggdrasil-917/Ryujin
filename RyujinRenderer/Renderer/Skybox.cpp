#include "Skybox.hpp"
#include "Renderer.hpp"
#include "../RenderUtils/Image.hpp"
#include "../RenderUtils/TextureUtils.hpp"
#include "RyujinCore/CoreUtils/StringHash.hpp"

namespace Ryujin
{
#define WORLD_BOUNDS 10000.0f
#define WORLD_BOUNDS_HALF (WORLD_BOUNDS * 0.5f)
    void SkyboxFeature::Initialize(ShaderModulePtr library, Renderer* renderer)
    {
        float vertices[] =
        {
            // + Y
            -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0,  0.0, -1.0,  0.0, 0.0,
            WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0,  0.0, -1.0,  0.0, 0.0,
            WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0,  0.0, -1.0,  0.0, 0.0,
            -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0,  0.0, -1.0,  0.0, 0.0,
            // -Y
            -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0,  0.0,  1.0,  0.0, 0.0,
            WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0,  0.0,  1.0,  0.0, 0.0,
            WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0,  0.0,  1.0,  0.0, 0.0,
            -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0,  0.0,  1.0,  0.0, 0.0,
            // +Z
            -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0,  0.0,  0.0, -1.0, 0.0,
            WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0,  0.0,  0.0, -1.0, 0.0,
            WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0,  0.0,  0.0, -1.0, 0.0,
            -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0,  0.0,  0.0, -1.0, 0.0,
            // -Z
            WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0,  0.0,  0.0,  1.0, 0.0,
            -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0,  0.0,  0.0,  1.0, 0.0,
            -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0,  0.0,  0.0,  1.0, 0.0,
            WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0,  0.0,  0.0,  1.0, 0.0,
            // -X
            -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0,  1.0,  0.0,  0.0, 0.0,
            -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0,  1.0,  0.0,  0.0, 0.0,
            -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0,  1.0,  0.0,  0.0, 0.0,
            -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0,  1.0,  0.0,  0.0, 0.0,
            // +X
            WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0, -1.0,  0.0,  0.0, 0.0,
            WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0, -1.0,  0.0,  0.0, 0.0,
            WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, -WORLD_BOUNDS_HALF, 1.0, -1.0,  0.0,  0.0, 0.0,
            WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF,  WORLD_BOUNDS_HALF, 1.0, -1.0,  0.0,  0.0, 0.0,
        };
        
        uint16 indices[] =
        {
            0,  3,  2,  2,  1,  0,
            4,  7,  6,  6,  5,  4,
            8, 11, 10, 10,  9,  8,
            12, 15, 14, 14, 13, 12,
            16, 19, 18, 18, 17, 16,
            20, 23, 22, 22, 21, 20,
        };
        boxVb = GfxDevice::GetDevice()->NewVertexBuffer(vertices, sizeof(float), countof(vertices));
        boxIb = GfxDevice::GetDevice()->NewIndexBuffer(indices, sizeof(uint16), countof(indices), true);
        
        frameCb = renderer->GetFrameCb();
        
        Image skyboxFaces[6];
        if (TextureUtils::LoadCubemap("Yokohama", skyboxFaces, ".jpg"))
        {
            uint16 size = skyboxFaces[0].GetWidth();
            TextureDescriptor skyboxDesc = {};
            skyboxDesc.width = size;
            skyboxDesc.mipmapped = false;
            skyboxDesc.pixelFormat = PF_RGBA8Unorm;
            skyboxDesc.usage = TU_ReadWrite;
            texture = GfxDevice::GetDevice()->NewTextureCube(skyboxDesc);
            if (texture)
            {
                Rect<uint16> region(0, 0, size, size);
                for (int32 i = 0; i < 6; ++i)
                {
                    texture->UpdateTexture(region, skyboxFaces[i].GetImageData(), 0, i);
                }
            }
        }
        
//        rtDesc.format = PF_RGBA8Unorm;
        rtDesc.format = PF_BGRA8Unorm;
        rtDesc.arrayLength = 1;
        rtDesc.width = 640;
        rtDesc.height = 360;
        rtDesc.depth = 1;
        rtDesc.nameHash = CalculateFNV("Scene Color");
    }
    
    void SkyboxFeature::Shutdown()
    {
        boxVb.SafeRelease();
        boxIb.SafeRelease();
    }
    
    void SkyboxFeature::Render(CommandQueue* queue, ShaderModulePtr library)
    {
        static SamplerDescriptor samplerDesc =
        {
            .r = SAM_ClampToEdge,
            .s = SAM_ClampToEdge,
            .minFilter = SMMF_Linear,
            .magFilter = SMMF_Linear,
            .mipFilter = SMF_Linear
        };
        
        rpd.label = "skybox";
        rpd.vertexShaderName = "skyboxVertex";
        rpd.fragShaderName = "skyboxFragment";
        rpd.colorAttachment[0].format = PF_BGRA8Unorm;// PF_RGBA8Unorm;
        rpd.colorAttachmentCount = 1;
        VertexDescriptor vd;
        vd.vertAttributeCount = 0;
        rpd.vertexDescriptor = vd;
        rpd.depthFormat = PF_Invalid;
        PipelineStatePtr pso = GetOrCachePso(library, rpd);
        CommandBufferPtr commandBuffer(queue->NewCommandBuffer());
        RenderPassDescriptor desc;
        desc.colorAttachments[0].texture = RenderTargetPool::Instance()->GetRenderTargetPoolItem(rtDesc)->texture;
        desc.colorAttachments[0].loadAction = GfxLoadAction::LA_Clear;
        desc.colorAttachments[0].storeAction = GfxStoreAction::SA_Store;
        desc.depthAttachment.texture = nullptr;
        desc.depthAttachment.loadAction = GfxLoadAction::LA_Clear;
        desc.depthAttachment.storeAction = GfxStoreAction::SA_Store;
        desc.depthAttachment.clearColor[0] = 1.0f;
        
        GfxEncoderPtr encoder(commandBuffer->NewGfxEncoderFromDescriptor(desc));
        encoder->PushDebugGroup("Skybox");
        encoder->SetPipelineState(pso);
        encoder->SetDepthStencilState(CompareFunction::CF_LessEqual, false);
        encoder->SetVertexBuffer(boxVb, 0, VERTEX_BUFFER_SLOT);
        encoder->SetVertexConstantBuffer(frameCb, 0, FRAME_CB_SLOT);
        encoder->SetFragmentTexture(texture, 0);
        encoder->SetFragmentSampler(GfxDevice::GetDevice()->GetOrCreateSampler(samplerDesc), 0);
        encoder->DrawIndexed(boxIb);
        encoder->PopDebugGroup();
        encoder->Finish();
        
        commandBuffer->Commit();
    }
    
    void SkyboxFeature::PreCachePso()
    {
        
    }
}
