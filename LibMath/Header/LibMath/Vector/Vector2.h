#ifndef LIBMATH_VECTOR_VECTOR2_H_
#define LIBMATH_VECTOR_VECTOR2_H_

#include "LibMath/Angle/Radian.h"

namespace LibMath
{
	class Vector2
	{
	public :
						Vector2();													// set all component to 0
		explicit		Vector2(float value);										// set all component to the same value
						Vector2(float x, float y);									// set all component individually
						Vector2(Vector2 const& other);								// copy all component
						~Vector2() = default;

		static Vector2	zero();														// return a vector with all its component set to 0
		static Vector2	one();														// return a vector with all its component set to 1
		static Vector2	unitX();													// return a vector with its x component set to 1 and y to 0
		static Vector2	unitY();													// return a vector with its y component set to 1 and x to 0

		Vector2&		operator=(Vector2 const&) = default;						

		float			operator[](int index) const;								// return this vector component value
		float&			operator[](int index);										// return this vector component value

		float			dot(Vector2 const& other) const;							// return dot product result

		float			cross(Vector2 const& other) const;							// return cross product result

		float			magnitude() const;											// return vector magnitude
		float			magnitudeSquared() const;									// return square value of the vector magnitude

		void			normalize();												// scale this vector to have magnitude of 1

		bool			isUnitVector() const;										// return true if this vector magnitude is 1

		Radian			angleBetween(Vector2 const& other) const;					// return angle between 2 vectors

		void			projectOnto(Vector2 const& vector);							// project this vector onto another

		void			reflectOnto(Vector2 const& vector);						    // reflect this vector by another

		float m_x;
		float m_y;
	};

	bool				operator==(Vector2 const& lhs, Vector2 const& rhs);
	bool				operator!=(Vector2 const& lhs, Vector2 const& rhs);

	Vector2				operator-(Vector2 v2);

	Vector2				operator+(Vector2 const& lhs, Vector2 const& rhs);
	Vector2				operator-(Vector2 const& lhs, Vector2 const& rhs);
	Vector2				operator*(Vector2 const& lhs, Vector2 const& rhs);
	Vector2				operator*(Vector2 const& v2, float scalar);
	Vector2				operator/(Vector2 const& lhs, Vector2 const& rhs);
	Vector2				operator/(Vector2 const& v2, float scalar);

	Vector2&			operator+=(Vector2& lhs, Vector2 const& rhs);
	Vector2&			operator-=(Vector2& lhs, Vector2 const& rhs);
	Vector2&			operator*=(Vector2& lhs, Vector2 const& rhs);
	Vector2&			operator/=(Vector2& lhs, Vector2 const& rhs);
}

#endif // !LIBMATH_VECTOR_VECTOR2_H_