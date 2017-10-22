#pragma once

#include "AudioDevice.hpp"


#if WITH_OPENAL

#if PLATFORM_MAC
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#else
#include <al.h>
#include <alc.h>
#endif

namespace Ryujin
{
    class EXTENSIONS_API OpenALSoundBuffer
    {
    private:
        ALuint bufferIds[2];
        ALuint internalFormat;
        
    public:
        OpenALSoundBuffer();
        ~OpenALSoundBuffer();
        
        void Init(const WaveInstance& wave);
        
        friend class OpenALSoundSource;
    };
    
    class EXTENSIONS_API OpenALSoundSource : public SoundSource
    {
    private:
        ALuint sourceId;
//        OpenALSoundBuffer* buffer;
        
        void HandleQueuedBuffer();
        
    public:
        OpenALSoundSource();
        ~OpenALSoundSource();
        
        void Init() OVERRIDE FINAL;
        
        void QueueSoundClip(const SharedPtrConcurrent<SoundClip>& inSoundClip) OVERRIDE FINAL;
        void UnqueueSoundClip() OVERRIDE FINAL;
        
        void Update() OVERRIDE FINAL;
        void Play() OVERRIDE FINAL;
        void Stop() OVERRIDE FINAL;
        void Pause() OVERRIDE FINAL;
        
        bool IsFinished() OVERRIDE FINAL;
        bool IsSourceFinished() OVERRIDE FINAL;
    };
    
    class EXTENSIONS_API OpenALDevice : public AudioProvider
    {
    private:
        ALCdevice* device;
        ALCcontext* context;
        
        DynamicArray<OpenALSoundBuffer*> buffers;
        
    public:
        OpenALDevice();
        ~OpenALDevice();
        
        bool Initialize() OVERRIDE FINAL;
        void Update() OVERRIDE FINAL;
        bool Shutdown() OVERRIDE FINAL;
    };
    
    FORCEINLINE bool CheckALErrors()
    {
        ALCenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            Debugf(Audio, "OpenAL error: %d", error);
            return true;
        }
        
        return false;
    }
}

#endif
