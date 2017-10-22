#include "OpenALDevice.hpp"


#if WITH_OPENAL

namespace Ryujin
{
    static ALuint GetInternalFormat(int16 numChannels, int8 numBitsPerSample)
    {
        ALuint InternalFormat = 0;
        
        switch(numChannels)
        {
            case 0:
            case 3:
            case 5:
                break;
            case 1:
                InternalFormat = numBitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
                break;
            case 2:
                InternalFormat = numBitsPerSample == 8 ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
                break;
            default:
                InternalFormat = numBitsPerSample == 8 ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
                break;
        }
        
        return(InternalFormat);
    }
    
    OpenALSoundBuffer::OpenALSoundBuffer()
    {
        bufferIds[0] = 0;
        internalFormat = 0;
    }
    
    OpenALSoundBuffer::~OpenALSoundBuffer()
    {
        if (bufferIds[0] != 0)
        {
            alDeleteBuffers(1, bufferIds);
            bufferIds[0] = 0;
        }
    }
    
    void OpenALSoundBuffer::Init(const WaveInstance& wave)
    {
        if (bufferIds[0] != 0)
        {
            return;
        }
        
        alGenBuffers(1, bufferIds);
        CheckALErrors();
        
        internalFormat = GetInternalFormat(wave.GetNumChannels(), wave.GetNumBitsPerSample());
        
        alBufferData(bufferIds[0], internalFormat, wave.GetWaveData(), wave.GetWaveDataLength(), wave.GetNumSamplesPerSec());
        CheckALErrors();
        
        ALfloat pos[3];
        ALfloat orientation[6];
        ALfloat vel[3];
        for (int32 i = 0; i < 3; ++i)
        {
            pos[i] = 0.0f;
            vel[i] = 0.0f;
        }
        orientation[0] = 0.0f; orientation[1] = 0.0f; orientation[2] = 1.0f;
        orientation[3] = 0.0f; orientation[4] = 1.0f; orientation[5] = 0.0f;
        
        alListenerfv(AL_POSITION, pos);
        alListenerfv(AL_ORIENTATION, &orientation[0]);
        alListenerfv(AL_VELOCITY, vel);
    }
    
    OpenALSoundSource::OpenALSoundSource()
    : SoundSource(),
      sourceId(0)
    {
        
    }
    
    OpenALSoundSource::~OpenALSoundSource()
    {
        UnqueueSoundClip();
        
        if (sourceId != 0)
        {
            alDeleteSources(1, &sourceId);
            sourceId = 0;
        }
    }
    
    void OpenALSoundSource::Init()
    {
        alGenSources(1, &sourceId);
        
        // Set the default volume
        alSourcef(sourceId, AL_GAIN, 1);
        
        // Set the default position of the sound
        alSource3f(sourceId, AL_POSITION, 0, 0, 0);
    }
    
    void OpenALSoundSource::QueueSoundClip(const SharedPtrConcurrent<SoundClip>& inSoundClip)
    {
        soundClip = inSoundClip;
        
        alSourceQueueBuffers(sourceId, 1, soundClip->GetSoundBuffer().bufferIds);
    }
    
    void OpenALSoundSource::UnqueueSoundClip()
    {
        soundClip = SharedPtrConcurrent<SoundClip>(nullptr);
        ALuint dequeuedBuffer;
        alSourceUnqueueBuffers(sourceId, 1, &dequeuedBuffer);
    }
    
    void OpenALSoundSource::Update()
    {
        float volume = 1.0f;
        alSourcef(sourceId, AL_GAIN, volume);
    }
    
    void OpenALSoundSource::Play()
    {
        alSourcePlay(sourceId);
        bIsPaused = false;
        bIsPlaying = true;
    }
    
    void OpenALSoundSource::Stop()
    {
        alSourceStop(sourceId);
        // This clears out any pending buffers that may or may not be queued or played
        alSourcei(sourceId, AL_BUFFER, 0);
        bIsPaused = false;
        bIsPlaying = false;
    }
    
