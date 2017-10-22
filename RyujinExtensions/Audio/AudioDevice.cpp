#include "AudioDevice.hpp"

#include "OpenALDevice.hpp"


namespace Ryujin
{
    SoundListener::SoundListener()
    : position(Vector3()),
      up(Vector3(0.0f, 1.0f, 0.0f)),
      velocity(Vector3())
    {
        
    }
    
    SharedPtrConcurrent<SoundSource> AudioProvider::CreateSoundSource()
    {
#if WITH_OPENAL
        SharedPtrConcurrent<SoundSource> source(RYUJIN_NEW OpenALSoundSource());
        source->Init();
        return source;
#endif
    }
    
    SharedPtrConcurrent<SoundClip> AudioProvider::LoadSoundClip(const char* filename)
    {
        if (filename)
        {
            WaveInstance* wave = RYUJIN_NEW WaveInstance();
            if (wave && wave->LoadFromFile(filename))
            {
                SoundClip* sound = RYUJIN_NEW SoundClip();
                sound->Init(wave);
                return SharedPtrConcurrent<SoundClip>(sound);
            }
        }
        
        return SharedPtrConcurrent<SoundClip>(nullptr);
    }
}
