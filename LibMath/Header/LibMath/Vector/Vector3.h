#ifndef LIBMATH_VECTOR_VECTOR3_H_
#define LIBMATH_VECTOR_VECTOR3_H_

#include <iostream>
#include <string>

#include "LibMath/Angle/Radian.h"

namespace LibMath
{
	class Vector3
	{
	public:
						Vector3();											// set all component to 0
		explicit		Vector3(float value);								// set all component to the same value
						Vector3(float x, float y, float z);					// set all component individually
						Vector3(Vector3 const& other);						// copy all component
						~Vector3() = default;

		static Vector3	zero();												// return a vector with all its component set to 0
		static Vector3	one();												// return a vector with all its component set to 1
		static Vector3	up();												// return a unit vector pointing upward
		static Vector3	down();												// return a unit vector pointing downward
		static Vector3	left();												// return a unit vector pointing left
		static Vector3	right();											// return a unit vector pointing right
		static Vector3	front();											// return a unit vector pointing forward
		static Vector3	back();												// return a unit vector pointing backward

		Vector3&		operator=(Vector3 const&) = default;

		float&			operator[](int index);								// return this vector component value
		float			operator[](int index) const;						// return this vector component value

		Radian			angleFrom(Vector3 const& other) const;				// return smallest angle between 2 vector

		Vector3			cross(Vector3 const& other) const;					// return a copy of the cross product result

		float			distanceFrom(Vector3 const& other) const;			// return distance between 2 points
		float			distanceSquaredFrom(Vector3 const& other) const;	// return square value of the distance between 2 points
		float			distance2DFrom(Vector3 const& other) const;			// return the distance between 2 points on the X-Y axis only
		float			distance2DSquaredFrom(Vector3 const& other) const;	// return the square value of the distance between 2 points points on the X-Y axis only

		float			dot(Vector3 const& other) const;					// return dot product result

		bool			isLongerThan(Vector3 const& other) const;			// return true if this vector magnitude is greater than the other
		bool			isShorterThan(Vector3 const& other) const;			// return true if this vector magnitude is less than the other

		bool			isUnitVector() const;								// return true if this vector magnitude is 1

		float			magnitude() const;									// return vector magnitude
		float			magnitudeSquared() const;							// return square value of the vector magnitude

		void			normalize();										// scale this vector to have a magnitude of 1

		void			projectOnto(Vector3 const& other);					// project this vector onto an other

		void			reflectOnto(Vector3 const& other);					// reflect this vector by an other

		void			rotate(Radian angleX, Radian angleY, Radian angleZ);// rotate this vector using Euler angle apply in the z, x, y order
		void			rotate(Radian angle, Vector3 const& axis);			// rotate this vector around an arbitrary axis
		//void			rotate(Quaternion const&); todo quaternion			// rotate this vector using a quaternion rotor

		void			scale(Vector3 const& scale);						// scale this vector by a given factor

		std::string		string() const;										// return a string representation of this vector
		std::string		stringLong() const;									// return a verbose string representation of this vector

		void			translate(Vector3 const& translation);				// offset this vector by a given distance

		float m_x;
		float m_y;
		float m_z;
	};

	bool			operator==(Vector3 const& lhs, Vector3 const& rhs);			// Vector3{ 1 } == Vector3::one()				// true					// return if 2 vectors have the same component
	bool			operator!=(Vector3 const& lhs, Vector3 const& rhs);			// Vector3{ 1 } != Vector3::one()				// false				// return if 2 vectors differ by at least a component

	Vector3			operator-(Vector3 vec);										// - Vector3{ .5, 1.5, -2.5 }					// { -.5, -1.5, 2.5 }	// return a copy of a vector with all its component inverted

	Vector3			operator+(Vector3 const& lhs, Vector3 const& rhs);			// Vector3{ .5, 1.5, -2.5 } + Vector3::one()	// { 1.5, 2.5, -1.5 }	// add 2 vectors component wise
	Vector3			operator-(Vector3 const& lhs, Vector3 const& rhs);			// Vector3{ .5, 1.5, -2.5 } - Vector3{ 1 }		// { -.5, .5, -3.5 }	// subtract 2 vectors component wise
	Vector3			operator*(Vector3 const& lhs, Vector3 const& rhs);			// Vector3{ .5, 1.5, -2.5 } * Vector3::zero()	// { 0, 0, 0 }			// multiply 2 vectors component wise
	Vector3			operator*(Vector3 const& v3, float scalar);
	Vector3			operator/(Vector3 const& lhs, Vector3 const& rhs);			// Vector3{ .5, 1.5, -2.5 } / Vector3{ 2 }		// { .25, .75, -1.25 }	// divide 2 vectors component wise
	Vector3			operator/(Vector3 const& v3, float scalar);

	Vector3&		operator+=(Vector3& lhs, Vector3 const& rhs);				// addition component wise
	Vector3&		operator-=(Vector3& lhs, Vector3 const& rhs);				// subtraction component wise
	Vector3&		operator*=(Vector3& lhs, Vector3 const& rhs);				// multiplication component wise
	Vector3&		operator/=(Vector3& lhs, Vector3 const& rhs);				// division component wise

	std::ostream&	operator<<(std::ostream& os, Vector3 const& vec);			// cout << Vector3{ .5, 1.5, -2.5 }				// add a vector string representation to an output stream
	std::istream&	operator>>(std::istream& is, Vector3& vec);					// ifstream file{ save.txt }; file >> vector;	// parse a string representation from an input stream into a vector
}

#endif // !LIBMATH_VECTOR_VECTOR3_H_
