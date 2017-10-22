
#pragma once



#include "RenderResourceFactory.hpp"
#include "RenderResources.hpp"
#include "Material.hpp"
#include "SceneView.hpp"
#include "Skybox.hpp"

#include "RyujinCore/CoreUtils/ServiceLocator.hpp"


namespace Ryujin
{
    class Viewport;
    class ConfigCache;
    
    
    struct RENDER_API SimpleTest
    {
        ConstantBufferPtr perFrameCB;
        ShaderModulePtr defaultLibrary;
        PipelineStatePtr pipelineState;
        VertexBufferPtr vb;
        IndexBufferPtr ib;
        ConstantBufferPtr cb;
        Transform worldMatrix;
        SceneView sceneView;
        
        PipelineStatePtr pipelineStateTextured;
        VertexBufferPtr vbTextured;
        ConstantBufferPtr cbTextured;
        IndexBufferPtr ibTextured;
        Transform worldMatrixTextured;
        TexturePtr checkerboard;
        SamplerPtr checkerboardSampler;
        
        PipelineStatePtr pipelineStateUnlit;
        Transform worldMatrixUnlit;
        ConstantBufferPtr cbVertexUnlit;
        Material unlitMat;
        
        PipelineStatePtr skyboxPso;
        TextureCubePtr skybox;
        SkyboxFeature* skyboxFeature;
        
        TexturePtr depth;
        
        RenderTargetPoolItemDescriptor rtDesc;
        
        void Init();
        void Render(float dt, DynamicArray<SceneView*>& views);
    };
    
    class RENDER_API RendererProvider
    {
    public:
//        VIRTUAL GALDevicePtr GetDevice() const = 0;
        VIRTUAL bool Initialize(int32 inResX, int32 inResY, Viewport* inViewport) = 0;
        VIRTUAL void Resize(const uint32 inWidth, const uint32 inHeight) = 0;
        VIRTUAL void RenderScene(float dt, DynamicArray<SceneView*>& views) = 0;
        VIRTUAL void Shutdown() = 0;
    };
    
    class RENDER_API Renderer : public RendererProvider
    {
        friend struct SimpleTest;
    private:
//        GALDevicePtr device;
        GfxDevice* device;
        Viewport* viewport;
		int32 resX;
		int32 resY;
        SimpleTest* simpleTest;
        
        ConstantBufferPtr perObjectCB;
        ConstantBufferPtr frameCB;
        
        DynamicArray<SceneView> views; // This should be moved outside of the renderer
        
        void PlatformSpecificInitialization();
        
    public:
        Renderer();
        ~Renderer();
        
//        FORCEINLINE GALDevicePtr GetDevice() const OVERRIDE FINAL { return device; }
        
        bool Initialize(int32 inResX, int32 inResY, Viewport* inViewport) OVERRIDE FINAL;
        void Resize(const uint32 inWidth, const uint32 inHeight) OVERRIDE FINAL;
        void RenderScene(float dt, DynamicArray<SceneView*>& views) OVERRIDE FINAL;
        void Shutdown() OVERRIDE FINAL;
        
        FORCEINLINE void SetFrameCB(ConstantBufferPtr cb) { frameCB = cb; }
        
        FORCEINLINE ConstantBufferPtr GetFrameCb() const { return frameCB; }
    };
    
    
    ///////////////////////////////////////////////////////////
    ////////// Null Renderer
    ///////////////////////////////////////////////////////////
    class RENDER_API RendererNull : public RendererProvider
    {
    public:
        RendererNull() {}
        ~RendererNull() {}
        
//        FORCEINLINE GALDevicePtr GetDevice() const OVERRIDE FINAL { return nullptr; }
        
        bool Initialize(int32 inResX, int32 inResY, Viewport* inViewport) OVERRIDE FINAL { return true; }
        void Resize(const uint32 inWidth, const uint32 inHeight) OVERRIDE FINAL {}
        void RenderScene(float dt, DynamicArray<SceneView*>& views) OVERRIDE FINAL {}
        void Shutdown() OVERRIDE FINAL {}
    };
}


