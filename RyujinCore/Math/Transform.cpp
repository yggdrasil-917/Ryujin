
#include "Transform.hpp"



#include "../Serialization/BinaryFilePupper.hpp"
#include "../Serialization/TextFilePupper.hpp"


namespace Ryujin
{
    const Transform Transform::identity = Transform(Vector3(0.0f), Vector3(1.0f), Quaternion(0.0f, 0.0f, 0.0f, 1.0f));
    
    Transform::Transform()
    {
        translation = Vector3(0.0f);
        scale = Vector3(1.0f);
        
        CheckAllForNan();
    }
    
    Transform::Transform(const Transform& transform)
    :
    translation(transform.translation),
    scale(transform.scale),
    rotation(transform.rotation)
    {
        
        CheckAllForNan();
        
    }
    
    Transform::Transform(Transform&& transform)
    :
    translation(Move(transform.translation)),
    scale(Move(transform.scale)),
    rotation(Move(transform.rotation))
    {
        
        CheckAllForNan();
        
    }
    
    Transform::Transform(const Vector3& inTranslation, const Vector3& inScale, const Quaternion& inRotation)
    :
    translation(inTranslation),
    scale(inScale),
    rotation(inRotation)
    {
        
        CheckAllForNan();
        
    }
    
    Transform::Transform(const Vector3& inTranslation)
    :
    translation(inTranslation),
    scale(Vector3(1.0f))
    {
        
        CheckAllForNan();
        
    }
    
    Transform::Transform(const Quaternion& inRotation)
    :
    scale(Vector3(1.0f)),
    rotation(inRotation)
    {
        
        CheckAllForNan();
        
    }
    
    void Transform::Multiply(const Transform& a, const Transform& b, Transform& out)
    {
        a.CheckAllForNan();
        b.CheckAllForNan();
        
        RYUJIN_ASSERT(a.IsRotationNormalized(), "Rotation for a not normalized");
        RYUJIN_ASSERT(b.IsRotationNormalized(), "Rotation for b not normalized");
        
        out.rotation = b.rotation * a.rotation;
        out.scale = b.scale * a.scale;
        out.translation = b.rotation * (b.scale * a.translation) + b.translation;
        out.CheckAllForNan();
    }
    
    void Transform::AddTranslation(const Vector3& delta)
    {
        translation += delta;
        bDirty = true;
        
        CheckTranslationForNan();
    }
    
    void Transform::Scale(const Vector3 &inScale)
    {
        scale *= inScale;
        bDirty = true;
        
        CheckScaleForNan();
    }
    
    void Transform::ConcatRotation(const Quaternion& quat)
    {
        rotation = quat * rotation;
        bDirty = true;
        
        CheckRotationForNan();
    }
    
    Vector3 Transform::TransformPoint(const Vector3& point)
    {
        Vector4 v( ToMatrix() * Vector4(point, 1.0f) );
        return Vector3(v.x, v.y, v.z);
    }
    
    Vector3 Transform::TransformVector(const Vector3& v)
    {
        Vector4 d( ToMatrix() * Vector4(v, 0.0f) );
        return Vector3(d.x, d.y, d.z);
    }
    
    Vector3 Transform::InverseTransformPoint(const Vector3& point)
    {
        Vector4 temp = ToInverseMatrix() * Vector4(point.x, point.y, point.z, 1.0f);
        return Vector3( temp.x, temp.y, temp.z );
    }
    
    Vector3 Transform::InverseTransformVector(const Vector3& v)
    {
        Vector4 temp = ToInverseMatrix() * Vector4(v.x, v.y, v.z, 0.0f);
        return Vector3( temp.x, temp.y, temp.z );
    }
    
    void Transform::SetIdentity()
    {
        *this = identity;
        bDirty = true;
//        translation = Vector3(0.0f);
//        scale = Vector3(1.0f);
//        rotation = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    Matrix44 Transform::ToMatrix() const
    {
        RYUJIN_ASSERT(IsRotationNormalized(), "Rotation not normalized");
        if (!bDirty)
        {
            return cachedMatrix;
        }
        
        Matrix44 s;
        Matrix44 t;
        
        s[0].x = scale.x; s[1].y = scale.y; s[2].z = scale.z;
        t[3].x = translation.x; t[3].y = translation.y; t[3].z = translation.z;
        rotation = Normalize(rotation);
        
        cachedMatrix = t * Math::QuaternionToRotationMatrix(rotation) * s;
        cachedInvMatrix = Math::Inverse(cachedMatrix);
        bDirty = false;
        return cachedMatrix;
    }
    
    Matrix44 Transform::ToInverseMatrix() const
    {
        RYUJIN_ASSERT(IsRotationNormalized(), "Rotation not normalized");
        if (!bDirty)
        {
            return cachedInvMatrix;
        }
        
        Matrix44 s;
        Matrix44 t;
        
        s[0].x = scale.x; s[1].y = scale.y; s[2].z = scale.z;
        t[3].x = translation.x; t[3].y = translation.y; t[3].z = translation.z;
        
        
        cachedMatrix = t * Math::QuaternionToRotationMatrix(rotation) * s;
        cachedInvMatrix = Math::Inverse(cachedMatrix);
        bDirty = false;
        return cachedInvMatrix;
    }
    
    void Pup(Pupper* p, Transform& obj, const VarInfo& inVarInfo)
    {
        Pup(p, obj.translation, inVarInfo.name + ".translation");
        Pup(p, obj.scale, inVarInfo.name + ".scale");
        Pup(p, obj.rotation, inVarInfo.name + ".rotation");
    }
}















