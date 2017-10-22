#pragma once

#include "../Utils/ExtensionsBase.hpp"
#include "RyujinCore/CoreUtils/ServiceLocator.hpp"
#include "RyujinCore/CoreUtils/SmartPointer.hpp"
#include "RyujinCore/Containers/DynamicArray.hpp"
#include "RyujinCore/Math/Math.hpp"

#include "AudioUtils.hpp"


#if PLATFORM_MAC || PLATFORM_WINDOWS

#define WITH_OPENAL 1

#else

#define WITH_OPENAL 0

#endif


namespace Ryujin
{
    struct AudioDeviceConfig
    {
        int8 maxNumChannels;
    };
    
    template <class TSoundBuffer>
    class TSoundClip
    {
    protected:
        // stores the audio data
        int32 bufferSize;
        int16 numChannels;
        int16 sampleRate;
        
        TSoundBuffer soundBuffer;
        WaveInstance* waveInstance;
        
    public:
        TSoundClip()
        : bufferSize(0),
          numChannels(1),
          sampleRate(0)
        {
            
        }
        ~TSoundClip()
        {
            SAFE_DELETE(waveInstance);
        }
        
        void Init(WaveInstance* wave)
        {
            if (!wave)
            {
                return;
            }
            waveInstance = wave;
            numChannels = wave->GetNumChannels();
            sampleRate = wave->GetNumSamplesPerSec();
            bufferSize = wave->GetWaveDataLength();
            soundBuffer.Init(*wave);
        }
        
        
        FORCEINLINE int32 GetBufferSize() const { return bufferSize; }
        FORCEINLINE int16 GetNumChannels() const { return numChannels; }
        FORCEINLINE int16 GetSampleRate() const { return sampleRate; }
        FORCEINLINE const TSoundBuffer& GetSoundBuffer() const { return soundBuffer; }
        FORCEINLINE WaveInstance* GetWaveInstance() const { return waveInstance; }
    };
    
#if WITH_OPENAL
    class OpenALSoundBuffer;
    using SoundClip = TSoundClip<OpenALSoundBuffer>;
#endif
    
    class EXTENSIONS_API SoundSource
    {
    protected:
        bool bIsPaused;
        bool bIsPlaying;
        
        SharedPtrConcurrent<SoundClip> soundClip;
        
    public:
        SoundSource()
        : bIsPaused(false),
          bIsPlaying(false),
          soundClip(SharedPtrConcurrent<SoundClip>(nullptr))
        {
            
        }
        
        VIRTUAL ~SoundSource() {}
        
        VIRTUAL void Init() = 0;
        
        VIRTUAL void QueueSoundClip(const SharedPtrConcurrent<SoundClip>& inSoundClip) = 0;
        VIRTUAL void UnqueueSoundClip() = 0;
        
        VIRTUAL void Update() = 0;
        VIRTUAL void Play() = 0;
        VIRTUAL void Stop() = 0;
        VIRTUAL void Pause() = 0;
        
        VIRTUAL bool IsFinished() = 0;
        VIRTUAL bool IsSourceFinished() = 0;
    };
    
    // TODO: flesh this class out in the future
    class EXTENSIONS_API SoundListener
    {
    private:
        Vector3 position;
        Vector3 up;
        Vector3 velocity;
        
    public:
        SoundListener();
        
        FORCEINLINE Vector3 GetPosition() const { return position; }
        FORCEINLINE Vector3 GetUp() const { return up; }
        FORCEINLINE Vector3 GetVelocity() const { return velocity; }
    };
    
    class EXTENSIONS_API AudioProvider
    {
    protected:
        AudioDeviceConfig deviceConfig;
        
    public:
        VIRTUAL bool Initialize() = 0;
        VIRTUAL void Update() = 0;
        VIRTUAL bool Shutdown() = 0;
        
        static SharedPtrConcurrent<SoundSource> CreateSoundSource();
        static SharedPtrConcurrent<SoundClip> LoadSoundClip(const char* filename);
    };
}
