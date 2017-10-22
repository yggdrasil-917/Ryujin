#ifndef AABB_HPP
#define AABB_HPP


#include "Math.hpp"

namespace Ryujin
{
	struct AABB
	{
		Vector3 minPoint;
		Vector3 maxPoint;

		AABB() : minPoint(Vector3(0.0f)), maxPoint(Vector3(0.0f)) {}
		AABB(const Vector3& _min, const Vector3& _max) : minPoint(_min), maxPoint(_max) {}

		bool Contains(Vector3 point) const;
		bool Contains(const AABB &other) const;
		bool Intersects(const AABB &other) const;
		void Enlarge(Vector3 point);
		void Enlarge(const AABB &other);
		void BuildBox(const Vector3* points, const uint32 count);
		void UpdateBounds(const Vector3& inMin, const Vector3& inMax);

		FORCEINLINE Vector3 GetCenter() const { return (minPoint + maxPoint) * 0.5f; }
		FORCEINLINE Vector3 GetExtents() const { return maxPoint - minPoint; }
	};
}


#endif