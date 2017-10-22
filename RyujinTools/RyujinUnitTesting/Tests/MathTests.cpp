
#include "../RyujinUnitTesting/RyujinUnitTesting.hpp"
#include "../RyujinUnitTesting/TestCase.hpp"

#include "RyujinCore/Math/Transform.hpp"


namespace Ryujin
{
    //    TEST_CASE(TransformTest, "Transform test")
    //    {
    //        Vector3 point(0,1,0);
    //        Quaternion q = Normalize(Math::EulerToQuaternion(Vector3(0,0,-90.0f * 3.1415f / 180.0f)));
    ////        point = q * point;
    //        Transform t(q);
    ////        Vector4 p = Math::QuaternionToRotationMatrix(q) * Vector4(point, 1.0f);
    ////        point = Vector3(p.x, p.y, p.z);
    //        point = t.TransformPoint(point);
    //        REQUIRE(Math::All(Math::Equal(point, Vector3(1,0,0))));
    //    }
    TEST_CASE(QuaternionTest, "Quaternion test")
    {
        float radP = MathUtils::DegreesToRadians(0);
        float radY = MathUtils::DegreesToRadians(180);
        Quaternion q(Math::EulerToQuaternion(Vector3(radP, radY,0)));
        Vector3 right(1,0,0);
        Vector3 v = Normalize(q * right);
        Vector3 rotatedVector(-1,0,0);
        REQUIRE(Math::All(Math::Equal(v, rotatedVector)));
        
        Quaternion qq(Math::EulerToQuaternion(Vector3(radY, 0,0)));
        Vector3 up(0,1,0);
        v = Normalize(qq * up);
        Vector3 rv(0,-1,0);
        REQUIRE(Math::All(Math::Equal(v, rv)));
    }
    
    TEST_CASE(MatrixDataExtractionTest, "Test for extracting transform data from matrix")
    {
        Vector3 t(1,2,3);
        Transform transform;
        transform.AddTranslation(t);
        Vector3 extractedTransform = Math::ExtractTranslation<3>(transform.ToMatrix());
        bool b = Math::All(Math::Equal(t, extractedTransform));
        REQUIRE(b == true);
        
        Vector3 s(1,2,3);
        transform.SetScale(s);
        Vector3 scale = Math::ExtractScale<3>(transform.ToMatrix());
        b = Math::All(Math::Equal(s, scale));
        REQUIRE(b == true);
        
        Quaternion q = Math::EulerToQuaternion(Vector3(45, 45, 45));
        Matrix44 r = Math::QuaternionToRotationMatrix(q);
        transform.SetRotation(q);
        Matrix44 rotation = Math::ExtractRotation(transform.ToMatrix());
        for (int32 i = 0; i < 3; ++i)
        {
            for (int32 j = 0; j < 3; ++j)
            {
                b = MathUtils::Abs(r[i][j] - rotation[i][j]) < 0.0001f;
                REQUIRE(b == true);
            }
        }
    }
}
