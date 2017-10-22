#include "Color.hpp"
#include "RyujinCore/Math/MathUtils.hpp"

namespace Ryujin
{
    Color::Color()
    {
        SetColor(0, 0, 0, 255);
    }
    
    Color::Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a)
    {
        SetColor(_r, _g, _b, _a);
    }
    
    void Color::SetHSL(float hue, float saturation, float lightness)
    {
        float flHue6 = hue/60;
        float flChroma = (1 - MathUtils::Abs(2 * lightness - 1)) * saturation;
        float flX = flChroma * (1.0f - MathUtils::Abs(MathUtils::Mod(flHue6, 2.0f) - 1.0f));
        
        float flR1, flG1, flB1;
        if (flHue6 < 1)
        {
            flR1 = flChroma;
            flG1 = flX;
            flB1 = 0;
        }
        else if (flHue6 < 2)
        {
            flR1 = flX;
            flG1 = flChroma;
            flB1 = 0;
        }
        else if (flHue6 < 3)
        {
            flR1 = 0;
            flG1 = flChroma;
            flB1 = flX;
        }
        else if (flHue6 < 4)
        {
            flR1 = 0;
            flG1 = flX;
            flB1 = flChroma;
        }
        else if (flHue6 < 5)
        {
            flR1 = flX;
            flG1 = 0;
            flB1 = flChroma;
        }
        else
        {
            flR1 = flChroma;
            flG1 = 0;
            flB1 = flX;
        }
        
        float flM = lightness - flChroma/2;
        
        r = uint8(flR1 + flM);
        g = uint8(flG1 + flM);
        b = uint8(flB1 + flM);
    }
    
    void Color::GetHSL(float& hue, float& saturation, float& lightness)
    {
        float red = ((float)r)/255;
        float green = ((float)g)/255;
        float blue = ((float)b)/255;
        
        float M = MathUtils::Max(MathUtils::Max(r, g), b);
        float m = MathUtils::Min(MathUtils::Min(r, g), b);
        
        float flChroma = M - m;
        
        if (flChroma == 0)
            hue = 0;
        else if (red > green && red > blue)
            hue = MathUtils::Mod((green - blue)/flChroma, 6.0f) * 60;
        else if (green > red && green > blue)
            hue = ((blue - red)/flChroma + 2) * 60;
        else if (blue > red && blue > green)
            hue = ((red - green)/flChroma + 4) * 60;
        
        lightness = (M + m)/2;
        
        saturation = 0;
        if (flChroma > 0)
            saturation = flChroma / (1 - MathUtils::Abs(2*lightness - 1));
    }
}
