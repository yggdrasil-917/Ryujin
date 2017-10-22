
#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP


#include "../OS/Platform.hpp"
#include "../Math/Math.hpp"

#if _DEBUG
    #define USE_NAN_CHECKS 1
#endif


namespace Ryujin
{
    
    class CORE_API Transform
    {
    private:
        Vector3 translation;
        Vector3 scale;
        mutable Quaternion rotation;
        
        mutable Matrix44 cachedMatrix;
        mutable Matrix44 cachedInvMatrix;
        mutable bool bDirty = true;
        
#if USE_NAN_CHECKS
        FORCEINLINE void CheckScaleForNan() const
        {
            RYUJIN_ASSERT(!(std::isnan(scale.x) && std::isnan(scale.y) && std::isnan(scale.z)), "Scale has nan values");
        }
        
        FORCEINLINE void CheckTranslationForNan() const
        {
            RYUJIN_ASSERT(!(std::isnan(translation.x) && std::isnan(translation.y) && std::isnan(translation.z)), "Translation has nan values");
        }
        
        FORCEINLINE void CheckRotationForNan() const
        {
            RYUJIN_ASSERT(!(std::isnan(rotation.v.x) && std::isnan(rotation.v.y) && std::isnan(rotation.v.z) && std::isnan(rotation.w)), "Rotation has nan values");
        }
        
        FORCEINLINE void CheckAllForNan() const
        {
            CheckScaleForNan();
            CheckTranslationForNan();
            CheckRotationForNan();
        }
#else
        FORCEINLINE void CheckScaleForNan() const
        {
        }
        
        FORCEINLINE void CheckTranslationForNan() const
        {
            
        }
        
        FORCEINLINE void CheckRotationForNan() const
        {
            
        }
        
        FORCEINLINE void CheckAllForNan() const
        {
        }
        
#endif
        
        FORCEINLINE bool IsRotationNormalized(float tolerance = 0.001f) const
        {
            return fabs(1.0f - (rotation.v.x * rotation.v.x + rotation.v.y * rotation.v.y + rotation.v.z * rotation.v.z + rotation.w * rotation.w)) <= tolerance;
        }
        
    public:
        Transform();
        Transform(const Transform& transform);
        Transform(Transform&& transform);
        Transform(const Vector3& inTranslation, const Vector3& inScale, const Quaternion& inRotation);
        Transform(const Vector3& inTranslation);
        Transform(const Quaternion& inRotation);
        
        static const Transform identity;
        
        FORCEINLINE Transform& operator=(const Transform& other)
        {
            translation = other.translation;
            scale = other.scale;
            rotation = other.rotation;
            bDirty = true;
            
            CheckAllForNan();
            
            return *this;
        }
        
        FORCEINLINE Transform& operator=(Transform&& other)
        {
            translation = Move(other.translation);
            scale = Move(other.scale);
            rotation = Move(other.rotation);
            bDirty = true;
            
            CheckAllForNan();
            
            return *this;
        }
        
        // A * B will apply transform a first
        void Multiply(const Transform& a, const Transform& b, Transform& out);
        void AddTranslation(const Vector3& delta);
        void Scale(const Vector3& inScale);
        void ConcatRotation(const Quaternion& quat);
        Vector3 TransformPoint(const Vector3& point);
        Vector3 TransformVector(const Vector3& v);
        Vector3 InverseTransformPoint(const Vector3& point);
        Vector3 InverseTransformVector(const Vector3& v);
        void SetIdentity();
        
        FORCEINLINE Transform operator*(const Transform& rhs)
        {
            Transform output;
            Multiply(*this, rhs, output);
            return output;
        }
        
        FORCEINLINE Transform& operator*=(const Transform& rhs)
        {
            Multiply(*this, rhs, *this);
            bDirty = true;
            return *this;
        }
        
        Matrix44 ToMatrix() const;
        Matrix44 ToInverseMatrix() const;
        
        FORCEINLINE void SetLocation(const Vector3& inLocation) { translation = inLocation; bDirty = true; }
        FORCEINLINE void SetRotation(const Quaternion& inQuat) { rotation = inQuat; bDirty = true; }
        FORCEINLINE void SetScale(const Vector3& inScale) { scale = inScale; bDirty = true; }
        
        FORCEINLINE Vector3 GetTranslation() const { return translation; }
        FORCEINLINE Vector3 GetScale() const { return scale; }
        FORCEINLINE Quaternion GetRotation() const { return rotation; }
        
        
        friend CORE_API void Pup(struct Pupper* p, Transform& obj, const struct VarInfo& inVarInfo);
    };
}


#endif
