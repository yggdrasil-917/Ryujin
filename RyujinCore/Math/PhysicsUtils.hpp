#pragma once


#include "Math.hpp"



namespace Ryujin
{
    FORCEINLINE Vector3 ProjectilePositionAtTime(Vector3 initialVelocity, Vector3 initialPos, Vector3 gravity, float t)
    {
        return gravity * (0.5f * t * t) + initialVelocity * t + initialPos;
    }
    
    
}
