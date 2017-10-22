
#include "Renderer.hpp"
#include "RenderTargetPool.hpp"
#include "CommandBucket.hpp"
#include "RyujinCore/Math/MathUtils.hpp"
#include "RyujinCore/Window/Viewport.hpp"
#include "RyujinCore/Profiler/ChromeProfiler.hpp"
#include "RyujinCore/Profiler/PixProfiler.hpp"
#include "RyujinCore/OS/Application.hpp"

#include "RyujinExtensions/EntitySystem/MeshComponent.hpp"
#include "../RenderUtils/TextureUtils.hpp"


namespace Ryujin
{
    void SimpleTest::Init()
    {
        rtDesc.format = PF_BGRA8Unorm;
        rtDesc.arrayLength = 1;
        rtDesc.width = 640;
        rtDesc.height = 360;
        rtDesc.depth = 1;
        rtDesc.nameHash = CalculateFNV("Scene Color");
        
        defaultLibrary = RYUJIN_NEW ShaderModule();
#if PLATFORM_MAC
        defaultLibrary->CreateFromFile(*String::Printf("%sShaders/SimpleShader.metallib", AppInfo::GetResourcesDir()));
#endif
        pipelineState = GfxDevice::GetDevice()->NewPipelineState(defaultLibrary, "vertex_main", "fragment_main");
        
        VertexDescriptor vd;
        vd.perVertex = true;
        vd.vertexAttributes[0] = { VET_Float4, VERTEX_BUFFER_SLOT, 0 };
        vd.vertexAttributes[1] = { VET_Float2, VERTEX_BUFFER_SLOT, sizeof(Vector4) };
        vd.vertAttributeCount = 2;
        vd.stride = sizeof(Vector4) + sizeof(Vector2);
        vd.vertexAttributesEnabled = VA_POSITION | VA_UV;
        RenderPipelineStateDescriptor rpd;
        rpd.label = "texturedCube";
        rpd.vertexShaderName = "vertex_textured";
        rpd.fragShaderName = "fragment_textured";
        rpd.vertexDescriptor = vd;
        rpd.depthFormat = PF_Depth32Float;
        rpd.colorAttachment[0].format = PF_BGRA8Unorm;
        rpd.colorAttachmentCount = 1;
//        pipelineStateTextured = GfxDevice::GetDevice()->NewPipelineState(defaultLibrary, "vertex_textured", "fragment_textured");
        pipelineStateTextured = GfxDevice::GetDevice()->NewPipelineState(defaultLibrary, rpd);
        
        const float buff[] =
        {
            // Front face
            -1.0, -1.0,  1.0, 1.0,
            0, 0, 0, 1,
            1.0, -1.0,  1.0, 1.0,
            1, 0, 0, 1,
            1.0,  1.0,  1.0, 1.0,
            0, 1, 0, 1,
            -1.0,  1.0,  1.0, 1.0,
            1, 1, 0, 1,
            
            // Back face
            -1.0, -1.0, -1.0, 1.0,
            0, 0, 0, 1,
            -1.0,  1.0, -1.0, 1.0,
            1, 0, 0, 1,
            1.0,  1.0, -1.0, 1.0,
            0, 1, 0, 1,
            1.0, -1.0, -1.0, 1.0,
            1, 1, 0, 1,
            
            // Top face
            -1.0,  1.0, -1.0, 1.0,
            0, 0, 0, 1,
            -1.0,  1.0,  1.0, 1.0,
            1, 0, 0, 1,
            1.0,  1.0,  1.0, 1.0,
            0, 1, 0, 1,
            1.0,  1.0, -1.0, 1.0,
            1, 1, 0, 1,
            
            // Bottom face
            -1.0, -1.0, -1.0, 1.0,
            0, 0, 0, 1,
            1.0, -1.0, -1.0, 1.0,
            1, 0, 0, 1,
            1.0, -1.0,  1.0, 1.0,
            0, 1, 0, 1,
            -1.0, -1.0,  1.0, 1.0,
            1, 1, 0, 1,
            
            // Right face
            1.0, -1.0, -1.0, 1.0,
            0, 0, 0, 1,
            1.0,  1.0, -1.0, 1.0,
            1, 0, 0, 1,
            1.0,  1.0,  1.0, 1.0,
            0, 1, 0, 1,
            1.0, -1.0,  1.0, 1.0,
            1, 1, 0, 1,
            
            // Left face
            -1.0, -1.0, -1.0, 1.0,
            0, 0, 0, 1,
            -1.0, -1.0,  1.0, 1.0,
            1, 0, 0, 1,
            -1.0,  1.0,  1.0, 1.0,
            0, 1, 0, 1,
            -1.0,  1.0, -1.0, 1.0,
            1, 1, 0, 1,
        };
        
        const uint16 index[] =
        {
            0,  1,  2,      0,  2,  3,    // front
            4,  5,  6,      4,  6,  7,    // back
            8,  9,  10,     8,  10, 11,   // top
            12, 13, 14,     12, 14, 15,   // bottom
            16, 17, 18,     16, 18, 19,   // right
            20, 21, 22,     20, 22, 23,   // left
        };
        
        const float buff2[] =
        {
            // -X
            -0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
            -0.5, -0.5,  0.5, 1.0, 0.0, 1.0,
            -0.5,  0.5,  0.5, 1.0, 1.0, 1.0,
            -0.5,  0.5, -0.5, 1.0, 1.0, 0.0,
            // +X
            0.5, -0.5,  0.5, 1.0, 0.0, 0.0,
            0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
            0.5,  0.5, -0.5, 1.0, 1.0, 1.0,
            0.5,  0.5,  0.5, 1.0, 1.0, 0.0,
            // -Y
            -0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
            0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
            0.5, -0.5,  0.5, 1.0, 1.0, 1.0,
            -0.5, -0.5,  0.5, 1.0, 1.0, 0.0,
            // + Y
            -0.5,  0.5,  0.5, 1.0, 0.0, 0.0,
            0.5,  0.5,  0.5, 1.0, 0.0, 1.0,
            0.5,  0.5, -0.5, 1.0, 1.0, 1.0,
            -0.5,  0.5, -0.5, 1.0, 1.0, 0.0,
            // -Z
            0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
            -0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
            -0.5,  0.5, -0.5, 1.0, 1.0, 1.0,
            0.5,  0.5, -0.5, 1.0, 1.0, 0.0,
            // +Z
            -0.5, -0.5,  0.5, 1.0, 0.0, 0.0,
            0.5, -0.5,  0.5, 1.0, 0.0, 1.0,
            0.5,  0.5,  0.5, 1.0, 1.0, 1.0,
            -0.5,  0.5,  0.5, 1.0, 1.0, 0.0,
        };
        
        const uint16 indices[] =
        {
            3,  1,  2,  0,  1,  3,
            7,  5,  6,  4,  5,  7,
            11,  9, 10,  8,  9, 11,
            15, 13, 14, 12, 13, 15,
            19, 17, 18, 16, 17, 19,
            23, 21, 22, 20, 21, 23,
        };
        
        vb = GfxDevice::GetDevice()->NewVertexBuffer(buff, sizeof(float), countof(buff));
        vbTextured = GfxDevice::GetDevice()->NewVertexBuffer(buff2, sizeof(float), countof(buff2));
        ib = GfxDevice::GetDevice()->NewIndexBuffer(index, sizeof(uint16), countof(index), true);
        ibTextured = GfxDevice::GetDevice()->NewIndexBuffer(indices, sizeof(uint16), countof(indices), true);
        sceneView.SetPosition(Vector3(0,0,5));
        sceneView.SetDirection(Vector3(0,0,-1), Vector3(0,1,0));
        sceneView.SetPerspective(60, 640.0/320.0, 0.1);
        sceneView.Update();
        worldMatrix.AddTranslation(Vector3(-1.5,0,0));
        worldMatrixTextured.AddTranslation(Vector3(1.5,0,0));
        worldMatrixTextured.Scale(Vector3(2));
        
        PerObjectCB objectCB;
        objectCB.worldMatrix = worldMatrix.ToMatrix();
        cb = GfxDevice::GetDevice()->NewConstantBuffer(&objectCB, sizeof(objectCB));
        objectCB.worldMatrix = worldMatrixTextured.ToMatrix();
        cbTextured = GfxDevice::GetDevice()->NewConstantBuffer(&objectCB, sizeof(objectCB));
        PerFrameCB frameCB;
        frameCB.cameraWorldPosition = sceneView.GetWorldPosition();
        frameCB.view = sceneView.GetView();
        frameCB.viewInv = sceneView.GetViewInverse();
        frameCB.projection = sceneView.GetProjection();
        frameCB.projectionInv = sceneView.GetProjectionInverse();
        frameCB.viewProjection = sceneView.GetViewProjection();
        frameCB.viewProjectionInv = sceneView.GetViewProjectionInverse();
        perFrameCB = GfxDevice::GetDevice()->NewConstantBuffer(&frameCB, sizeof(frameCB));
        ((Renderer*)ServiceLocator::GetRenderer())->SetFrameCB(perFrameCB);
        depth = GfxDevice::GetDevice()->NewTexture(640, 320, PF_Depth32Float, false, TU_BackBuffer);
        
        uint8* data = TextureUtils::CreateCheckerBoardTexture(64, 64);
        checkerboard = GfxDevice::GetDevice()->NewTexture(64, 64, PF_RGBA8Unorm, true, TU_ReadWrite);
        checkerboard->UpdateTexture(Rect<uint16>(0, 0, 64, 64), data, 0);
        checkerboard->GenerateMips(GfxDevice::GetDevice()->GetImmediateCmdQueue());
        
        SamplerDescriptor sd;
        sd.minFilter = SMMF_Nearest;
        sd.magFilter = SMMF_Nearest;
        sd.mipFilter = SMF_Nearest;
        sd.r = SAM_Repeat;
        sd.s = SAM_Repeat;
        sd.t = SAM_Repeat;
        checkerboardSampler = GfxDevice::GetDevice()->GetOrCreateSampler(sd);
        delete[] data;
        
        Uniform colorCB;
        colorCB.color[0] = 1.0f;
        colorCB.color[1] = 0.0f;
        colorCB.color[2] = 1.0f;
        colorCB.color[3] = 1.0f;
        ConstantBufferPtr cbUnlit = GfxDevice::GetDevice()->NewConstantBuffer(&colorCB, sizeof(colorCB));
        unlitMat.SetFragmentCB(cbUnlit);
        pipelineStateUnlit = GfxDevice::GetDevice()->NewPipelineState(defaultLibrary, "vertex_basic", "fragment_basic");
        worldMatrixUnlit.AddTranslation(Vector3(0,1.5,0));
        worldMatrixUnlit.Scale(Vector3(0.5f, 0.5f, 0.5f));
        objectCB.worldMatrix = worldMatrixUnlit.ToMatrix();
        cbVertexUnlit = GfxDevice::GetDevice()->NewConstantBuffer(&objectCB, sizeof(objectCB));
        
        Image skyboxFaces[6];
        if (TextureUtils::LoadCubemap("Yokohama", skyboxFaces, ".jpg"))
        {
            uint16 size = skyboxFaces[0].GetWidth();
            TextureDescriptor skyboxDesc = {};
            skyboxDesc.width = size;
            skyboxDesc.mipmapped = false;
            skyboxDesc.pixelFormat = PF_RGBA8Unorm;
            skyboxDesc.usage = TU_ReadWrite;
            skybox = GfxDevice::GetDevice()->NewTextureCube(skyboxDesc);
            if (skybox)
            {
                Rect<uint16> region(0, 0, size, size);
                for (int32 i = 0; i < 6; ++i)
                {
                    skybox->UpdateTexture(region, skyboxFaces[i].GetImageData(), 0, i);
                }
            }
        }
        skyboxFeature = RYUJIN_NEW SkyboxFeature();
        skyboxFeature->Initialize(defaultLibrary, (Renderer*)ServiceLocator::GetRenderer());
    }
    