    void OpenALSoundSource::Pause()
    {
        alSourcePause(sourceId);
        bIsPaused = true;
    }
    
    bool OpenALSoundSource::IsSourceFinished()
    {
        ALint state = AL_STOPPED;
        
        // Check the source for data to continue playing
        alGetSourcei(sourceId, AL_SOURCE_STATE, &state);
        if(state == AL_PLAYING || state == AL_PAUSED)
        {
            return false;
        }
        
        return true;
    }
    
    void OpenALSoundSource::HandleQueuedBuffer()
    {
        ALuint dequeuedBuffer;
        
        // Unqueue the processed buffers
        alSourceUnqueueBuffers(sourceId, 1, &dequeuedBuffer);
        
        WaveInstance* wave = soundClip->GetWaveInstance();
        alBufferData(soundClip->GetSoundBuffer().bufferIds[0], soundClip->GetSoundBuffer().internalFormat,
                     wave->GetWaveData(), wave->GetWaveDataLength(), wave->GetNumSamplesPerSec());
        
        // Queue the same packet again for looping
        alSourceQueueBuffers(sourceId, 1, soundClip->GetSoundBuffer().bufferIds);
    }
    
    bool OpenALSoundSource::IsFinished()
    {
        if (IsSourceFinished())
        {
            return true;
        }
        else
        {
            // Check to see if any complete buffers have been processed
            ALint buffersProcessed;
            alGetSourcei(sourceId, AL_BUFFERS_PROCESSED, &buffersProcessed);
            Debugf(Audio, "Buffers processed %d", buffersProcessed);
            
            switch (buffersProcessed)
            {
                case 0:
                    // Nothing needs to be updated
                    break;
                    
                case 1:
                    // A buffer needs to be repopulated
                    HandleQueuedBuffer();
                    break;
                    
                case 2:
                    HandleQueuedBuffer();
                    HandleQueuedBuffer();
                    alSourcePlay(sourceId);
                    break;
            }
            
            return false;
        }
    }
    
    static void ListAudioDevices(const ALCchar* devices)
    {
        const ALCchar *device = devices, *next = devices + 1;
        PtrSize len = 0;
        
        Debugf(Audio, "Audio devices list:");
        while (device && *device != '\0' && next && *next != '\0')
        {
            Debugf(Audio, "\t%s", device);
            len = strlen(device);
            device += (len + 1);
            next += (len + 2);
        }
    }
    
    OpenALDevice::OpenALDevice()
    {
        deviceConfig.maxNumChannels = 32;
    }
    
    OpenALDevice::~OpenALDevice()
    {
        RYUJIN_ASSERT(buffers.GetSize() == 0, "AL device has not been properly shutdown");
    }
    
    bool OpenALDevice::Initialize()
    {
        ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
        
        if (enumeration == AL_FALSE)
        {
            // enumeration not supported
            device = alcOpenDevice(NULL);
        }
        else
        {
            // enumeration supported
            ListAudioDevices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
            device = alcOpenDevice(NULL);
        }
        
        CheckALErrors();
        
        if (!device)
        {
            return false;
        }
        
        int32 caps[] =
        {
            ALC_FREQUENCY, 44100,
            ALC_STEREO_SOURCES, 4,
            0, 0 };
        context = alcCreateContext(device, caps);
        if (!alcMakeContextCurrent(context))
        {
            CheckALErrors();
            return false;
        }
        
        return true;
    }
    
    void OpenALDevice::Update()
    {
        
    }
    
    bool OpenALDevice::Shutdown()
    {
        for (uint32 i = 0; i < buffers.GetSize(); ++i)
        {
            SAFE_DELETE(buffers[i]);
        }
        buffers.RemoveAll();
        
        alcDestroyContext(context);
        alcCloseDevice(device);
        
        context = nullptr;
        device = nullptr;
        
        return true;
    }
}

#endif
