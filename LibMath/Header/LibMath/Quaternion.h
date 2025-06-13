#ifndef LIBMATH_QUATERNION_H_
#define LIBMATH_QUATERNION_H_

#include "Angle/Radian.h"
#include "Vector/Vector3.h"

namespace LibMath
{
	class Quaternion
	{
	public:
		Quaternion();										// set all component to 0
		Quaternion(float, float, float, float);				// set all component individually
		Quaternion(Quaternion const&);						// copy all component
		Quaternion(Radian, Radian, Radian);					// create rotation from euler angles
		Quaternion(Radian, Vector3);						// create rotation from an angle and an axis
		~Quaternion();

		static Quaternion	identity();						// return a valid quaternion with a rotation of 0

		float m_x;
		float m_y;
		float m_z;
		float m_w;
	};

	Quaternion	operator*(Quaternion const&, Quaternion const&);					// todo:
}

#endif // !LIBMATH_QUATERNION_H_
