#ifndef LIBMATH_GEOMETRY2D_H
#define LIBMATH_GEOMETRY2D_H

#include "LibMath/Vector/Vector2.h"

namespace LibMath
{
    // Point class representing a point in 2D space
    class Point2D 
    {
    public:
        Point2D();
        Point2D(float x, float y);
        Point2D(const Point2D& other);
        ~Point2D() = default;

        Point2D& operator=(Point2D const&) = default;

        float   getX() const { return m_x; }                              // Access x component
        float   getY() const { return m_y; }                              // Access y component

    private:
        float   m_x;
        float   m_y;
    };

    // Line2D class representing a line segment defined by two points
    class Line2D 
    {
    public:
        Line2D();
        Line2D(const Point2D& start, const Point2D& end);
        Line2D(const Line2D& other);
        ~Line2D() = default;

        Line2D& operator=(Line2D const&) = default;

        Point2D   getStart() const { return m_start; }                      // Access starting point component
        Point2D   getEnd() const { return m_end; }                          // Access ending point component

        float   length() const;                                           // Return lenght of line
        float   lengthSquared() const;                                    // Return lenght squared of line

    private:
        Point2D   m_start;
        Point2D   m_end;
    };

    // Axis-Aligned Bounding Box (AABB) Rectangle class
    class RectangleAABB 
    {
    public:
        RectangleAABB();
        RectangleAABB(const Point2D& min, const Point2D& max);
        RectangleAABB(const RectangleAABB& other);
        ~RectangleAABB() = default;

        RectangleAABB& operator=(RectangleAABB const&) = default;

        Point2D   getMin() const { return m_min; }                          // Access point minimum component
        Point2D   getMax() const { return m_max; }                          // Access point maximum component

        Point2D   center() const;                                           // Return center point of rectangle
       
        float   extentX() const;                                          // Return distance from center to side along X axis
        float   extentY() const;                                          // Return distance from center to side along Y axis
            
    private:
        Point2D   m_min;
        Point2D   m_max;
    };

    // Oriented Bounding Box (OBB) Rectangle class
    class RectangleOBB 
    {
    public:
        RectangleOBB();
        RectangleOBB(const Point2D& center, float width, float height, Radian rotation);
        RectangleOBB(const RectangleOBB& other);
        ~RectangleOBB() = default;

        RectangleOBB& operator=(RectangleOBB const&) = default;

        Point2D   getCenter() const { return m_center; }                     // Access center point component
        float   getWidth() const { return m_width; }                       // Access width component
        float   getHeight() const { return m_height; }                     // Access height component
        Radian  getRotation() const { return m_rotation; }                // Access rotation component

        void    rotate(Radian deltaRotation);                               // Rotate rectangle by a given deltaRotation

    private:
        Point2D   m_center;
        float   m_width;
        float   m_height;
        Radian  m_rotation; // in radians
    };

    // Circle class representing a circle with a center and radius
    class Circle 
    {
    public:
        Circle();
        Circle(const Point2D& center, float radius);
        Circle(const Circle& other);
        ~Circle() = default;

        Circle& operator=(Circle const&) = default;

        Point2D   getCenter() const { return m_center; }                    // Access center point component
        float   getRadius() const { return m_radius; }                    // Access radius component

        float   area() const;                                             // Return area of circle
        float   circumference() const;                                    // Return circumference of circle
                
    private:
        Point2D   m_center;
        float   m_radius;
    };

    // Separating Axis Theorem (SAT)
    void        projectLineOntoAxis(const Line2D& line, const Vector2& axis, float& min, float& max);                // Project line onto axis
    void        projectRectangleOntoAxis(const RectangleAABB& rect, const Vector2& axis, float& min, float& max);  // Project rectangle onto axis (Rectangle AABB)
    void        projectRectangleOntoAxis(const RectangleOBB& rect, const Vector2& axis, float& min, float& max);   // Project rectangle onto axis (Rectangle OBB)
    bool        overlapOnAxis(float min1, float max1, float min2, float max2);                                     // Check overlap between min and max
    Vector2     getEdgeVector(const Point2D& p1, const Point2D& p2);                                                   // Return vector of edges

    // Collision detection functions
    bool        isColliding(const Point2D& p, const Line2D& l);                                         // Collision between Point and Line2D
    bool        isColliding(const Point2D& p, const Circle& c);                                       // Collision between Point and Circle
    bool        isColliding(const Point2D& p, const RectangleAABB& rect);                             // Collision between Point and RectangleAABB
    bool        isColliding(const Line2D& l, const Circle& c);                                        // Collision between Line2D and Circle
    bool        isColliding(const Line2D& l, const RectangleAABB& rect);                              // Collision between Line2D and RectangleAABB (SAT)
    bool        isColliding(const Line2D& l1, const Line2D& l2);                                        // Collision between Line2D and Line2D
    bool        isColliding(const RectangleAABB& r1, const RectangleAABB& r2);                      // Collision between RectangleAABB and RectangleAABB
    bool        isColliding(const Circle& c1, const Circle& c2);                                    // Collision between Circle and Circle
    bool        isColliding(const RectangleOBB& r1, const RectangleOBB& r2);                        // Collision between RectangleOBB and RectangleOBB (SAT)
}

#endif // !LIBMATH_GEOMETRY_H

