#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP


#include "Plane.hpp"


namespace Ryujin
{
	struct AABB;

	enum FrustumPlane
	{
		FRUSTUM_TOP,
		FRUSTUM_RIGHT,
		FRUSTUM_BOTTOM,
		FRUSTUM_LEFT,
		FRUSTUM_NEAR,
		FRUSTUM_FAR
	};

	struct CORE_API Frustum
	{
		Plane plane[6];

		Frustum() {}
		Frustum(const Matrix44& viewProjectionMatrix);

		bool Contains(const Vector3 &point) const;
		bool Contains(const AABB &box) const;
		bool Contains(const Vector3& center, const real radius) const;
		bool Intersects(const AABB &box) const;
		bool FastBoxCheck(const AABB &box) const;
		void UpdateFrustum(const Matrix44& viewProjection);
        
        
		CORE_API friend void Pup(Pupper* p, Frustum& obj, const VarInfo& inVarInfo);
	};
}


#endif
