#include "AudioResource.hpp"


namespace Ryujin
{
    AudioResource::AudioResource(const char* name, void* resourceDesc)
    : ResourceBase(name)
    {
        AudioAssetDesc* desc = (AudioAssetDesc*)resourceDesc;
        RYUJIN_ASSERT(desc, "wrong resource desc type detected");
        
        AudioProvider* audio = ServiceLocator::GetAudioDevice();
        soundClip = audio->LoadSoundClip(desc->filepath);
    }
    
    IMPLEMENT_RESOURCE_FACTORY_REGISTRANT(AudioResource);
}
