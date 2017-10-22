#include "PlayerCamera.hpp"

#include "RyujinExtensions/Controller/PlayerController.hpp"
#include "RyujinRenderer/Renderer/SceneView.hpp"

namespace Ryujin
{
    PlayerCamera::PlayerCamera()
    : controller(nullptr),
    view(nullptr)
    {
        
    }
    
    PlayerCamera::~PlayerCamera()
    {
        delete controller;
        delete view;
    }
    
    bool PlayerCamera::Initialize(const PlayerCameraDescriptor& desc)
    {
        controller = RYUJIN_NEW PlayerController();
        if (controller->Initialize(0) == false)
            return false;
        
        controller->OnActionPerformed.AddCallback(&PlayerCamera::HandleActionPerformed, this);
        controller->OnRotationActionPerformed.AddCallback(&PlayerCamera::HandleRotationActionPerformed, this);
        
        view = RYUJIN_NEW SceneView(desc.viewPosition, Normalize(desc.lookAt - desc.viewPosition), desc.up,
                                    Vector2(desc.nearPlane, desc.farPlane), desc.fov, SceneView::ProjectionType::PT_Perspective);
        
        view->SetPerspective(desc.fov, desc.aspectRatio, desc.nearPlane, desc.farPlane);
        
        view->Update();
        
        acceleration = desc.acceleration;
        horizRotationSpeed = desc.horizRotationSpeed;
        vertRotationSpeed = desc.vertRotationSpeed;
        
        return true;
    }
    
    void PlayerCamera::Update(float dt)
    {
        controller->Update(dt);
    }
    
    void PlayerCamera::HandleRotationActionPerformed(float dt, float yawDelta, float pitchDelta, float rollDelta)
    {
        eulerAngles.x += MathUtils::DegreesToRadians(pitchDelta * dt * horizRotationSpeed);
        eulerAngles.y -= MathUtils::DegreesToRadians(yawDelta * dt * vertRotationSpeed);
        eulerAngles.z += MathUtils::DegreesToRadians(rollDelta * dt);
        if (eulerAngles.x >= MathUtils::PiOverTwo) eulerAngles.x = MathUtils::PiOverTwo;
        else if (eulerAngles.x <= -MathUtils::PiOverTwo) eulerAngles.x = -MathUtils::PiOverTwo;
        if (eulerAngles.y >= MathUtils::TwoPi) eulerAngles.y -= MathUtils::TwoPi;
        
        view->Rotate(eulerAngles);
        view->Update();
    }
    
    void PlayerCamera::HandleActionPerformed(ControllerAction action, float dt)
    {
        switch (action)
        {
            case ControllerAction::CA_Forward:
            {
                Vector3 p = view->GetWorldPosition();
                Vector3 newPos = p + view->GetLocalForwardDirection()*dt + acceleration * dt * dt;
                view->SetPosition(newPos);
//                view->SetPosition(p + view->GetLocalForwardDirection() * dt);
                view->Update();
            }
                break;
                
            case ControllerAction::CA_Back:
            {
                Vector3 p = view->GetWorldPosition();
                Vector3 newPos = p - view->GetLocalForwardDirection()*dt + acceleration * dt * dt;
                view->SetPosition(newPos);
//                view->SetPosition(p - view->GetLocalForwardDirection() * dt);
                view->Update();
            }
                break;
                
            case ControllerAction::CA_Right:
            {
                Vector3 p = view->GetWorldPosition();
                Vector3 newPos = p + view->GetLocalRightDirection()*dt + acceleration * dt * dt;
                view->SetPosition(newPos);
//                view->SetPosition(p + view->GetLocalRightDirection() * dt);
                view->Update();
            }
                break;
                
            case ControllerAction::CA_Left:
            {
                Vector3 p = view->GetWorldPosition();
                Vector3 newPos = p - view->GetLocalRightDirection()*dt + acceleration * dt * dt;
                view->SetPosition(newPos);
//                view->SetPosition(p - view->GetLocalRightDirection() * dt);
                view->Update();
            }
                break;
                
            case ControllerAction::CA_Up:
            {
                Vector3 p = view->GetWorldPosition();
                Vector3 newPos = p + view->GetLocalUpDirection()*dt + acceleration * dt * dt;
                view->SetPosition(newPos);
//                view->SetPosition(p + view->GetLocalUpDirection() * dt);
                view->Update();
            }
                break;
                
            case ControllerAction::CA_Down:
            {
                Vector3 p = view->GetWorldPosition();
                Vector3 newPos = p - view->GetLocalUpDirection()*dt + acceleration * dt * dt;
                view->SetPosition(newPos);
//                view->SetPosition(p - view->GetLocalUpDirection() * dt);
                view->Update();
            }
                break;
        }
    }
}
