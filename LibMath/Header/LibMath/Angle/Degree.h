#ifndef LIBMATH_ANGLE_DEGREE_H_
#define LIBMATH_ANGLE_DEGREE_H_

namespace LibMath
{
	class Radian;

	class Degree
	{
	public:
					Degree();
		explicit	Degree(float value);				// explicit so no ambiguous / implicit conversion from float to angle can happen
					Degree(Degree const& other);
					~Degree() = default;

		operator	LibMath::Radian() const;				// Radian angle = Degree{45};		// implicit conversion from Degree to Radian

		Degree&		operator=(Degree const&) = default;
		Degree&		operator+=(Degree const& other);			// Degree angle += Degree{45};
		Degree&		operator-=(Degree const& other);			// Degree angle -= Degree{45};
		Degree&		operator*=(float scalar);			// Degree angle *= 3;
		Degree&		operator/=(float scalar);			// Degree angle /= 3;

		void		wrap(bool range180 = false);		// true -> limit m_value to range [-180, 180[	// false -> limit m_value to range [0, 360[

		float		degree(bool range180 = false) const;	// return angle in degree	// true -> return value in range [-180, 180[	// false -> return value in range [0, 360[
		float		radian(bool rangePi = true) const;		// return angle in radian	// true -> return value in range [-pi, pi[		// false -> return value in range [0, 2 pi[
		float		raw() const { return m_value; }			// return m_angle

	private:
		float m_value;
	};

	bool	operator==(Degree lhs, Degree rhs);						// bool isEqual = Degree{45} == Degree{45};				// true
	bool	operator==(Degree const& lhs, Radian const& rhs);		// bool isEqual = Degree{60} == Radian{0.5};			// false
	bool    operator!=(Degree const& lhs, Degree const& rhs);         // bool isNotEqual = Degree{45} != Degree{90}			// true
	bool    operator<(Degree const& lhs, Degree const& rhs);          // bool isLess = Degree{30} < Degree{45}				// true
	bool    operator<=(Degree const& lhs, Degree const& rhs);         // bool isLessOrEqual = Degree{45} <= Degree{45}		// true
	bool    operator>(Degree const& lhs, Degree const& rhs);          // bool isGreater = Degree{90} > Degree{45}				// true
	bool    operator>=(Degree const& lhs, Degree const& rhs);         // bool isGreaterOrEqual = Degree{45} >= Degree{30}		// true

	Degree	operator-(Degree deg);									// Degree angle = - Degree{45};					// Degree{-45}

	Degree	operator+(Degree const& lhs, Degree const& rhs);	// Degree angle = Degree{45} + Degree{45};		// Degree{90}
	Degree	operator-(Degree const& lhs, Degree const& rhs);	// Degree angle = Degree{45} - Degree{45};		// Degree{0}
	Degree	operator*(Degree const& deg, float scalar);			// Degree angle = Degree{45} * 3;				// Degree{135}
	Degree	operator/(Degree const& deg, float scalar);			// Degree angle = Degree{45} / 3;				// Degree{15}

	inline namespace Literal
	{
		LibMath::Degree operator""_deg(long double value);			// Degree angle = 7.5_deg;
		LibMath::Degree operator""_deg(unsigned long long int value);	// Degree angle = 45_deg;
	}
}

#endif // !LIBMATH_ANGLE_DEGREE_H_
