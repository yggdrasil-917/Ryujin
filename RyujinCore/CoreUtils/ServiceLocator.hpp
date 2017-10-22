
#ifndef SERVICE_LOCATOR_HPP
#define SERVICE_LOCATOR_HPP


#include "../OS/Types.hpp"


namespace Ryujin
{
    class RendererProvider;
    class AudioProvider;
    
    class CORE_API ServiceLocator
    {
    private:
        static RendererProvider* renderer;
        static AudioProvider* audio;
        
    public:
        static RendererProvider* GetRenderer()
        {
            return renderer;
        }
        
        static void Provide(RendererProvider* inRenderer)
        {
            renderer = inRenderer;
        }
        
        static AudioProvider* GetAudioDevice()
        {
            return audio;
        }
        
        static void Provide(AudioProvider* inAudio)
        {
            audio = inAudio;
        }
    };
    
}


#endif
