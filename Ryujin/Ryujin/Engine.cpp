

#include "Engine.hpp"

#include "RyujinCore/Input/Events.hpp"
#include "RyujinCore/OS/Process.hpp"

#if !PLATFORM_LINUX
#include "../Lua/LuaVM.hpp"
#endif

#if PLATFORM_WINDOWS
#include "RyujinCore/Window/WindowWindows.hpp"
#endif

#include "RyujinExtensions/Controller/PlayerController.hpp"


namespace Ryujin
{
    bool bEngineIsRunning = true;
	bool bEngineIsRequestingExit = false;
    
    String Engine::errorMessage = "";
    
    static void CompileOutOfDateShaders()
    {
        SCOPED_PROFILE_EVENT(CompileOutOfDateShaders, PC_Engine, 255, 0, 0);
        Debugf(Info, "Checking out of date shaders");
        
        const char* shaderDirectory = Directory::GetCurrentDirectoryPath() + "/../../../RyujinRenderer/Shaders/";
        const char* builtShadersDirectory = Directory::GetCurrentDirectoryPath() + "/../ShadersIR/";
        
        DynamicArray<String> shaders;
        if (Directory::GetFiles(shaderDirectory, shaders))
        {
            DynamicArray<String> shadersToCompile;
            for (String& shader : shaders)
            {
                String irShader = shader;
                irShader.Replace(irShader.Length() - 4, StrLen(SHADER_EXT), SHADER_EXT);
                if (File::IsFileNewerThan(shaderDirectory + shader, builtShadersDirectory + irShader))
                {
                    shadersToCompile.Add(shader);
                }
            }
            
            if (shadersToCompile.GetSize() > 0)
            {
                Debugf(Info, "Compiling %d shaders", shadersToCompile.GetSize());
#if PLATFORM_WINDOWS
#define EXE_EXT ".exe"
#else
#define EXE_EXT ""
#endif
                Process shaderCompiler;
                String arguments = "-shaders ";
                for (String& str : shadersToCompile)
                {
                    arguments += str + " ";
                }
                shaderCompiler.RunProcess(*String::Printf("%s/RyujinShaderCompiler%s", *Directory::GetCurrentDirectoryPath(), EXE_EXT),
                                          *arguments,
                                          true);
                int32 code = shaderCompiler.GetExitCode();
                if (code == 0)
                {
                     String dst = AppInfo::GetResourcesDir();
                     dst += "Shaders/";
                     const char* shaderLibDirectory = Directory::GetCurrentDirectoryPath() + "/../Shaders/";
                     Directory::CopyAllFiles(shaderLibDirectory, dst);
                }
            }
        }
    }
    
    Engine::Engine()
    :	deltaFrameTime(0.0f),
		frameNumber(1),
    soundClip(SharedPtrConcurrent<SoundClip>(nullptr)),
    soundSource(SharedPtrConcurrent<SoundSource>(nullptr))
    {
        
    }
    
    
    Engine::~Engine()
    {
        
    }
    
    
    bool Engine::PreInit()
	{
		SCOPED_PROFILE_EVENT(PreInit, PC_Engine, 255, 0, 0);
        
        String wd = AppInfo::GetApplicationDir();
#if PLATFORM_MAC
        wd += "../";
#endif
        Debugf(Info, "Setting wd to %s", *wd);
        chdir(*wd);

		// Delete old logs
		String fileLogDir = String::Printf("%sLogs/", AppInfo::GetAppDataDir());
		DynamicArray<String> outFiles;
		if (Directory::GetFiles(*fileLogDir, outFiles))
		{
			for (auto& file : outFiles)
			{
				String filepath = fileLogDir + file;
				if (File::IsFileOlderThanNDays(*filepath, 4))
				{
					Debugf(Info, "Deleting log file %s", *file);
					File::Delete(*filepath);
				}
			}
		}

        if (!commandLineArgs.Contains("-nosplash"))
            ShowSplash();
        
        RegisterDelegates();
        
        CompileOutOfDateShaders();
        
        return true;
    }
    
    
    bool Engine::PostInit()
	{
		SCOPED_PROFILE_EVENT(PostInit, PC_Engine, 255, 0, 0);

        if (!commandLineArgs.Contains("-nosplash"))
            HideSplash();

		viewport.GetWindow()->SetWindowVisibility(true);
        
        imGui.Init();
        
        PlayerCamera* player = RYUJIN_NEW PlayerCamera();
        PlayerCameraDescriptor desc;
        desc.viewPosition = Vector3(0,0,5);
        desc.lookAt = Vector3();
        desc.up = Vector3(0,1,0);
        desc.fov = 60.0f;
        desc.nearPlane = 0.1;
        desc.farPlane = 0.0f;
        desc.acceleration = Vector3(0.1f);
        desc.horizRotationSpeed = 2.0f;
        desc.vertRotationSpeed = 2.0f;
        uint32 w = 0;
        uint32 h = 0;
        viewport.GetClientDimensions(w, h);
        desc.aspectRatio = w / (float)h;
        if (player->Initialize(desc))
        {
            players.Add(SharedPtr<PlayerCamera>(player));
            if (!mainController)
            {
                mainController = player->GetController();
            }
            playerViews.Add(player->GetView());
        }
        
        
        return true;
    }

#if PLATFORM_WINDOWS
#include "../resource.h"

