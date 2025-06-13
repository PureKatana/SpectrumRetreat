#include "LibMath/Geometry2D.h"
#include "LibMath/Trigonometry.h"
#include "LibMath/Constants.h"
#include "LibMath/Arithmetic.h"

// -------------------------------------------------------------------------------------------------------------------------------------------
// POINT2D
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors

LibMath::Point2D::Point2D() : m_x(0.0f), m_y(0.0f) {}

LibMath::Point2D::Point2D(float x, float y) : m_x(x), m_y(y) {}

LibMath::Point2D::Point2D(const Point2D& other) : m_x(other.m_x), m_y(other.m_y) {}

// -------------------------------------------------------------------------------------------------------------------------------------------
// LINE2D
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors

LibMath::Line2D::Line2D() : m_start(Point2D()), m_end(Point2D()) {}

LibMath::Line2D::Line2D(const Point2D& start, const Point2D& end) : m_start(start), m_end(end) {}

LibMath::Line2D::Line2D(const Line2D& other) : m_start(other.m_start), m_end(other.m_end) {}

// Lenght
float LibMath::Line2D::length() const
{
	return LibMath::squareRoot(lengthSquared());
}

// Lenght Squared
float LibMath::Line2D::lengthSquared() const
{
	float dx = m_end.getX() - m_start.getX();
	float dy = m_end.getY() - m_start.getY();
	return dx * dx + dy * dy;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// RECTANGLE AABB
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors

LibMath::RectangleAABB::RectangleAABB() : m_min(Point2D()), m_max(Point2D()) {}

LibMath::RectangleAABB::RectangleAABB(const Point2D& min, const Point2D& max) : m_min(min), m_max(max) {}

LibMath::RectangleAABB::RectangleAABB(const RectangleAABB& other) : m_min(other.m_min), m_max(other.m_max) {}

// Center
LibMath::Point2D LibMath::RectangleAABB::center() const
{
	return Point2D((m_min.getX() + m_max.getX()) / 2.0f, (m_min.getY() + m_max.getY()) / 2.0f);
}

// ExtentX
float LibMath::RectangleAABB::extentX() const
{
	return (m_max.getX() - m_min.getX()) / 2.0f;
}

// ExtentY
float LibMath::RectangleAABB::extentY() const
{
	return (m_max.getY() - m_min.getY()) / 2.0f;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// RECTANGLE OBB
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors

LibMath::RectangleOBB::RectangleOBB() : m_center(Point2D()), m_width(0.0f), m_height(0.0f), m_rotation(Radian()) {}

LibMath::RectangleOBB::RectangleOBB(const Point2D& center, float width, float height, Radian rotation)
	: m_center(center), m_width(width), m_height(height), m_rotation(rotation) {}

LibMath::RectangleOBB::RectangleOBB(const RectangleOBB& other) 
	: m_center(other.m_center), m_width(other.m_width), m_height(other.m_height), m_rotation(other.m_rotation) {}

// Rotation
void LibMath::RectangleOBB::rotate(Radian deltaRotation)
{
	m_rotation += deltaRotation;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// CIRCLE
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors

LibMath::Circle::Circle() : m_center(Point2D()), m_radius(0.0f) {}

LibMath::Circle::Circle(const Point2D& center, float radius) : m_center(center), m_radius(radius) {}

LibMath::Circle::Circle(const Circle& other) : m_center(other.m_center), m_radius(other.m_radius) {}

// Area
float LibMath::Circle::area() const
{
	return g_pi * m_radius * m_radius;
}

// Circumference
float LibMath::Circle::circumference() const
{
	return 2.0f * g_pi * m_radius;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// SEPARATING AXIS THEOREM (SAT)
// -------------------------------------------------------------------------------------------------------------------------------------------

// Project line onto axis
void LibMath::projectLineOntoAxis(const Line2D& line, const Vector2& axis, float& min, float& max)
{
	Vector2 start(line.getStart().getX(), line.getStart().getY());
	Vector2 end(line.getEnd().getX(), line.getEnd().getY());

	float startProjection = axis.dot(start);
	float endProjection = axis.dot(end);

	min = fmin(startProjection, endProjection);
	max = fmax(startProjection, endProjection);
}

// Project rectangle AABB onto axis
void LibMath::projectRectangleOntoAxis(const RectangleAABB& rect, const Vector2& axis, float& min, float& max)
{
	Point2D minPoint = rect.getMin();
	Point2D maxPoint = rect.getMax();

	Vector2 corners[4] = 
	{
		Vector2(minPoint.getX(), minPoint.getY()),
		Vector2(maxPoint.getX(), minPoint.getY()),
		Vector2(minPoint.getX(), maxPoint.getY()),
		Vector2(maxPoint.getX(), maxPoint.getY())
	};

	min = max = axis.dot(corners[0]);

	for (int i = 1; i < 4; ++i) 
	{
		float projection = axis.dot(corners[i]);
		if (projection < min)
		{
			min = projection;
		}
		if (projection > max)
		{
			max = projection;
		}	
	}
}

// Project rectangle OBB onto axis
void LibMath::projectRectangleOntoAxis(const RectangleOBB& rect, const Vector2& axis, float& min, float& max)
{
	float halfWidth = rect.getWidth() / 2;
	float halfHeight = rect.getHeight() / 2;

	Point2D center = rect.getCenter();
	Radian rotation = rect.getRotation();

	float cosR = LibMath::cos(rotation);
	float sinR = LibMath::sin(rotation);

	// Calculate the corners of the rectangle
	Vector2 corners[4] = 
	{
		{ center.getX() + cosR * halfWidth - sinR * halfHeight, center.getY() + sinR * halfWidth + cosR * halfHeight },
		{ center.getX() - cosR * halfWidth - sinR * halfHeight, center.getY() - sinR * halfWidth + cosR * halfHeight },
		{ center.getX() - cosR * halfWidth + sinR * halfHeight, center.getY() - sinR * halfWidth - cosR * halfHeight },
		{ center.getX() + cosR * halfWidth + sinR * halfHeight, center.getY() + sinR * halfWidth - cosR * halfHeight }
	};

	// Project the corners onto the axis and find the min and max projections
	min = max = axis.dot(corners[0]);

	for (int i = 1; i < 4; ++i) 
	{
		float projection = axis.dot(corners[i]);
		if (projection < min)
		{
			min = projection;
		}
		if (projection > max)
		{
			max = projection;
		}
	}
}

// Check overlap on axis
bool LibMath::overlapOnAxis(float min1, float max1, float min2, float max2)
{
	return !(max1 < min2 || max2 < min1);
}

// Return vector of edges
LibMath::Vector2 LibMath::getEdgeVector(const Point2D& p1, const Point2D& p2)
{
	return Vector2(p2.getX() - p1.getX(), p2.getY() - p1.getY());
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// COLLISIONS 2D
// -------------------------------------------------------------------------------------------------------------------------------------------

// Collision between Point and Line2D
bool LibMath::isColliding(const Point2D& p, const Line2D& l)
{
	Vector2 start(l.getStart().getX(), l.getStart().getY());
	Vector2 end(l.getEnd().getX(), l.getEnd().getY());
	Vector2 point(p.getX(), p.getY());

	Vector2 lineVec = end - start; // Vector representing the line
	Vector2 pointVec = point - start; // Vector from start to point

	// Calculate the projection factor t using the dot product
	float t = pointVec.dot(lineVec) / lineVec.magnitudeSquared();

	// Clamp t to the range [0, 1] to ensure the projection is within the line segment
	t = std::fmax(0.0f, std::fmin(1.0f, t));

	// Calculate the projection point on the line segment
	Vector2 projection = start + lineVec * t;

	// Check if the point coincides with the projection point (collision)
	return (point - projection).magnitudeSquared() == 0;
}

// Collision between Point and Circle
bool LibMath::isColliding(const Point2D& p, const Circle& c)
{
	Vector2 pointVec(p.getX(), p.getY());
	Vector2 centerVec(c.getCenter().getX(), c.getCenter().getY());

	// Check if the squared distance from the point to the circle's center is less than or equal to the squared radius
	return (pointVec - centerVec).magnitudeSquared() <= c.getRadius() * c.getRadius();
}

// Collision between Point and RectangleAABB
bool LibMath::isColliding(const Point2D& p, const RectangleAABB& rect)
{
	return (p.getX() >= rect.getMin().getX() && p.getX() <= rect.getMax().getX() &&
			p.getY() >= rect.getMin().getY() && p.getY() <= rect.getMax().getY());
}

// Collision between Line2D and Circle
bool LibMath::isColliding(const Line2D& l, const Circle& c)
{
	Vector2 start(l.getStart().getX(), l.getStart().getY());
	Vector2 end(l.getEnd().getX(), l.getEnd().getY());
	Vector2 center(c.getCenter().getX(), c.getCenter().getY());

	Vector2 lineVec = end - start; // Vector representing the line segment
	Vector2 centerVec = center - start; // Vector from start of the line to the circle's center

	// Calculate the projection factor t using the dot product
	float t = centerVec.dot(lineVec) / lineVec.magnitudeSquared();

	// Clamp t to the range [0, 1] to ensure the projection is within the line segment
	t = std::fmax(0.0f, std::fmin(1.0f, t));

	// Calculate the projection point on the line segment
	Vector2 projection = start + lineVec * t;

	// Check if the distance from the circle's center to the projection point is less than or equal to the circle's radius
	return (center - projection).magnitudeSquared() <= c.getRadius() * c.getRadius();
}

// Collision between Line2D and RectangleAABB
bool LibMath::isColliding(const Line2D& l, const RectangleAABB& rect)
{
	Vector2 lineVec(l.getEnd().getX() - l.getStart().getX(), l.getEnd().getY() - l.getStart().getY());
	Vector2 perp(-lineVec.m_y, lineVec.m_x); // Perpendicular to the line
	perp.normalize();

	// Rectangle axes
	Vector2 axisX(1, 0);
	Vector2 axisY(0, 1);

	float minLine, maxLine, minRect, maxRect;

	// Check axis perpendicular to the line
	projectLineOntoAxis(l, perp, minLine, maxLine);
	projectRectangleOntoAxis(rect, perp, minRect, maxRect);
	if (!overlapOnAxis(minLine, maxLine, minRect, maxRect))
	{
		return false;
	}
	// Check axis along x-axis
	projectLineOntoAxis(l, axisX, minLine, maxLine);
	projectRectangleOntoAxis(rect, axisX, minRect, maxRect);
	if (!overlapOnAxis(minLine, maxLine, minRect, maxRect))
	{
		return false;
	}
	// Check axis along y-axis
	projectLineOntoAxis(l, axisY, minLine, maxLine);
	projectRectangleOntoAxis(rect, axisY, minRect, maxRect);
	if (!overlapOnAxis(minLine, maxLine, minRect, maxRect))
	{
		return false;
	}

	return true; // No separating axis found, collision occurs
}

// Collision between Line2D and Line2D
bool LibMath::isColliding(const Line2D& l1, const Line2D& l2)
{
	Vector2 p1(l1.getStart().getX(), l1.getStart().getY());
	Vector2 q1(l1.getEnd().getX(), l1.getEnd().getY());
	Vector2 p2(l2.getStart().getX(), l2.getStart().getY());
	Vector2 q2(l2.getEnd().getX(), l2.getEnd().getY());

	Vector2 r = q1 - p1;
	Vector2 s = q2 - p2;

	float rxs = r.cross(s);
	Vector2 p2_p1 = p2 - p1;
	float tNumerator = p2_p1.cross(s);

	if (rxs == 0) 
	{
		// Lines are parallel
		if (tNumerator != 0) 
		{
			// Parallel but not collinear
			return false;
		}

		// Collinear: check if the segments overlap
		float t0 = p2_p1.dot(r) / r.magnitudeSquared();
		float t1 = ((q2 - p1).dot(r)) / r.magnitudeSquared();

		
		if (t0 > t1) 
		{
			float temp = t0;
			t0 = t1;
			t1 = temp;
		}

		// Check if intervals [t0, t1] and [0, 1] overlap
		return (t0 <= 1 && t1 >= 0);
	}

	// Lines are not parallel, check for intersection within segments
	float t = tNumerator / rxs;
	float u = p2_p1.cross(r) / rxs;

	return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
}

// Collision between RectangleAABB and RectangleAABB
bool LibMath::isColliding(const RectangleAABB& r1, const RectangleAABB& r2)
{
	return !(r1.getMax().getX() < r2.getMin().getX() || r1.getMin().getX() > r2.getMax().getX() ||
			r1.getMax().getY() < r2.getMin().getY() || r1.getMin().getY() > r2.getMax().getY());
}

// Collision between Circle and Circle
bool LibMath::isColliding(const Circle& c1, const Circle& c2)
{
	Vector2 centerVec1(c1.getCenter().getX(), c1.getCenter().getY());
	Vector2 centerVec2(c2.getCenter().getX(), c2.getCenter().getY());

	float radiusSum = c1.getRadius() + c2.getRadius();

	// Check if the squared distance between the centers is less than or equal to the squared sum of the radius
	return (centerVec1 - centerVec2).magnitudeSquared() <= radiusSum * radiusSum;
}

// Collision between RectangleOBB and RectangleOBB
bool LibMath::isColliding(const RectangleOBB& r1, const RectangleOBB& r2)
{
	Point2D center1 = r1.getCenter();
	Point2D center2 = r2.getCenter();

	// Define the axes to test (normals to the edges of the rectangles)
	Vector2 axes[4] = 
	{
		getEdgeVector(center1, { center1.getX() + LibMath::cos(r1.getRotation()), center1.getY() + LibMath::sin(r1.getRotation()) }),
		getEdgeVector(center1, { center1.getX() - LibMath::sin(r1.getRotation()), center1.getY() + LibMath::cos(r1.getRotation()) }),
		getEdgeVector(center2, { center2.getX() + LibMath::cos(r2.getRotation()), center2.getY() + LibMath::sin(r2.getRotation()) }),
		getEdgeVector(center2, { center2.getX() - LibMath::sin(r2.getRotation()), center2.getY() + LibMath::cos(r2.getRotation()) })
	};

	float min1, max1, min2, max2;

	// Check for separation on each axis
	for (Vector2& axis : axes) 
	{
		axis.normalize();
		projectRectangleOntoAxis(r1, axis, min1, max1);
		projectRectangleOntoAxis(r2, axis, min2, max2);
		if (!overlapOnAxis(min1, max1, min2, max2))
		{
			return false; // Separation axis found, no collision
		}
	}

	return true; // No separating axis found, collision occurs
}
