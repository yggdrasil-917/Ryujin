

#include "SceneView.hpp"

#include "RyujinCore/Math/Transform.hpp"

namespace Ryujin
{
    SceneView::SceneView()
    {
        
    }
    
    SceneView::SceneView(const Vector3& inPosition, const Vector3& inForwardDirection, const Vector3& inUp, const Vector2& inClipDistance, const real inFov, const ProjectionType inProjectionType)
    :
    projection(Matrix44()),
    position(inPosition),
    forward(inForwardDirection),
    up(inUp),
    zNear(inClipDistance.x),
    zFar(inClipDistance.y),
    fov(inFov),
    projectionType(inProjectionType),
    bIsDirty(true)
    {
        forward = Math::Normalize(forward);
        right = Math::Cross(forward, inUp);
        up = Math::Cross(right, forward);
        Update();
    }
    
    void SceneView::ConditionalUpdate()
    {
        if (!bIsDirty)
            return;
        
        bIsDirty = false;
        view = Math::LookAt(position, position + forward, up);
        
        viewProjection = projection * view;
        frustum.UpdateFrustum(viewProjection);
        
        invView = Math::Inverse(view);
        invProjection = Math::Inverse(projection);
        invViewProjection = Math::Inverse(viewProjection);
    }
    
    void SceneView::Update()
    {
        ConditionalUpdate();
    }
    
    void SceneView::Rotate(const Vector3& eulerAngles)
    {
        bIsDirty = true;
        Quaternion q(Math::EulerToQuaternion(eulerAngles));
        forward = Normalize(q * Vector3(0,0,-1));
        right = Normalize(q * Vector3(1,0,0));
        up = Normalize(q * Vector3(0,1,0));
    }
    
    void SceneView::SetPosition(const Vector3& inPosition)
    {
        bIsDirty = true;
        position = inPosition;
    }
    
    void SceneView::SetDirection(const Vector3& inDirection, const Vector3& inUp)
    {
        bIsDirty = true;
        forward = inDirection;
        
        right = Math::Cross(forward, inUp);
        up = Math::Cross(right, forward);
    }
    
    void SceneView::SetPerspective(const real fovy, const real aspect, const real nearZ, const real farZ)
    {
        bIsDirty = true;
        fov = fovy;
        zNear = nearZ;
        zFar = farZ;
        projectionType = ProjectionType::PT_Perspective;
        
        if (farZ == 0.0)
        {
            projection = Math::PerspectiveInfinite(MathUtils::DegreesToRadians(fovy), aspect, nearZ);
        }
        else
        {
            projection = Math::PerspectiveFov(MathUtils::DegreesToRadians(fovy), aspect, nearZ, farZ);
        }
    }
    
    void SceneView::SetOrthographic(const real left, const real right, const real top, const real bottom, const real nearZ, const real farZ)
    {
        bIsDirty = true;
        zNear = nearZ;
        zFar = farZ;
        projectionType = ProjectionType::PT_Orthographic;
        
        projection = Math::Orthographic(left, right, bottom, top, nearZ, farZ);
    }
    
    void Pup(Pupper* p, SceneView& obj, const VarInfo& inVarInfo)
    {
        Pup(p, obj.view, inVarInfo.name + ".view");
        Pup(p, obj.projection, inVarInfo.name + ".projection");
        Pup(p, obj.viewProjection, inVarInfo.name + ".viewProjection");
        Pup(p, obj.position, inVarInfo.name + ".position");
        Pup(p, obj.forward, inVarInfo.name + ".forward");
        Pup(p, obj.up, inVarInfo.name + ".up");
        Pup(p, obj.right, inVarInfo.name + ".right");
        Pup(p, obj.zNear, inVarInfo.name + ".zNear");
        Pup(p, obj.zFar, inVarInfo.name + ".zFar");
        Pup(p, obj.fov, inVarInfo.name + ".fov");
        Pup(p, obj.frustum, inVarInfo.name + ".frustum");
        for (int32 i = 0; i < 8; ++i)
        {
            Pup(p, obj.corners[i], String::Printf("%s.corner_%d", *inVarInfo.name, i));
        }
        uint8 projectionType = (uint8)obj.projectionType;
        Pup(p, projectionType, inVarInfo.name + ".projectionType");
        obj.projectionType = (SceneView::ProjectionType)projectionType;
        Pup(p, obj.bIsDirty, inVarInfo.name + ".isDirty");
    }
}
