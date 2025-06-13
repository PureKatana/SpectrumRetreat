#include "LibMath/Angle.h"
#include "LibMath/Constants.h"
#include "LibMath/Arithmetic.h"

// -------------------------------------------------------------------------------------------------------------------------------------------
// DEGREE
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors
LibMath::Degree::Degree() : m_value(0.0f) {}

LibMath::Degree::Degree(float value) : m_value(value) {}

LibMath::Degree::Degree(Degree const& other) : m_value(other.m_value) {}

// Conversion operator
LibMath::Degree::operator LibMath::Radian() const
{
	return Radian(m_value * g_pi / 180.0f);
}

// Assignment operator
LibMath::Degree& LibMath::Degree::operator+=(Degree const& other)
{
	m_value += other.m_value;
	return *this;
}

LibMath::Degree& LibMath::Degree::operator-=(Degree const& other)
{
	m_value -= other.m_value;
	return *this;
}

LibMath::Degree& LibMath::Degree::operator*=(float scalar)
{
	m_value *= scalar;
	return *this;
}

LibMath::Degree& LibMath::Degree::operator/=(float scalar)
{
	m_value /= scalar;
	return *this;
}

// Wrap function
void LibMath::Degree::wrap(bool range180)
{
	m_value = range180 ? LibMath::wrap(m_value, -180.0f, 180.0f)
					   : LibMath::wrap(m_value, 0.0f, 360.0f);
}

// Accessors
float LibMath::Degree::degree(bool range180) const
{
	float deg = LibMath::wrap(m_value, 0.0f, 360.0f);

	return (range180 && deg > 180.0f) ? deg - 360.0f : deg; // Adjust for [-180, 180) range if needed
}

float LibMath::Degree::radian(bool rangePi) const
{
	float rad = m_value * g_pi / 180.0f;
	rad = LibMath::wrap(rad, 0.0f, g_twoPi);

	return (rangePi && rad > g_pi) ? rad - g_twoPi : rad; // Adjust for [-pi, pi) range if needed
}

// Comparison operators
bool LibMath::operator==(Degree lhs, Degree rhs)
{
	return LibMath::almostEqual(LibMath::wrap(lhs.raw(), 0.0f, 360.0f),
								LibMath::wrap(rhs.raw(), 0.0f, 360.0f));
}

bool LibMath::operator==(Degree const& lhs, Radian const& rhs)
{
	return lhs == static_cast<Degree>(rhs);
}

bool LibMath::operator!=(Degree const& lhs, Degree const& rhs)
{
	return lhs.raw() != rhs.raw();
}

bool LibMath::operator<(Degree const& lhs, Degree const& rhs)
{
	return lhs.raw() < rhs.raw();
}

bool LibMath::operator<=(Degree const& lhs, Degree const& rhs)
{
	return lhs.raw() <= rhs.raw();
}

bool LibMath::operator>(Degree const& lhs, Degree const& rhs)
{
	return lhs.raw() > rhs.raw();
}

bool LibMath::operator>=(Degree const& lhs, Degree const& rhs)
{
	return lhs.raw() >= rhs.raw();
}

// Unary operator
LibMath::Degree LibMath::operator-(Degree deg)
{
	return Degree(-deg.raw());
}

// Binary operators
LibMath::Degree LibMath::operator+(Degree const& lhs, Degree const& rhs)
{
	return Degree(lhs.raw() + rhs.raw());
}

LibMath::Degree LibMath::operator-(Degree const& lhs, Degree const& rhs)
{
	return Degree(lhs.raw() - rhs.raw());
}

LibMath::Degree LibMath::operator*(Degree const& deg, float scalar)
{
	return Degree(deg.raw() * scalar);
}

LibMath::Degree LibMath::operator/(Degree const& deg, float scalar)
{
	return Degree(deg.raw() / scalar);
}

// Literal operators
LibMath::Degree LibMath::Literal::operator""_deg(long double value)
{
	return Degree(static_cast<float>(value));
}

