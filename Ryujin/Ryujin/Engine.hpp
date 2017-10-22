
#ifndef ENGINE_HPP
#define ENGINE_HPP


#include "RyujinCore/CoreUtils/Delegate.hpp"
#include "RyujinCore/Threading/Thread.hpp"
#include "RyujinCore/Window/Viewport.hpp"
#include "RyujinCore/OS/Application.hpp"
#include "RyujinCore/Threading/JobSystem.hpp"
#include "RyujinCore/CoreUtils/ServiceLocator.hpp"
#include "RyujinCore/Input/Input.hpp"

#include "RyujinCore/Profiler/Profiler.hpp"
#include "RyujinCore/Profiler/PixProfiler.hpp"
#include "RyujinCore/Profiler/ChromeProfiler.hpp"

#include "RyujinRenderer/Renderer/Renderer.hpp"


#include "../SceneManagement/World.hpp"
#include "../IO/IO.hpp"
#include "../UI/ImGuiManager.hpp"

#include "RyujinExtensions/Utils/SymbolLocator.hpp"
#include "RyujinExtensions/Audio/OpenALDevice.hpp"
#include "RyujinExtensions/AssetManagement/AssetRegistry.hpp"
#include "PlayerCamera.hpp"


namespace Ryujin
{
    static Delegate<> OnToggleWireframeDelegate;
    static Delegate<> OnEscapePressedDelegate;
    static Delegate<int32, int32> OnWindowResizeDelegate;
    static Delegate<bool> OnWindowFocusChangedDelegate;
    
    extern bool bEngineIsRunning;
	extern bool bEngineIsRequestingExit;
    
    
    
    class Engine
    {
    private:
        Viewport viewport;
        Window* splashScreen;
        Renderer renderer;
        World world;
        
        IoManager io;
        JobSystem jobSystem;
        ImGuiManager imGui;
        AudioProvider* audio;
		Profiler* profiler;
        
        AssetRegistry alwaysLoadedAssets;
        
        Timer timeSinceStartup;
        Timer gameLoopTimer;
        
        SharedPtrConcurrent<SoundClip> soundClip;
        SharedPtrConcurrent<SoundSource> soundSource;
        
        UniquePtr<HIDManager> hidManager;
        
        TimeResult deltaFrameTime;
		uint32 frameNumber;
        DynamicArray<String> commandLineArgs;
        
        DynamicArray<SharedPtr<PlayerCamera>> players;
        PlayerController* mainController;
        DynamicArray<SceneView*> playerViews;
        
        static String errorMessage;
        bool bEngineHasFocus = false;
        bool bAllowMouseWarp = true;
        
        void RegisterDelegates();
        void UnregisterDelegates();
        
        void ShowSplash();
        void HideSplash();
        
        bool PreInit();
        bool PostInit();
        
    public:
        Engine();
        ~Engine();
        
        bool Start();
        
        void Shutdown();
        
        void EngineLoop();
        
        bool ProcessInput(float dt);
        
        void Run(int32 argc, char** argv);
        
        static void SetErrorMessage(const char* err) { errorMessage = err; }
        static const String& GetErrorMessage() { return errorMessage; }
        
        void HandleOnToggleWireframe();
        void HandleOnEscapePressed();
        void HandleOnWindowResize(int32 newWidth, int32 newHeight);
        void HandleOnWindowFocusChanged(bool bHasFocus);
    };
}


#endif
