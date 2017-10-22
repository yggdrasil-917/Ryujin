#ifndef PLANE_HPP
#define PLANE_HPP


#include "../Logger/Logger.hpp"
#include "../Math/Math.hpp"


namespace Ryujin
{
	struct Plane
	{
		Vector3 normal;
		real distance;

		Plane() : normal(Vector3(0.0f, 1.0f, 0.0f)), distance(0.0f) {}
		Plane(Vector3 n, real d) : normal(n), distance(d) {}
		Plane(const Vector3& v0, const Vector3& v1, const Vector3& v2) { Set(v0, v1, v2); }

		real Distance(const Vector3 &point) const;
		void Set(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2);
		FORCEINLINE void Set(Vector3 n, real d) { normal = n; distance = d; }
	};

	FORCEINLINE real Plane::Distance(const Vector3& point) const
	{
        return Math::Dot(normal, point) + distance;
	}

	FORCEINLINE void Plane::Set(const Vector3& v0, const Vector3& v1, const Vector3& v2)
	{
		Vector3 v10 = v1 - v0;
		Vector3 v20 = v2 - v0;
        v10 = Math::Normalize(v10);
        v20 = Math::Normalize(v20);
        normal = Math::Cross(v10, v20);
        distance = Math::Length(v0 - normal);
	}
}


#endif