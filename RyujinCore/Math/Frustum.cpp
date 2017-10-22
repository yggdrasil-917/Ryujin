#include "Frustum.hpp"
#include "AABB.hpp"


namespace Ryujin
{
    Frustum::Frustum(const Matrix44& viewProjectionMatrix)
    {
        UpdateFrustum(viewProjectionMatrix);
    }

    bool Frustum::Contains(const Vector3& point) const
    {
        for (int i = 0; i < 6; ++i)
        {
            if (plane[i].Distance(point) < 0.0f)
                return false;
        }
        return true;
    }

    bool Frustum::Contains(const AABB &box) const
    {
        for (int i = 0; i < 6; ++i)
        {
            const Vector3 positive((plane[i].normal.x < 0.f) ? box.minPoint.x : box.maxPoint.x,
                                     (plane[i].normal.y < 0.f) ? box.minPoint.y : box.maxPoint.y,
                                     (plane[i].normal.z < 0.f) ? box.minPoint.z : box.maxPoint.z);

            if (plane[i].Distance(positive) < 0.0f)
                return false;
        }
        return true;
    }

    bool Frustum::Contains(const Vector3& center, const real radius) const
    {
        for (int i = 0; i < 6; ++i)
        {
            real dist = plane[i].Distance(center);
            if (dist < -radius)
                return false;
        }
        return true;
    }

    bool Frustum::Intersects(const AABB &box) const
    {
        for (int i = 0; i < 6; ++i)
        {
            const Vector3 negative((plane[i].normal.x < 0.f) ? box.maxPoint.x : box.minPoint.x,
                                     (plane[i].normal.y < 0.f) ? box.maxPoint.y : box.minPoint.y,
                                     (plane[i].normal.z < 0.f) ? box.maxPoint.z : box.minPoint.z);

            if (plane[i].Distance(negative) > 0.0f)
                return false;
        }
        return true;
    }

    bool Frustum::FastBoxCheck(const AABB &box) const
    {
        Vector3 center = box.GetCenter();
        Vector3 halfExtent = box.GetExtents() * 0.5f;
        for (int i = 0; i < 6; ++i)
        {
            const Vector3 positive(MathUtils::Abs(plane[i].normal.x), MathUtils::Abs(plane[i].normal.y), MathUtils::Abs(plane[i].normal.z));

            real result = Math::Dot(center, plane[i].normal) + Math::Dot(halfExtent, positive);
            if (result <= -plane[i].distance)
                return false;
        }
        return true;
    }

    void Frustum::UpdateFrustum(const Matrix44& viewProjection)
    {
        // Left Frustum Plane
        // Add first column of the matrix to the fourth column
        plane[0].normal.x = viewProjection[0][3] + viewProjection[0][0];
        plane[0].normal.y = viewProjection[1][3] + viewProjection[1][0];
        plane[0].normal.z = viewProjection[2][3] + viewProjection[2][0];
        plane[0].distance = viewProjection[3][3] + viewProjection[3][0];

        // Right Frustum Plane
        // Subtract first column of matrix from the fourth column
        plane[1].normal.x = viewProjection[0][3] - viewProjection[0][0];
        plane[1].normal.y = viewProjection[1][3] - viewProjection[1][0];
        plane[1].normal.z = viewProjection[2][3] - viewProjection[2][0];
        plane[1].distance = viewProjection[3][3] - viewProjection[3][0];

        // Top Frustum Plane
        // Subtract second column of matrix from the fourth column
        plane[2].normal.x = viewProjection[0][3] - viewProjection[0][1];
        plane[2].normal.y = viewProjection[1][3] - viewProjection[1][1];
        plane[2].normal.z = viewProjection[2][3] - viewProjection[2][1];
        plane[2].distance = viewProjection[3][3] - viewProjection[3][1];

        // Bottom Frustum Plane
        // Add second column of the matrix to the fourth column
        plane[3].normal.x = viewProjection[0][3] + viewProjection[0][1];
        plane[3].normal.y = viewProjection[1][3] + viewProjection[1][1];
        plane[3].normal.z = viewProjection[2][3] + viewProjection[2][1];
        plane[3].distance = viewProjection[3][3] + viewProjection[3][1];

        // Near Frustum Plane
        plane[4].normal.x = viewProjection[0][3] + viewProjection[0][2];
        plane[4].normal.y = viewProjection[1][3] + viewProjection[1][2];
        plane[4].normal.z = viewProjection[2][3] + viewProjection[2][2];
        plane[4].distance = viewProjection[3][3] + viewProjection[3][2];

        // Far Frustum Plane
        // Subtract third column of matrix from the fourth column
        plane[5].normal.x = viewProjection[0][3] - viewProjection[0][2];
        plane[5].normal.y = viewProjection[1][3] - viewProjection[1][2];
        plane[5].normal.z = viewProjection[2][3] - viewProjection[2][2];
        plane[5].distance = viewProjection[3][3] - viewProjection[3][2];

        for (unsigned i = 0; i < 6; ++i)
        {
            real oneOverLength = 1.0f / Math::Length(plane[i].normal);
            plane[i].normal *= oneOverLength;
            plane[i].distance *= oneOverLength;
        }
    }
    
    void Pup(Pupper* p, Frustum& obj, const VarInfo& inVarInfo)
    {
        for (int32 i = 0; i < 6; ++i)
        {
            Pup(p, obj.plane[i].normal, String::Printf("%s.plane_%d.normal", *inVarInfo.name, i));
            Pup(p, obj.plane[i].distance, String::Printf("%s.plane_%d.distance", *inVarInfo.name, i));
        }
    }
    
}
