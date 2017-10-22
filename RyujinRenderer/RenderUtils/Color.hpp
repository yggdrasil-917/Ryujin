#pragma once


#include "RyujinCore/OS/Types.hpp"


namespace Ryujin
{
    class Color
    {
    private:
        uint8 r;
        uint8 g;
        uint8 b;
        uint8 a;
        
    public:
        Color();
        Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a = 255);
        
        // From: http://en.wikipedia.org/wiki/HSL_and_HSV#Converting_to_RGB
        void SetHSL(float hue, float saturation, float lightness);
        // From: http://en.wikipedia.org/wiki/HSL_and_HSV#General_approach
        void GetHSL(float& hue, float& saturation, float& lightness);
        
        FORCEINLINE void SetColorFloat(float _r, float _g, float _b, float _a = 1.0f)
        {
            r = uint8(255.0f * _r);
            g = uint8(255.0f * _g);
            b = uint8(255.0f * _b);
            a = uint8(255.0f * _a);
        }
        
        FORCEINLINE void SetColor(uint8 _r, uint8 _g, uint8 _b, uint8 _a)
        {
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }
        
        FORCEINLINE uint8 GetRed() const { return r; }
        FORCEINLINE uint8 GetGreen() const { return g; }
        FORCEINLINE uint8 GetBlue() const { return b; }
        FORCEINLINE uint8 GetAlpha() const { return a; }
    };
}
