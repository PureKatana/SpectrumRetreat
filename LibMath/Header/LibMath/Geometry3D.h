#ifndef LIBMATH_GEOMETRY3D_H
#define LIBMATH_GEOMETRY3D_H

#include "LibMath/Vector/Vector3.h"
#include "LibMath/Angle/Radian.h"

namespace LibMath
{
	class Object3D
	{
	public:
		virtual ~Object3D() = default; // Base class for 3D objects
	};

	// Point class representing a point in 3D space
	class Point3D : public Object3D
	{
	public:
		Point3D();
		Point3D(float x, float y, float z);
		Point3D(const Point3D& other);
		~Point3D() = default;
		Point3D& operator=(Point3D const&) = default;

		float   getX() const { return m_x; }                              // Access x component
		float   getY() const { return m_y; }                              // Access y component
		float   getZ() const { return m_z; }                              // Access z component

		void    setX(float x) { m_x = x; }                                // Set x component
		void    setY(float y) { m_y = y; }                                // Set y component
		void    setZ(float z) { m_z = z; }                                // Set z component

	private:
		float   m_x;
		float   m_y;
		float   m_z;
	};

	// Line3D class representing a line segment defined by two points
	class Line3D : public Object3D
	{
	public:
		Line3D();
		Line3D(const Point3D& origin, const Vector3& direction);
		Line3D(const Line3D& other);
		~Line3D() = default;

		Line3D& operator=(Line3D const&) = default;

		Point3D getOrigin() const { return m_origin; }
		Vector3 getDirection() const { return m_direction; }

		// Optionally, for segments:
		float getLength() const;
		Point3D getPoint(float t) const; // Returns origin + t * direction

	private:
		Point3D m_origin;
		Vector3 m_direction;
	};

	class Plane3D : public Object3D
	{
	public:
		Plane3D();
		Plane3D(const Point3D& point, const Point3D& normal);
		Plane3D(const Plane3D& other);
		~Plane3D() = default;

		Plane3D& operator=(Plane3D const&) = default;

		Point3D   getPoint() const { return m_point; }                      // Access a point on the plane
		Vector3   getNormal() const { return m_normal; }                    // Access the normal vector of the plane

	private:
		Point3D   m_point;  // A point on the plane
		Vector3   m_normal; // The normal vector of the plane
	};

	class Prism3DAABB : public Object3D
	{
		public:
		Prism3DAABB();
		Prism3DAABB(const Point3D& min, const Point3D& max);
		Prism3DAABB(const Prism3DAABB& other);
		~Prism3DAABB() = default;

		Prism3DAABB& operator=(Prism3DAABB const&) = default;
		Point3D   getMin() const { return m_min; }                          // Access minimum corner of the AABB
		Point3D   getMax() const { return m_max; }                          // Access maximum corner of the AABB
		Point3D   getCenter() const;										// Access center of the AABB

		float	extentX() const;											// Access extent along the X-axis
		float	extentY() const;											// Access extent along the Y-axis
		float	extentZ() const;											// Access extent along the Z-axis

	private:
		Point3D   m_min;  // Minimum corner of the AABB
		Point3D   m_max;  // Maximum corner of the AABB
	};

	class Prism3DOBB : public Object3D
	{
		public:
		Prism3DOBB();
		Prism3DOBB(const Point3D& center, const Point3D& halfSize);
		Prism3DOBB(const Prism3DOBB& other);
		~Prism3DOBB() = default;
		Prism3DOBB& operator=(Prism3DOBB const&) = default;

		Point3D   getCenter() const { return m_center; }                    // Access center of the OBB
		Vector3   getHalfSize() const { return m_halfSize; }                // Access half-size of the OBB

		void rotate(Radian angleX, Radian angleY, Radian angleZ);

	private:
		Point3D   m_center;   // Center of the OBB
		Vector3   m_halfSize; // Half-size of the OBB
	};

	class Sphere3D : public Object3D
	{
		public:
		Sphere3D();
		Sphere3D(const Point3D& center, float radius);
		Sphere3D(const Sphere3D& other);
		~Sphere3D() = default;

		Sphere3D& operator=(Sphere3D const&) = default;
		Point3D   getCenter() const { return m_center; }                    // Access center of the sphere
		float     getRadius() const { return m_radius; }                    // Access radius of the sphere

	private:
		Point3D   m_center; // Center of the sphere
		float     m_radius; // Radius of the sphere
	};

	class Capsule3D : public Object3D
	{
		public:
		Capsule3D();
		Capsule3D(const Point3D& start, const Point3D& end, float radius);
		Capsule3D(const Capsule3D& other);
		~Capsule3D() = default;

		Capsule3D& operator=(Capsule3D const&) = default;
		Point3D   getStart() const { return m_start; }                      // Access starting point of the capsule
		Point3D   getEnd() const { return m_end; }                          // Access ending point of the capsule
		float     getRadius() const { return m_radius; }                    // Access radius of the capsule

	private:
		Point3D   m_start; // Starting point of the capsule
		Point3D   m_end;   // Ending point of the capsule
		float     m_radius; // Radius of the capsule
	};

	// Helper functions for 3D geometry
	Point3D		getClosestPointOnSegment(const LibMath::Point3D& point,
		const LibMath::Point3D& start, const LibMath::Point3D& end); // Get closest point on a line segment)

	bool axisAABBIntersection(float origin, float direction, float min, float max, float& tNear, float& tFar); // Check AABB intersection along an axis

	Point3D    getClosestPointOnAABB(const Point3D& point, const Prism3DAABB& aabb); // Get closest point on AABB

	// Collision detection functions
	bool	isColliding(const Point3D& point, const Prism3DAABB& aabb); // Check collision between point and AABB
	bool	isColliding(const Point3D& point, const Capsule3D& capsule); // Check collision between point and capsule
	bool	isColliding(const Point3D& point, const Sphere3D& sphere); // Check collision between point and sphere

	bool	isColliding(const Line3D& line, const Prism3DAABB& aabb); // Check collision between line and AABB
	bool    isColliding(const Line3D& ray, const Capsule3D& capsule); // Check collision between ray and capsule

	bool    isColliding(const Capsule3D& capsule, const Prism3DAABB& aabb, Vector3& outNormal); // Check collision between capsule and AABB

}

#endif// LIBMATH_GEOMETRY3D_H
