#include "LibMath/Geometry3D.h"
#include "LibMath/Constants.h"
#include "LibMath/Arithmetic.h"

// -------------------------------------------------------------------------------------------------------------------------------------------
// POINT3D
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors

LibMath::Point3D::Point3D() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}

LibMath::Point3D::Point3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

LibMath::Point3D::Point3D(const Point3D& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) {}

// -------------------------------------------------------------------------------------------------------------------------------------------
// LINE3D
// -------------------------------------------------------------------------------------------------------------------------------------------

LibMath::Line3D::Line3D()
{
	m_origin = Point3D(0.0f, 0.0f, 0.0f);
	m_direction = Vector3(1.0f, 0.0f, 0.0f);
}

LibMath::Line3D::Line3D(const Point3D& origin, const Vector3& direction)
{
	m_origin = origin;
	m_direction = direction;
}

LibMath::Line3D::Line3D(const Line3D& other)
{
	m_origin = other.m_origin;
	m_direction = other.m_direction;
}

float LibMath::Line3D::getLength() const
{
	return m_direction.magnitude();
}

LibMath::Point3D LibMath::Line3D::getPoint(float t) const
{
	// Compute origin + t * direction
	Vector3 result = Vector3(m_origin.getX(), m_origin.getY(), m_origin.getZ()) + m_direction * t;
	return Point3D(result.m_x, result.m_y, result.m_z);
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// PLANE3D
// -------------------------------------------------------------------------------------------------------------------------------------------

LibMath::Plane3D::Plane3D()
{
	m_normal = Vector3(0.0f, 1.0f, 0.0f);
	m_point = Point3D(0.0f, 0.0f, 0.0f);
}

LibMath::Plane3D::Plane3D(const Point3D& point, const Point3D& normal)
{
	m_point = point;
	m_normal = Vector3(normal.getX(), normal.getY(), normal.getZ());
	m_normal.normalize();
}

LibMath::Plane3D::Plane3D(const Plane3D& other)
{
	m_point = other.m_point;
	m_normal = other.m_normal;
	m_normal.normalize();
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// PRISM3DAABB
// -------------------------------------------------------------------------------------------------------------------------------------------

LibMath::Prism3DAABB::Prism3DAABB()
{
	m_min = Point3D(0.0f, 0.0f, 0.0f);
	m_max = Point3D(1.0f, 1.0f, 1.0f);
}

LibMath::Prism3DAABB::Prism3DAABB(const Point3D& min, const Point3D& max)
{
	m_min = min;
	m_max = max;
}

LibMath::Prism3DAABB::Prism3DAABB(const Prism3DAABB& other)
{
	m_min = other.m_min;
	m_max = other.m_max;
}

LibMath::Point3D LibMath::Prism3DAABB::getCenter() const
{
	Vector3 minVec(m_min.getX(), m_min.getY(), m_min.getZ());
	Vector3 maxVec(m_max.getX(), m_max.getY(), m_max.getZ());
	Vector3 centerVec = (minVec + maxVec) * 0.5f;
	return Point3D(centerVec.m_x, centerVec.m_y, centerVec.m_z);
}

float LibMath::Prism3DAABB::extentX() const
{
	return (m_max.getX() - m_min.getX()) / 2.0f;
}

float LibMath::Prism3DAABB::extentY() const
{
	return (m_max.getY() - m_min.getY()) / 2.0f;
}

float LibMath::Prism3DAABB::extentZ() const
{
	return (m_max.getZ() - m_min.getZ()) / 2.0f;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// PRISM3DOBB
// -------------------------------------------------------------------------------------------------------------------------------------------

LibMath::Prism3DOBB::Prism3DOBB()
{
	m_center = Point3D(0.0f, 0.0f, 0.0f);
	m_halfSize = Vector3(1.0f, 1.0f, 1.0f);
}

LibMath::Prism3DOBB::Prism3DOBB(const Point3D& center, const Point3D& halfSize)
{
	m_center = center;
	m_halfSize = Vector3(halfSize.getX(), halfSize.getY(), halfSize.getZ());
}

LibMath::Prism3DOBB::Prism3DOBB(const Prism3DOBB& other)
{
	m_center = other.m_center;
	m_halfSize = other.m_halfSize;
}

void LibMath::Prism3DOBB::rotate(Radian angleX, Radian angleY, Radian angleZ)
{
	m_halfSize.rotate(angleX, angleY, angleZ);
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// SPHERE3D
// -------------------------------------------------------------------------------------------------------------------------------------------

LibMath::Sphere3D::Sphere3D()
{
	m_center = Point3D(0.0f, 0.0f, 0.0f);
	m_radius = 1.0f;
}

LibMath::Sphere3D::Sphere3D(const Point3D& center, float radius)
{
	m_center = center;
	m_radius = radius;
}

LibMath::Sphere3D::Sphere3D(const Sphere3D& other)
{
	m_center = other.m_center;
	m_radius = other.m_radius;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// CAPSULE3D
// -------------------------------------------------------------------------------------------------------------------------------------------

LibMath::Capsule3D::Capsule3D()
{
	m_start = Point3D(0.0f, 0.0f, 0.0f);
	m_end = Point3D(1.0f, 1.0f, 1.0f);
	m_radius = 1.0f;
}

LibMath::Capsule3D::Capsule3D(const Point3D& start, const Point3D& end, float radius)
{
	m_start = start;
	m_end = end;
	m_radius = radius;
}

LibMath::Capsule3D::Capsule3D(const Capsule3D& other)
{
	m_start = other.m_start;
	m_end = other.m_end;
	m_radius = other.m_radius;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// COLLISION DETECTION
// -------------------------------------------------------------------------------------------------------------------------------------------

LibMath::Point3D LibMath::getClosestPointOnSegment(const LibMath::Point3D& point,
	const LibMath::Point3D& start, const LibMath::Point3D& end)
{
	LibMath::Vector3 segment = LibMath::Vector3(end.getX() - start.getX(), end.getY() - start.getY(), end.getZ() - start.getZ());
	LibMath::Vector3 toPoint = LibMath::Vector3(point.getX() - start.getX(), point.getY() - start.getY(), point.getZ() - start.getZ());

	float segmentLengthSquared = segment.magnitudeSquared();
	if (segmentLengthSquared == 0.0f) // Start and end points are the same
	{
		return start; // Closest point is the start point
	}

	float t = toPoint.dot(segment) / segmentLengthSquared; // Project point onto the segment
	t = LibMath::clamp(t, 0.0f, 1.0f); // Clamp t to the range [0, 1]

	// Calculate the closest point on the segment
	return LibMath::Point3D(start.getX() + segment.m_x * t, start.getY() + segment.m_y * t, start.getZ() + segment.m_z * t);
}

bool LibMath::axisAABBIntersection(float origin, float direction, float min, float max, float& tNear, float& tFar)
{
	if (almostEqual(direction, 0.0f))
	{
		if (origin < min || origin > max)
			return false; // Line is parallel and outside slab
	}
	else
	{
		float invDir = 1.0f / direction;
		float t1 = (min - origin) * invDir;
		float t2 = (max - origin) * invDir;
		if (t1 > t2) std::swap(t1, t2);
		tNear = std::max(tNear, t1);
		tFar = std::min(tFar, t2);
		if (tNear > tFar)
			return false; // No intersection on this axis
	}
	return true;
}

LibMath::Point3D LibMath::getClosestPointOnAABB(const Point3D& point, const Prism3DAABB& aabb)
{
	Point3D result;
	result.setX(clamp(point.getX(), aabb.getMin().getX(), aabb.getMax().getX()));
	result.setY(clamp(point.getY(), aabb.getMin().getY(), aabb.getMax().getY()));
	result.setZ(clamp(point.getZ(), aabb.getMin().getZ(), aabb.getMax().getZ()));
	return result; // Return the closest point on the AABB to the given point
}

bool LibMath::isColliding(const Point3D& point, const Prism3DAABB& aabb)
{
	// Check if the point's x-coordinate is within the AABB's x-range
	if (point.getX() < aabb.getMin().getX() || point.getX() > aabb.getMax().getX()) 
	{
		return false; // Point is outside on the x-axis
	}

	// Check if the point's y-coordinate is within the AABB's y-range
	if (point.getY() < aabb.getMin().getY() || point.getY() > aabb.getMax().getY())
	{
		return false; // Point is outside on the y-axis
	}

	// Check if the point's z-coordinate is within the AABB's z-range
	if (point.getZ() < aabb.getMin().getZ() || point.getZ() > aabb.getMax().getZ()) 
	{
		return false; // Point is outside on the z-axis
	}

	// If all checks passed, the point is inside the AABB
	return true;
}

bool LibMath::isColliding(const Point3D& point, const Capsule3D& capsule)
{
	if (capsule.getRadius() <= 0.0f) 
	{
		return false; // Invalid capsule radius
	}

	// Get the closest point on the capsule segment
	Point3D closestPoint = getClosestPointOnSegment(point, capsule.getStart(), capsule.getEnd());

	// Calculate the vector from the closest point to the point
	Vector3 vectorToPoint = Vector3(point.getX() - closestPoint.getX(), point.getY() - closestPoint.getY(), point.getZ() - closestPoint.getZ());

	// Calculate the squared distance from the closest point to the point
	float distanceSquared = vectorToPoint.magnitudeSquared();

	// Calculate the squared radius of the capsule
	float radiusSquared = capsule.getRadius() * capsule.getRadius();

	return distanceSquared <= radiusSquared; // Check if the point is within the capsule's radius
}

bool LibMath::isColliding(const Point3D& point, const Sphere3D& sphere)
{
	if (sphere.getRadius() <= 0.0f) 
	{
		return false; // Invalid sphere radius
	}

	// Calculate the vector from the sphere's center to the point
	Vector3 vectorToPoint = Vector3(point.getX() - sphere.getCenter().getX(), point.getY() - sphere.getCenter().getY(), point.getZ() - sphere.getCenter().getZ());

	// Calculate the squared distance from the sphere's center to the point
	float distanceSquared = vectorToPoint.magnitudeSquared();

	// Calculate the squared radius of the sphere
	float radiusSquared = sphere.getRadius() * sphere.getRadius();

	return distanceSquared <= radiusSquared; // Check if the point is within the sphere's radius
}

bool LibMath::isColliding(const Line3D& line, const Prism3DAABB& aabb)
{
	Point3D origin = line.getOrigin();
	Vector3 direction = line.getDirection();

	// Degenerate case: if the direction is zero, treat it as a point
	if (almostEqual(direction.m_x, 0.0f) && almostEqual(direction.m_y, 0.0f) && almostEqual(direction.m_z, 0.0f))
	{
		// Return true if the origin is inside the AABB
		return (origin.getX() >= aabb.getMin().getX() && origin.getX() <= aabb.getMax().getX() &&
			origin.getY() >= aabb.getMin().getY() && origin.getY() <= aabb.getMax().getY() &&
			origin.getZ() >= aabb.getMin().getZ() && origin.getZ() <= aabb.getMax().getZ());
	}

	float tNear = -INFINITY;
	float tFar = INFINITY;
	
	// Check intersection along each axis
	if (!axisAABBIntersection(origin.getX(), direction.m_x, aabb.getMin().getX(), aabb.getMax().getX(), tNear, tFar))
		return false;
	if (!axisAABBIntersection(origin.getY(), direction.m_y, aabb.getMin().getY(), aabb.getMax().getY(), tNear, tFar))
		return false;
	if (!axisAABBIntersection(origin.getZ(), direction.m_z, aabb.getMin().getZ(), aabb.getMax().getZ(), tNear, tFar))
		return false;

	// The segment is from t=0 (origin) to t=1 (origin + dir).
	// No overlap if the AABB intersection is entirely "after" the segment (t_near > 1)
	// or entirely "before" the segment (t_far < 0).
	if (tNear > 1.0f || tFar < 0.0f) 
	{
		return false; // Line segment does not intersect the AABB
	}

	return true; // Line segment intersects the AABB
}

bool LibMath::isColliding(const Line3D& ray, const Capsule3D& capsule)
{
	const Point3D& origin = ray.getOrigin();
	const Vector3& direction = ray.getDirection();

	const Point3D& start = capsule.getStart();
	const Point3D& end = capsule.getEnd();
	float radius = capsule.getRadius();

	// Check cylindrical body via two successive projections
	Vector3 segment = Vector3(end.getX() - start.getX(), end.getY() - start.getY(), end.getZ() - start.getZ());
	// Segment length squared
	float segmentLengthSquared = segment.magnitudeSquared();
	// Parameter 'l' for the projection of O onto the infinite line AB
	float l;

	// Handle degenerate capsule segment (capsule is a sphere, segmentLengthSquared is close to zero)
	if (almostEqual(segmentLengthSquared, 0.0f)) // Capsule start and end are the same point
	{
		l = 0.0f; // Closest point is the start point
	}
	else
	{
		l = ((origin.getX() - start.getX()) * segment.m_x + (origin.getY() - start.getY()) * segment.m_y + (origin.getZ() - start.getZ()) * segment.m_z) / segmentLengthSquared;
	}

	// Clamp 'l' to the segment range [0, 1]
	l = LibMath::clamp(l, 0.0f, 1.0f);
	// Calculate the closest point on the capsule segment
	Point3D closestPointOnSegment = Point3D(start.getX() + l * segment.m_x, start.getY() + l * segment.m_y, start.getZ() + l * segment.m_z);
	// 
	float t = ((closestPointOnSegment.getX() - origin.getX()) * direction.m_x + (closestPointOnSegment.getY() - origin.getY()) * direction.m_y + (closestPointOnSegment.getZ() - origin.getZ()) * direction.m_z);

	t = LibMath::clamp(t, 0.0f, 1.0f); // Clamp t to the range [0, 1]

	Point3D closestPointOnRay = Point3D(origin.getX() + t * direction.m_x, origin.getY() + t * direction.m_y, origin.getZ() + t * direction.m_z);

	float distanceSquared = (closestPointOnSegment.getX() - closestPointOnRay.getX()) * (closestPointOnSegment.getX() - closestPointOnRay.getX()) +
		(closestPointOnSegment.getY() - closestPointOnRay.getY()) * (closestPointOnSegment.getY() - closestPointOnRay.getY()) +
		(closestPointOnSegment.getZ() - closestPointOnRay.getZ()) * (closestPointOnSegment.getZ() - closestPointOnRay.getZ());
	
	if (distanceSquared <= radius * radius) 
	{
		return true; // Collision detected with the cylindrical part of the capsule.
	}

	// Check the spherical ends of the capsule

	Point3D centers[2] = { start, end };

	for (const Point3D& center : centers)
	{
		Vector3 toCenter = Vector3(origin.getX() - center.getX(), origin.getY() - center.getY(), origin.getZ() - center.getZ());

		float A_quad_coeff = direction.dot(direction); // A = dot(direction, direction)
		float B_quad_coeff = 2.0f * toCenter.dot(direction); // B = 2 * dot(toCenter, direction)
		float C_quad_coeff = toCenter.magnitudeSquared() - radius * radius; // C = magnitudeSquared(toCenter) - radius^2

		if (almostEqual(A_quad_coeff, 0.0f)) // Ray is parallel to the sphere's center
		{
			if (C_quad_coeff <= 0.0f) // Ray starts inside the sphere
			{
				return true; // Collision detected with the spherical end
			}
			continue; // No intersection with this spherical end
		}

		float discriminant = B_quad_coeff * B_quad_coeff - 4.0f * A_quad_coeff * C_quad_coeff; // Discriminant of the quadratic equation
		
		if (discriminant < 0.0f) 
		{
			continue; // No intersection with this spherical end
		}

		float t1 = (-B_quad_coeff - squareRoot(discriminant)) / (2.0f * A_quad_coeff);
		float t2 = (-B_quad_coeff + squareRoot(discriminant)) / (2.0f * A_quad_coeff);

		if (t1 >= 0.0f || t2 >= 0.0f) 
		{
			return true; // Collision detected with the spherical end
		}
	}

	return false; // No collision detected with the capsule
}


bool LibMath::isColliding(const Capsule3D& capsule, const Prism3DAABB& aabb, Vector3& outNormal)
{
	// 1) Initialize outNormal to zero
	outNormal = Vector3::zero();

	// 2) Get capsule segment endpoints and radius
	const Point3D& segA = capsule.getStart();
	const Point3D& segB = capsule.getEnd();
	float          radius = capsule.getRadius();

	// 3) Compute vector AB and its squared length
	Vector3 ab(
		segB.getX() - segA.getX(),
		segB.getY() - segA.getY(),
		segB.getZ() - segA.getZ()
	);
	float abLenSq = ab.magnitudeSquared();

	// 4) Find parameter t ∈ [0,1] of the point on segment closest to the box center
	float t = 0.0f;
	if (abLenSq > 0.0f)
	{
		Point3D boxCenter = aabb.getCenter();
		Vector3 ac(
			boxCenter.getX() - segA.getX(),
			boxCenter.getY() - segA.getY(),
			boxCenter.getZ() - segA.getZ()
		);
		t = ab.dot(ac) / abLenSq;
		t = clamp(t, 0.0f, 1.0f);
	}

	// 5) Compute the closest point on the capsule segment
	Point3D closestOnSegment(
		segA.getX() + ab.m_x * t,
		segA.getY() + ab.m_y * t,
		segA.getZ() + ab.m_z * t
	);

	// 6) Compute the closest point on the AABB to that segment point
	Point3D closestOnAABB = getClosestPointOnAABB(closestOnSegment, aabb);

	// 7) Compute the vector between these two closest points
	Vector3 diff(
		closestOnSegment.getX() - closestOnAABB.getX(),
		closestOnSegment.getY() - closestOnAABB.getY(),
		closestOnSegment.getZ() - closestOnAABB.getZ()
	);
	float distSq = diff.magnitudeSquared();

	// 8) If the squared distance ≤ radius², they collide
	if (distSq <= radius * radius)
	{
		// If diff is nearly zero, pick an arbitrary normal (e.g., up)
		if (distSq < 1e-8f)
		{
			outNormal = Vector3::up();
		}
		else
		{
			// Normalize diff to get the collision normal
			diff.normalize();
			outNormal = diff;
		}
		return true;
	}

	// No collision: outNormal stays as zero
	return false;
}