    void SimpleTest::Render(float dt, DynamicArray<SceneView*>& views)
    {
        SCOPED_PROFILE_EVENT(Render, PC_Rendering, 255, 255, 0);
        
        SceneView* view = views[0];
        view->Update();
        PerFrameCB frameCB;
        frameCB.cameraWorldPosition = view->GetWorldPosition();
        frameCB.view = view->GetView();
        frameCB.viewInv = view->GetViewInverse();
        frameCB.projection = view->GetProjection();
        frameCB.projectionInv = view->GetProjectionInverse();
        frameCB.viewProjection = view->GetViewProjection();
        frameCB.viewProjectionInv = view->GetViewProjectionInverse();
        perFrameCB->Update(&frameCB, sizeof(frameCB));
        
        BackBufferImagePtr image(GfxDevice::GetDevice()->GetSwapChain()->GetNextImage());
        skyboxFeature->Render(GfxDevice::GetDevice()->GetImmediateCmdQueue(), defaultLibrary);
        
        CommandBufferPtr commandBuffer(GfxDevice::GetDevice()->GetImmediateCmdQueue()->NewCommandBuffer());
        Texture* sceneColor = dynamic_cast<Texture*>(RenderTargetPool::Instance()->GetRenderTargetPoolItem(rtDesc)->texture);

        // Set up a render pass to draw into the current drawable's texture
        RenderPassDescriptor desc;
        desc.colorAttachments[0].texture = sceneColor;
        desc.colorAttachments[0].loadAction = GfxLoadAction::LA_Load;
        desc.colorAttachments[0].storeAction = GfxStoreAction::SA_Store;
        desc.depthAttachment.texture = depth;
        desc.depthAttachment.loadAction = GfxLoadAction::LA_Clear;
        desc.depthAttachment.storeAction = GfxStoreAction::SA_Store;
        desc.depthAttachment.clearColor[0] = 1.0f;

        GfxEncoderPtr encoder(commandBuffer->NewGfxEncoderFromDescriptor(desc));

        static float yRot = 0.0;
        yRot += 0.5f * dt;
        worldMatrix.SetRotation(Math::EulerToQuaternion(Vector3(0,yRot,0)));

        PerObjectCB objectCB;
        objectCB.worldMatrix = worldMatrix.ToMatrix();
        cb->Update(&objectCB, sizeof(objectCB));

        encoder->PushDebugGroup("Ryujin test");
        encoder->SetPipelineState(pipelineState);
        encoder->SetDepthStencilState(CompareFunction::CF_LessEqual, true);
        encoder->SetTriangleWinding(FW_CounterClockwise);
        encoder->SetCullingMode(CM_Back);
        encoder->SetVertexBuffer(vb, 0, VERTEX_BUFFER_SLOT);
        encoder->SetVertexConstantBuffer(cb, 0, MESH_CB_SLOT);
        encoder->SetVertexConstantBuffer(perFrameCB, 0, FRAME_CB_SLOT);
        encoder->DrawIndexed(ib);

        // Textured cube
        worldMatrixTextured.SetRotation(Math::EulerToQuaternion(Vector3(0,yRot,0)));
        objectCB.worldMatrix = worldMatrixTextured.ToMatrix();
        cbTextured->Update(&objectCB, sizeof(objectCB));
        encoder->SetPipelineState(pipelineStateTextured);
        encoder->SetVertexBuffer(vbTextured, 0, VERTEX_BUFFER_SLOT);
        encoder->SetVertexConstantBuffer(cbTextured, 0, MESH_CB_SLOT);
        encoder->SetVertexConstantBuffer(perFrameCB, 0, FRAME_CB_SLOT);
        encoder->SetFragmentTexture(checkerboard, 0);
        encoder->SetFragmentSampler(checkerboardSampler, 0);
        encoder->DrawIndexed(ibTextured);

        encoder->SetPipelineState(pipelineStateUnlit);
        encoder->SetVertexBuffer(vb, 0, VERTEX_BUFFER_SLOT);
        worldMatrixUnlit.SetRotation(Math::EulerToQuaternion(Vector3(0,yRot,0)));
        objectCB.worldMatrix = worldMatrixUnlit.ToMatrix();
        cbVertexUnlit->Update(&objectCB, sizeof(objectCB));
        encoder->SetVertexConstantBuffer(cbVertexUnlit, 0, MESH_CB_SLOT);
        encoder->SetFragmentConstantBuffer(unlitMat.GetFragmentCB(), 0, MESH_CB_SLOT);
        encoder->SetVertexConstantBuffer(perFrameCB, 0, FRAME_CB_SLOT);
        encoder->DrawIndexed(ib);

        encoder->PopDebugGroup();
        encoder->Finish();

//        commandBuffer->Present(image);

        commandBuffer->Commit();
        
        GfxDevice::GetDevice()->CopyTextureToBackBuffer(sceneColor, image);
    }
    
