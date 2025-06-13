#ifndef LIBMATH_VECTOR_VECTOR4_H_
#define LIBMATH_VECTOR_VECTOR4_H_

#include <iostream>
#include <string>
#include "LibMath/Vector/Vector3.h"

namespace LibMath
{
    class Vector4
    {
    public:
                        Vector4();                                                 // Set all components to 0
        explicit        Vector4(float value);                                      // Set all components to the same value
                        Vector4(float x, float y, float z, float w);               // Set all components individually
                        Vector4(Vector4 const& other);                             // Copy all components
                        ~Vector4() = default;

        static Vector4  zero();                                                    // Return a vector with all components set to 0
        static Vector4  one();                                                     // Return a vector with all components set to 1
        static Vector4  unitX();                                                   // Return a unit vector along the X-axis
        static Vector4  unitY();                                                   // Return a unit vector along the Y-axis
        static Vector4  unitZ();                                                   // Return a unit vector along the Z-axis
        static Vector4  unitW();                                                   // Return a unit vector along the W-axis

        Vector4&        operator=(Vector4 const&) = default;                       // Assignment operator

        float&          operator[](int index);                                     // Access component by index (non-const)
        float           operator[](int index) const;                               // Access component by index (const)

        float           dot(Vector4 const& other) const;                           // Dot product
        float           magnitude() const;                                         // Magnitude (length) of the vector
        float           magnitudeSquared() const;                                  // Squared magnitude of the vector
                                                                        
        Vector3         homogenize() const;                                        // Homogenize the vector (convert to 3D Cartesian coordinates)

        std::string     string() const;                                            // Compact string representation
        std::string     stringLong() const;                                        // Verbose string representation

        float m_x;
        float m_y;
        float m_z;
        float m_w;
    };

    bool            operator==(Vector4 const& lhs, Vector4 const& rhs);             // Equality check
    bool            operator!=(Vector4 const& lhs, Vector4 const& rhs);             // Inequality check

    Vector4         operator-(Vector4 vec);                                         // Negation
    Vector4         operator+(Vector4 const& lhs, Vector4 const& rhs);              // Component-wise addition
    Vector4         operator-(Vector4 const& lhs, Vector4 const& rhs);              // Component-wise subtraction
    Vector4         operator*(Vector4 const& lhs, Vector4 const& rhs);              // Component-wise multiplication
    Vector4         operator*(Vector4 const& v4, float scalar);
    Vector4         operator/(Vector4 const& lhs, Vector4 const& rhs);              // Component-wise division
    Vector4         operator/(Vector4 const& v4, float scalar);

    Vector4&        operator+=(Vector4& lhs, Vector4 const& rhs);
    Vector4&        operator-=(Vector4& lhs, Vector4 const& rhs);
    Vector4&        operator*=(Vector4& lhs, Vector4 const& rhs);
    Vector4&        operator/=(Vector4& lhs, Vector4 const& rhs);

    std::ostream&   operator<<(std::ostream& os, Vector4 const& vec);                // Output to stream
    std::istream&   operator>>(std::istream& is, Vector4& vec);                      // Input from stream
}

#ifdef LIBMATH_MATRIX_MATRIX4_H_
#include "LibMath/Matrix4Vector4Operation.h"
#endif // LIBMATH_MATRIX_MATRIX4_H_

#endif // !LIBMATH_VECTOR_VECTOR4_H_
