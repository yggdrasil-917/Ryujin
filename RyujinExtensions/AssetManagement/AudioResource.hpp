#pragma once



#include "ResourceFactory.hpp"

#include "../Audio/OpenALDevice.hpp"


namespace Ryujin
{
    struct AudioAssetDesc
    {
        const char* filepath;
    };
    
    class AudioResource : public ResourceBase
    {
        DECLARE_RESOURCE_FACTORY_REGISTRANT(AudioResource);
    private:
        SharedPtrConcurrent<SoundClip> soundClip;
        
    public:
        AudioResource(const char* name, void* resourceDesc);
    };
}