    Renderer::Renderer()
    {
        
    }
    
    
    Renderer::~Renderer()
    {
        
    }
    
    
    bool Renderer::Initialize(int32 inResX, int32 inResY, Viewport* inViewport)
	{
		ConfigCache configCache;
		if (!configCache.LoadConfig("renderer.ini"))
		{
			Errorf(Renderer, "Failed to load renderer.ini");
			return false;
		}

        ServiceLocator::Provide(this);
        
        SceneView sceneView;
        sceneView.SetPosition(Vector3(0,0,3));
        sceneView.SetDirection(Vector3(0,0,-1), Vector3(0,1,0));
        sceneView.SetPerspective(60, (float)inResX / (float)inResY, 0.1);
        sceneView.Update();
        views.Add(sceneView);
        
        device = static_cast<GfxDevice*>(GfxDevice::GetDevice());
        if (device->CreateDevice() == false)
            return false;
        
		device->CacheWindowReference(inViewport->GetWindow());
        if (!device->CreateDevice())
        {
            Errorf(Renderer, "Failed to initialize device");
            return false;
        }

		device->PrintDeviceInfo();

		resX = inResX;
		resY = inResY;

		if (!device->PostInit(resX, resY))
		{
			return false;
		}
        
        viewport = inViewport;
        
        PlatformSpecificInitialization();
        
        // Temp initialization of a simple scene
        simpleTest = RYUJIN_NEW SimpleTest();
        simpleTest->Init();
        // end temp code
        
        PerFrameCB data;
        data.cameraWorldPosition = sceneView.GetWorldPosition();
        data.view = sceneView.GetView();
        data.viewInv = sceneView.GetViewInverse();
        data.projection = sceneView.GetProjection();
        data.projectionInv = sceneView.GetProjectionInverse();
        data.viewProjection = sceneView.GetViewProjection();
        data.viewProjectionInv = sceneView.GetViewProjectionInverse();
        frameCB = device->NewConstantBuffer(&data, sizeof(data));
        
        RenderTargetPool::Instance()->Initialize(resX, resY);
        
        return true;
    }
    
    void Renderer::RenderScene(float dt, DynamicArray<SceneView*>& views)
    {
#if PLATFORM_MAC
        @autoreleasepool
#endif
        {
            
            simpleTest->Render(dt, views);
            
        }
    }
    
    
    void Renderer::Resize(const uint32 inWidth, const uint32 inHeight)
	{
		resX = MathUtils::Clamp<uint32>(inWidth, 1, 3840);
		resY = MathUtils::Clamp<uint32>(inHeight, 1, 2160);
        viewport->Resize(inWidth, inHeight);

		device->OnWindowResize(inWidth, inHeight);
        RenderTargetPool::Instance()->Initialize(inWidth, inHeight);
    }
    
    
    void Renderer::Shutdown()
    {
        Threading::Sleep(16); // hack to allow gpu enough time to finish
        frameCB.SafeRelease();
        perObjectCB.SafeRelease();
        delete simpleTest;
        RenderTargetPool::Instance()->Shutdown();
        viewport->Close();
        device->Shutdown();
    }
    
#if PLATFORM_WINDOWS || PLATFORM_LINUX
	void Renderer::PlatformSpecificInitialization()
	{

	}
#endif
}
