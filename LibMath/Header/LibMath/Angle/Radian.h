#ifndef LIBMATH_ANGLE_RADIAN_H_
#define LIBMATH_ANGLE_RADIAN_H_

namespace LibMath
{
	class Degree;

	class Radian
	{
	public:
					Radian();
		explicit	Radian(float value);				// explicit so no ambiguous / implicit conversion from float to angle can happen
					Radian(Radian const& other);
					~Radian() = default;

		operator	LibMath::Degree() const;				// Degree angle = Radian{0.5};		// implicit conversion from Radian to Degree

		Radian&		operator=(Radian const&) = default;
		Radian&		operator+=(Radian const& other);			// Radian angle += Radian{0.5};
		Radian&		operator-=(Radian const& other);			// Radian angle -= Radian{0.5};
		Radian&		operator*=(float scalar);			// Radian angle *= 3;
		Radian&		operator/=(float scalar);			// Radian angle /= 3;

		void		wrap(bool rangePi = false);			// true -> limit m_value to range [-pi, pi[		// false -> limit m_value to range [0, 2 pi[

		float		degree(bool range180 = false) const;	// return angle in degree	// true -> return value in range [-180, 180[	// false -> return value in range [0, 360[
		float		radian(bool rangePi = true) const;	    // return angle in radian	// true -> return value in range [-pi, pi[		// false -> return value in range [0, 2 pi[
		float		raw() const { return m_value; }		    // return m_angle

	private:
		float m_value;
	};

	bool	operator==(Radian lhs, Radian rhs);			// bool isEqual = Radian{0.5} == Radian{0.5};	// true
	bool	operator==(Radian const& lhs, Degree const& rhs);	// bool isEqual = Radian{0.5} == Degree{60};	// false
	bool	operator!=(Radian const& lhs, Radian const& rhs);         // bool isNotEqual = Radian{0.5} != Radian{0.7}; // true
	bool	operator<(Radian const& lhs, Radian const& rhs);          // bool isLess = Radian{0.5} < Radian{1.0};   // true
	bool	operator<=(Radian const& lhs, Radian const& rhs);         // bool isLessOrEqual = Radian{0.5} <= Radian{0.5}; // true
	bool	operator>(Radian const& lhs, Radian const& rhs);          // bool isGreater = Radian{1.0} > Radian{0.5}; // true
	bool	operator>=(Radian const& lhs, Radian const& rhs);         // bool isGreaterOrEqual = Radian{0.5} >= Radian{0.5}; // true

	Radian	operator-(Radian rad);					// Degree angle = - Radian{0.5};				// Radian{-0.5}

	Radian	operator+(Radian const& lhs, Radian const& rhs);			// Radian angle = Radian{0.5} + Radian{0.5};	// Radian{1}
	Radian	operator-(Radian const& lhs, Radian const& rhs);			// Radian angle = Radian{0.5} - Radian{0.5};	// Radian{0}
	Radian	operator*(Radian const& rad, float scalar);			// Radian angle = Radian{0.5} * 3;				// Radian{1.5}
	Radian	operator/(Radian const& rad, float scalar);			// Radian angle = Radian{0.5} / 3;				// Radian{0.166...}

	inline namespace Literal
	{
		LibMath::Radian operator""_rad(long double value);			// Radian angle = 0.5_rad;
		LibMath::Radian operator""_rad(unsigned long long int value);	// Radian angle = 1_rad;
	}
}

#endif // !LIBMATH_ANGLE_RADIAN_H_
