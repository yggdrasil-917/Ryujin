
#ifndef SCENE_VIEW_HPP
#define SCENE_VIEW_HPP


#include "../Renderer/RendererBase.hpp"

#include "RyujinCore/Math/Frustum.hpp"


namespace Ryujin
{
    class RENDER_API SceneView
    {
    public:
        enum class ProjectionType : uint8
        {
            PT_Perspective,
            PT_Orthographic
        };
        
    private:
        Matrix44 view;
        Matrix44 invView;
        Matrix44 projection;
        Matrix44 invProjection;
        Matrix44 viewProjection;
        Matrix44 invViewProjection;
        
        Vector3 position;
        Vector3 forward;
        Vector3 up;
        Vector3 right;
        real zNear;
        real zFar;
        real fov;
        
        Frustum frustum;
        Vector3 corners[8];
        
        ProjectionType projectionType;
        bool bIsDirty;
        
        void ConditionalUpdate();
        
    public:
        SceneView();
        
        SceneView(const Vector3& inPosition, const Vector3& inForwardDirection, const Vector3& inUp, const Vector2& inClipDistance, const real inFov, const ProjectionType inProjectionType);
        
        void Rotate(const Vector3& eulerAngles);
        
        void SetPosition(const Vector3& inPosition);
        void SetDirection(const Vector3& inDirection, const Vector3& inUp);
        
        void SetPerspective(const real fovy, const real aspect, const real nearZ, const real farZ = 0.0);
        void SetOrthographic(const real left, const real right, const real top, const real bottom, const real nearZ, const real farZ);
        void Update();
        
        FORCEINLINE const Matrix44& GetView() const
        {
            return view;
        }
        
        FORCEINLINE const Matrix44& GetViewInverse() const
        {
            return invView;
        }
        
        FORCEINLINE const Matrix44& GetProjection() const
        {
            return projection;
        }
        
        FORCEINLINE const Matrix44& GetProjectionInverse() const
        {
            return invProjection;
        }
        
        FORCEINLINE const Matrix44& GetViewProjection() const
        {
            return viewProjection;
        }
        
        FORCEINLINE const Matrix44& GetViewProjectionInverse() const
        {
            return invViewProjection;
        }
        
        FORCEINLINE Vector3 GetWorldPosition() const
        {
            return position;
        }
        
        FORCEINLINE Vector3 GetLocalForwardDirection() const
        {
            return forward;
        }
        
        FORCEINLINE Vector3 GetLocalUpDirection() const
        {
            return up;
        }
        
        FORCEINLINE Vector3 GetLocalRightDirection() const
        {
            return right;
        }
        
        FORCEINLINE Vector2 GetClipDistances() const
        {
            return Vector2(zNear, zFar);
        }
        
        FORCEINLINE real GetFov() const
        {
            return fov;
        }
        
        FORCEINLINE bool IsUsingPerspective() const
        {
            return projectionType == ProjectionType::PT_Perspective;
        }
        
        FORCEINLINE bool IsOrthographic() const
        {
            return projectionType == ProjectionType::PT_Orthographic;
        }
        
		RENDER_API friend void Pup(Pupper* p, SceneView& obj, const VarInfo& inVarInfo);
    };
}


#endif
