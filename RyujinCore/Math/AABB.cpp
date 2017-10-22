#include "AABB.hpp"
#include "../OS/Platform.hpp"



bool Ryujin::AABB::Contains(Vector3 point) const
{
	return	minPoint.x <= point.x && maxPoint.x >= point.x &&
			minPoint.y <= point.y && maxPoint.y >= point.y &&
			minPoint.z <= point.z && maxPoint.z >= point.z;
}

bool Ryujin::AABB::Contains(const AABB &other) const
{
	return  minPoint.x <= other.minPoint.x && maxPoint.x >= other.maxPoint.x &&
			minPoint.y <= other.minPoint.y && maxPoint.y >= other.maxPoint.y &&
			minPoint.z <= other.minPoint.z && maxPoint.z >= other.maxPoint.z;
}

bool Ryujin::AABB::Intersects(const AABB &other) const
{
	return Contains(other);
}

void Ryujin::AABB::Enlarge(Vector3 point)
{
	for (int32 i = 0; i < 3; ++i)
	{
		minPoint[i] = minPoint[i] < point[i] ? minPoint[i] : point[i];
		maxPoint[i] = maxPoint[i] > point[i] ? maxPoint[i] : point[i];
	}
}

void Ryujin::AABB::Enlarge(const AABB &other)
{
	for (int32 i = 0; i < 3; ++i)
	{
		minPoint[i] = minPoint[i] < other.minPoint[i] ? minPoint[i] : other.minPoint[i];
		maxPoint[i] = maxPoint[i] > other.maxPoint[i] ? maxPoint[i] : other.maxPoint[i];
	}
}

void Ryujin::AABB::BuildBox(const Vector3* points, const uint32 count)
{
	Vector3 boxMin = Vector3(9999999.0f);
	Vector3 boxMax = Vector3(-9999999.0f);
	for (uint32 i = 0; i < count; ++i)
	{
		boxMin.x = MathUtils::Min<real>(boxMin.x, points[i].x);
		boxMin.y = MathUtils::Min<real>(boxMin.x, points[i].y);
		boxMin.z = MathUtils::Min<real>(boxMin.x, points[i].z);

		boxMax.x = MathUtils::Max<real>(boxMin.x, points[i].x);
		boxMax.y = MathUtils::Max<real>(boxMin.x, points[i].y);
		boxMax.z = MathUtils::Max<real>(boxMin.x, points[i].z);
	}

	UpdateBounds(boxMin, boxMax);
}

void Ryujin::AABB::UpdateBounds(const Vector3& inMin, const Vector3& inMax)
{
	minPoint = inMin;
	maxPoint = inMax;
}