#pragma once

#include "RyujinExtensions/Controller/ControllerBase.hpp"
#include "RyujinCore/Math/Math.hpp"

namespace Ryujin
{
    struct PlayerCameraDescriptor
    {
        Vector3 viewPosition;
        Vector3 lookAt;
        Vector3 up;
        float fov;
        float aspectRatio;
        float nearPlane;
        float farPlane;
        Vector3 acceleration;
        float horizRotationSpeed;
        float vertRotationSpeed;
    };
    
    class PlayerCamera
    {
    private:
        class PlayerController* controller;
        class SceneView* view;
        float horizRotationSpeed;
        float vertRotationSpeed;
        Vector3 eulerAngles;
        Vector3 acceleration;
        
        void HandleActionPerformed(ControllerAction action, float dt);
        void HandleRotationActionPerformed(float dt, float yawDelta, float pitchDelta, float rollDelta);
        
    public:
        PlayerCamera();
        ~PlayerCamera();
        
        bool Initialize(const PlayerCameraDescriptor& desc);
        void Update(float dt);
        
        FORCEINLINE PlayerController* GetController() const { return controller; }
        FORCEINLINE SceneView* GetView() const { return view; }
    };
}