	void Engine::ShowSplash()
	{
		WindowDescriptor splash;
		splash.width = 400;
		splash.height = 600;
		splash.bIsSplashScreen = true;

		HBITMAP splashBitmap = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(RYUJIN_SPLASH));
		BITMAP bmp;
		if (GetObject(splashBitmap, sizeof(BITMAP), (LPSTR)&bmp))
		{
			splash.width = bmp.bmWidth;
			splash.height = bmp.bmHeight;
			DeleteObject(splashBitmap);
		}

		splashScreen = RYUJIN_NEW WindowWindows();
		splashScreen->BuildWindow(splash);

		PumpEvents_Windows();
	}

	void Engine::HideSplash()
	{
		if (splashScreen)
		{
			splashScreen->DestroyWindow();
			SAFE_DELETE(splashScreen);
		}
	}
#elif PLATFORM_LINUX
    void Engine::ShowSplash()
    {

    }

    void Engine::HideSplash()
    {

    }

#endif
    
    
    bool Engine::Start()
	{
		if (commandLineArgs.Contains("-chromeProfile"))
		{
			profiler = RYUJIN_NEW ChromeProfiler();
			profiler->Init();
		}
		else if (commandLineArgs.Contains("-pixProfile"))
		{
			profiler = RYUJIN_NEW PixProfiler();
			profiler->Init();
		}
		SCOPED_PROFILE_EVENT(EngineStartup, PC_Engine, 255, 0, 0);

		if (!PreInit())
            return false;
        
        
#if !PLATFORM_LINUX
        {
            LuaVM lua;
            lua.DoFile("options.lua");
            int32 w = lua.GetValue<int32>("viewport.width", 0);
            int32 h = lua.GetValue<int32>("viewport.height", 0);
            
            DynamicArray<int32> intArr;
            lua.GetArray<int32>("misc.intArray", intArr);
        }
#endif
        
        timeSinceStartup.Start();
        
        
        io.Initialize();
        String cubeAsset = String::Printf("%scube.asset", AppInfo::GetCookedContentDir());
        
        io.AsyncLoad(*cubeAsset,
                     nullptr,
                     [](const IoResult& result) { Debugf(IO, "SUCCESS loading %s", *result.filename); },
                     [](const char* errorMsg) { Debugf(IO, "%s", errorMsg); });
        
//        io.AsyncCancel(id);
        
        jobSystem.Initialize(Threading::NumConcurrentThreadsSupported() - 1);
        
#if PLATFORM_MAC
        hidManager = UniquePtr<HIDManager>(RYUJIN_NEW HIDManagerDarwin());
#elif PLATFORM_WINDOWS
		hidManager = UniquePtr<HIDManager>(RYUJIN_NEW HIDManagerWindows());
#elif PLATFORM_LINUX
        hidManager = UniquePtr<HIDManager>(RYUJIN_NEW HIDManagerLinux());
#endif
        hidManager->Init();
        
        if (!commandLineArgs.Contains("-nosplash"))
            Threading::Sleep(1000);
        
        ConfigCache configCache;
        if (!configCache.LoadConfig("options.ini"))
        {
            SetErrorMessage("Failed to load options.ini");
            return false;
        }
        
		WindowDescriptor windowDesc = {};
        windowDesc.colorBits = 32;
        windowDesc.depthBits = 24;
        windowDesc.stencilBits = 8;
		//windowDesc.bShowMinimized = true; // commenting this in causes Vulkan surface caps' current extent to be zero
        configCache.GetInt("Viewport", "width", windowDesc.width, 640);
        configCache.GetInt("Viewport", "height", windowDesc.height, 320);
        configCache.GetBool("Viewport", "fullscreen", windowDesc.bIsFullscreen);
        configCache.GetString("Viewport", "title", windowDesc.windowTitleOrSplashScreenPath);
        if (!viewport.CreateWindow(windowDesc))
        {
            SetErrorMessage("Failed to create window");
            return false;
        }

        if (!renderer.Initialize(windowDesc.width, windowDesc.height, &viewport))
        {
            SetErrorMessage("Failed to initialize renderer");
            return false;
        }
        
#if WITH_OPENAL
        audio = RYUJIN_NEW OpenALDevice();
#else
        audio = nullptr;
#endif
        if (!audio->Initialize())
        {
            SetErrorMessage("Failed to initialize audio provider");
            return false;
        }
        
        soundClip = audio->LoadSoundClip("../../../Content/Audio/swvader03.wav");
        soundSource = AudioProvider::CreateSoundSource();
        soundSource->QueueSoundClip(soundClip);
        
        // Load map
        String startupMap;
        configCache.GetString("Misc", "startupMap", startupMap);
        if (startupMap.Length() == 0)
        {
            SetErrorMessage("No startup map specified");
            return false;
        }
        
//        world.LoadWorld(*startupMap);
        
        if (!PostInit())
            return false;
        
        if (bEngineHasFocus && bAllowMouseWarp)
        {
            viewport.SetMouseCursorVisible(false);
        }
        
        return true;
    }
    
    bool Engine::ProcessInput(float dt)
	{
		SCOPED_PROFILE_EVENT(ProcessInput, PC_Engine, 255, 0, 0);

        PumpEvents();
        
        RyujinEvent event;
        while (PollEvents(event))
        {
            switch (event.engineEvent)
            {
                case EngineEvent::Quit:
                    bEngineIsRunning = false;
                    return false;

				case EngineEvent::ExitOnError:
					bEngineIsRequestingExit = true;
					SetErrorMessage("Engine encountered an error. Check the logs for more information.");
					return false;
                    
                case EngineEvent::WindowEvent:
                {
                    switch (event.windowEventID)
                    {
                        case WindowEventID::Close:
                            bEngineIsRunning = false;
                            return false;
                            
                        case WindowEventID::FocusGained:
                            OnWindowFocusChangedDelegate(true);
                            break;
                            
                        case WindowEventID::FocusLost:
                            OnWindowFocusChangedDelegate(false);
                            break;
                            
                        case WindowEventID::Resize:
                            OnWindowResizeDelegate(event.Data1, event.Data2);
                            break;
                    }
                }
                    break;
                    
                case EngineEvent::KeyUp:
                    switch (event.Data1)
                    {
                        case RYUJIN_KEY_SPACE:
                            OnToggleWireframeDelegate();
                            break;
                            
                        case RYUJIN_KEY_ESCAPE:
                            OnEscapePressedDelegate();
                            break;
                            
                        case RYUJIN_KEY_u:
                            break;
                            
                        case RYUJIN_KEY_p:
                            if (soundSource->IsFinished())
                            {
                                soundSource->Update();
                                soundSource->Play();
                            }
                            break;
                    }
                    break;
            }
            
            PopEvent();
        }
        
        // Process keyboard input for the camera
//        int32 mouseX, mouseY;
//        uint32 mouseState = 0;
//        const uint8* keyStates = Keyboard::Instance()->GetKeyboardState();
//        Mouse::Instance()->GetMousePosition(mouseX, mouseY);
//        mouseState = Mouse::Instance()->GetCurrentButtonState();
        
        for (auto& player : players)
        {
            player->Update(dt);
        }
        
        if (bEngineHasFocus && bAllowMouseWarp && mainController->ShouldWarpMouse())
        {
            uint32 x = 0;
            uint32 y = 0;
            viewport.GetClientDimensions(x, y);
            viewport.WarpMouse(x / 2, y / 2);
        }
        
        return true;
    }
    
    
    void Engine::EngineLoop()
    {
        gameLoopTimer.Start();
        while (bEngineIsRunning)
		{
			SCOPED_PROFILE_EVENT(Frame, PC_Engine, 255, 0, 0);
            
            deltaFrameTime = gameLoopTimer.ElapsedSeconds(true);
            
            hidManager->UpdateStates(deltaFrameTime);
            ProcessInput(deltaFrameTime);

			if (bEngineIsRequestingExit)
			{
				break;
			}
            
            world.Update(deltaFrameTime);

            renderer.RenderScene(deltaFrameTime, playerViews);

			++frameNumber;
        }
    }
    
    void Engine::Shutdown()
    {
		SCOPED_PROFILE_EVENT(EngineShutdown, PC_Engine, 255, 0, 0);

        world.UnloadWorld();
        
        io.Shutdown();
        jobSystem.Shutdown();
        
        hidManager->Shutdown();
        
        imGui.Shutdown();
        renderer.Shutdown();
        soundClip.SafeRelease();
        soundSource.SafeRelease();
        audio->Shutdown();
        
		UnregisterDelegates();
    }
    
    void Engine::Run(int32 argc, char **argv)
    {
        for (int32 i = 0; i < argc; ++i)
        {
            commandLineArgs.Add(argv[i]);
        }
        
        if (!Start())
        {
            ErrorMsg(*GetErrorMessage());
            Shutdown();
            return;
        }
        
        EngineLoop();

		if (errorMessage.Length() > 0)
		{
			ErrorMsg(*GetErrorMessage());
            Errorf(Info, "Engine shutdown due to error: %s", *GetErrorMessage());
		}
        else
        {
            Debugf(Info, "User requested shutdown");
        }
        
        Shutdown();
        
        if (Profiler::IsProfiling())
        {
            Profiler::Instance()->Shutdown();
        }
    }
    
    
    void Engine::RegisterDelegates()
    {
        OnToggleWireframeDelegate.AddCallback(&Engine::HandleOnToggleWireframe, this);
        OnEscapePressedDelegate.AddCallback(&Engine::HandleOnEscapePressed, this);
        OnWindowResizeDelegate.AddCallback(&Engine::HandleOnWindowResize, this);
        OnWindowFocusChangedDelegate.AddCallback(&Engine::HandleOnWindowFocusChanged, this);
    }
    
    void Engine::UnregisterDelegates()
    {
        OnToggleWireframeDelegate.RemoveAllCallbacks();
        OnEscapePressedDelegate.RemoveAllCallbacks();
        OnWindowResizeDelegate.RemoveAllCallbacks();
        OnWindowFocusChangedDelegate.RemoveAllCallbacks();
    }
    
    void Engine::HandleOnToggleWireframe()
    {
    }
    
    void Engine::HandleOnEscapePressed()
    {
        bAllowMouseWarp = !bAllowMouseWarp;
        if (bAllowMouseWarp)
        {
            viewport.SetMouseCursorVisible(false);
        }
        else
        {
            viewport.SetMouseCursorVisible(true);
        }
    }
    
    void Engine::HandleOnWindowResize(int32 newWidth, int32 newHeight)
    {
		renderer.Resize(newWidth, newHeight);
    }
    
    void Engine::HandleOnWindowFocusChanged(bool bHasFocus)
    {
        bEngineHasFocus = bHasFocus;
        if (bHasFocus)
        {
            bAllowMouseWarp = true;
            viewport.SetMouseCursorVisible(false);
        }
        else
        {
            bAllowMouseWarp = false;
            viewport.SetMouseCursorVisible(true);
        }
    }
}