LibMath::Degree LibMath::Literal::operator""_deg(unsigned long long int value)
{
	return Degree(static_cast<float>(value));
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// RADIAN
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors
LibMath::Radian::Radian() : m_value(0.0f) {}

LibMath::Radian::Radian(float value) : m_value(value) {}

LibMath::Radian::Radian(Radian const& other) : m_value(other.m_value) {}

// Conversion operator
LibMath::Radian::operator LibMath::Degree() const
{
	return Degree(m_value * 180.0f / g_pi);
}

//Assignment operator
LibMath::Radian& LibMath::Radian::operator+=(Radian const& other)
{
	m_value += other.m_value;
	return *this;
}

LibMath::Radian& LibMath::Radian::operator-=(Radian const& other)
{
	m_value -= other.m_value;
	return *this;
}

LibMath::Radian& LibMath::Radian::operator*=(float scalar)
{
	m_value *= scalar;
	return *this;
}

LibMath::Radian& LibMath::Radian::operator/=(float scalar)
{
	m_value /= scalar;
	return *this;
}

// Wrap function
void LibMath::Radian::wrap(bool rangePi)
{
	m_value = rangePi ? LibMath::wrap(m_value, -g_pi, g_pi)
					  : LibMath::wrap(m_value, 0.0f, g_twoPi);
}

// Accessors
float LibMath::Radian::degree(bool range180) const
{
	float deg = m_value * 180.0f / g_pi;
	deg = LibMath::wrap(deg, 0.0f, 360.0f);

	return (range180 && deg > 180.0f) ? deg - 360.0f : deg; // Adjust for [-180, 180) range if needed
}

float LibMath::Radian::radian(bool rangePi) const
{
	float rad = LibMath::wrap(m_value, 0.0f, g_twoPi);

	return (rangePi && rad >= g_pi) ? rad - g_twoPi : rad; // Adjust for [-pi, pi) range if needed
}

//Comparison operators
bool LibMath::operator==(Radian lhs, Radian rhs)
{
	// Normalize both radian values within the [0, 2*pi) range
	float lhsNormalized = LibMath::wrap(lhs.raw(), 0.0f, g_twoPi);
	float rhsNormalized = LibMath::wrap(rhs.raw(), 0.0f, g_twoPi);

	// Check if the values are approximately equal considering the tolerance
	return LibMath::almostEqual(lhsNormalized, rhsNormalized);
}

bool LibMath::operator==(Radian const& lhs, Degree const& rhs)
{
	return lhs == static_cast<Radian>(rhs);
}

bool LibMath::operator!=(Radian const& lhs, Radian const& rhs)
{
	return lhs.raw() != rhs.raw();
}

bool LibMath::operator<(Radian const& lhs, Radian const& rhs)
{
	return lhs.raw() < rhs.raw();
}

bool LibMath::operator<=(Radian const& lhs, Radian const& rhs)
{
	return lhs.raw() <= rhs.raw();
}

bool LibMath::operator>(Radian const& lhs, Radian const& rhs)
{
	return lhs.raw() > rhs.raw();
}

bool LibMath::operator>=(Radian const& lhs, Radian const& rhs)
{
	return lhs.raw() >= rhs.raw();
}

// Unary operator
LibMath::Radian LibMath::operator-(Radian rad)
{
	return Radian(-rad.raw());
}

// Binary operator
LibMath::Radian LibMath::operator+(Radian const& lhs, Radian const& rhs)
{
	return Radian(lhs.raw() + rhs.raw());
}

LibMath::Radian LibMath::operator-(Radian const& lhs, Radian const& rhs)
{
	return Radian(lhs.raw() - rhs.raw());
}

LibMath::Radian LibMath::operator*(Radian const& rad, float scalar)
{
	return Radian(rad.raw() * scalar);
}

LibMath::Radian LibMath::operator/(Radian const& rad, float scalar)
{
	return Radian(rad.raw() / scalar);
}

// Literal operators
LibMath::Radian LibMath::Literal::operator""_rad(long double value)
{
	return Radian(static_cast<float>(value));
}

LibMath::Radian LibMath::Literal::operator""_rad(unsigned long long int value)
{
	return Radian(static_cast<float>(value));
}


