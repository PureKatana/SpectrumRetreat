#include "LibMath/Vector.h"
#include "LibMath/Arithmetic.h"
#include "LibMath/Trigonometry.h"
#include "LibMath/Matrix/Matrix3.h"
#include <sstream>

// -------------------------------------------------------------------------------------------------------------------------------------------
// VECTOR2
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors
LibMath::Vector2::Vector2() : m_x(0.0f), m_y(0.0f) {}

LibMath::Vector2::Vector2(float value) : m_x(value), m_y(value) {}

LibMath::Vector2::Vector2(float x, float y) : m_x(x), m_y(y) {}

LibMath::Vector2::Vector2(Vector2 const& other) : m_x(other.m_x), m_y(other.m_y) {}

// Constants
LibMath::Vector2 LibMath::Vector2::zero()
{
	return Vector2(0.0f, 0.0f);
}

LibMath::Vector2 LibMath::Vector2::one()
{
	return Vector2(1.0f, 1.0f);
}

LibMath::Vector2 LibMath::Vector2::unitX()
{
	return Vector2(1.0f, 0.0f);
}

LibMath::Vector2 LibMath::Vector2::unitY()
{
	return Vector2(0.0f, 1.0f);
}

// Assignment operators
LibMath::Vector2& LibMath::operator+=(Vector2& lhs, Vector2 const& rhs)
{
	lhs.m_x += rhs.m_x;
	lhs.m_y += rhs.m_y;
	return lhs;
}

LibMath::Vector2& LibMath::operator-=(Vector2& lhs, Vector2 const& rhs)
{
	lhs.m_x -= rhs.m_x;
	lhs.m_y -= rhs.m_y;
	return lhs;
}

LibMath::Vector2& LibMath::operator*=(Vector2& lhs, Vector2 const& rhs)
{
	lhs.m_x *= rhs.m_x;
	lhs.m_y *= rhs.m_y;
	return lhs;
}

LibMath::Vector2& LibMath::operator/=(Vector2& lhs, Vector2 const& rhs)
{
	lhs.m_x /= rhs.m_x;
	lhs.m_y /= rhs.m_y;
	return lhs;
}

// Operator []
float LibMath::Vector2::operator[](int index) const
{
	return (index == 0) ? m_x : m_y;
}

float& LibMath::Vector2::operator[](int index)
{
	return (index == 0) ? m_x : m_y;
}

// Comparison operator
bool LibMath::operator==(Vector2 const& lhs, Vector2 const& rhs)
{
	return lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y;
}

bool LibMath::operator!=(Vector2 const& lhs, Vector2 const& rhs)
{
	return !(lhs == rhs);
}

// Unary operator
LibMath::Vector2 LibMath::operator-(Vector2 v2)
{
	return Vector2(-v2.m_x, -v2.m_y);
}

// Binary operators
LibMath::Vector2 LibMath::operator+(Vector2 const& lhs, Vector2 const& rhs)
{
	return Vector2(lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y);
}

LibMath::Vector2 LibMath::operator-(Vector2 const& lhs, Vector2 const& rhs)
{
	return Vector2(lhs.m_x - rhs.m_x, lhs.m_y - rhs.m_y);
}

LibMath::Vector2 LibMath::operator*(Vector2 const& lhs, Vector2 const& rhs)
{
	return Vector2(lhs.m_x * rhs.m_x, lhs.m_y * rhs.m_y);
}

LibMath::Vector2 LibMath::operator*(Vector2 const& v2, float scalar)
{
	return Vector2(v2.m_x * scalar, v2.m_y * scalar);
}

LibMath::Vector2 LibMath::operator/(Vector2 const& lhs, Vector2 const& rhs)
{
	return Vector2(lhs.m_x / rhs.m_x, lhs.m_y / rhs.m_y);
}

LibMath::Vector2 LibMath::operator/(Vector2 const& v2, float scalar)
{
	return Vector2(v2.m_x / scalar, v2.m_y / scalar);
}

// Dot Product
float LibMath::Vector2::dot(Vector2 const& other) const
{
	return m_x * other.m_x + m_y * other.m_y;
}

// Cross Product
float LibMath::Vector2::cross(Vector2 const& other) const
{
	return m_x * other.m_y - m_y * other.m_x;
}

// Magnitude
float LibMath::Vector2::magnitude() const
{
	return squareRoot(magnitudeSquared());
}

// Squared magnitude
float LibMath::Vector2::magnitudeSquared() const
{
	return m_x * m_x + m_y * m_y;
}

//Normalize
void LibMath::Vector2::normalize()
{
	float mag = magnitude();
	if (mag > 0) 
	{
		m_x /= mag;
		m_y /= mag;
	}
	else 
	{
		m_x = 0;
		m_y = 0;
	}
}

// Check if vector is unit vector (magnitude = 1)
bool LibMath::Vector2::isUnitVector() const
{
	return almostEqual(magnitude(), 1.0f);
}

// Angle between vectors
LibMath::Radian LibMath::Vector2::angleBetween(Vector2 const& other) const
{
	float dotProduct = dot(other);
	float mags = magnitude() * other.magnitude();

	return Radian(LibMath::acos(dotProduct / mags));
}

// Project onto another vector
void LibMath::Vector2::projectOnto(Vector2 const& vector)
{
	float dotProduct = dot(vector);
	float magSq = vector.magnitudeSquared();

	if (magSq > 0) 
	{
		float scale = dotProduct / magSq;
		m_x = vector.m_x * scale;
		m_y = vector.m_y * scale;
	}
	else 
	{
		m_x = 0;
		m_y = 0;
	}
}

// Reflect onto another vector
void LibMath::Vector2::reflectOnto(Vector2 const& vector)
{
	Vector2 projection = *this;

	projection.projectOnto(vector);
	m_x = m_x - 2.0f * projection.m_x;
	m_y = m_y - 2.0f * projection.m_y;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// VECTOR3
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors
LibMath::Vector3::Vector3() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}

LibMath::Vector3::Vector3(float value) : m_x(value), m_y(value), m_z(value) {}

LibMath::Vector3::Vector3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

LibMath::Vector3::Vector3(Vector3 const& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) {}


// Constants
LibMath::Vector3 LibMath::Vector3::zero()
{
	return Vector3(0.0f);
}

LibMath::Vector3 LibMath::Vector3::one()
{
	return Vector3(1.0f);
}

LibMath::Vector3 LibMath::Vector3::up()
{
	return Vector3(0.0f, 1.0f, 0.0f);
}

LibMath::Vector3 LibMath::Vector3::down()
{
	return Vector3(0.0f, -1.0f, 0.0f);
}

LibMath::Vector3 LibMath::Vector3::left()
{
	return Vector3(-1.0f, 0.0f, 0.0f);
}

LibMath::Vector3 LibMath::Vector3::right()
{
	return Vector3(1.0f, 0.0f, 0.0f);
}

LibMath::Vector3 LibMath::Vector3::front()
{
	return Vector3(0.0f, 0.0f, 1.0f);
}

LibMath::Vector3 LibMath::Vector3::back()
{
	return Vector3(0.0f, 0.0f, -1.0f);
}

// Operator []
float& LibMath::Vector3::operator[](int index)
{
	if (index == 0) return m_x;
	if (index == 1) return m_y;
	if (index == 2) return m_z;

	throw std::out_of_range("Index out of range for Vector3");
}

float LibMath::Vector3::operator[](int index) const
{
	if (index == 0) return m_x;
	if (index == 1) return m_y;
	if (index == 2) return m_z;

	throw std::out_of_range("Index out of range for Vector3");
}

// AngleFrom function
LibMath::Radian LibMath::Vector3::angleFrom(Vector3 const& other) const
{
	float dotProduct = dot(other);
	float magnitudeProduct = magnitude() * other.magnitude();

	return Radian(LibMath::acos(dotProduct / magnitudeProduct));
}

// Cross Product
LibMath::Vector3 LibMath::Vector3::cross(Vector3 const& other) const
{
	return Vector3
	(
		m_y * other.m_z - m_z * other.m_y,
		m_z * other.m_x - m_x * other.m_z,
		m_x * other.m_y - m_y * other.m_x
	);
}

// Distance functions
float LibMath::Vector3::distanceFrom(Vector3 const& other) const
{
	return squareRoot(distanceSquaredFrom(other));
}

float LibMath::Vector3::distanceSquaredFrom(Vector3 const& other) const
{
	float dx = m_x - other.m_x;
	float dy = m_y - other.m_y;
	float dz = m_z - other.m_z;

	return dx * dx + dy * dy + dz * dz;
}

float LibMath::Vector3::distance2DFrom(Vector3 const& other) const
{
	return squareRoot(distance2DSquaredFrom(other));
}

float LibMath::Vector3::distance2DSquaredFrom(Vector3 const& other) const
{
	float dx = m_x - other.m_x;
	float dy = m_y - other.m_y;

	return dx * dx + dy * dy;
}

// Dot Product
float LibMath::Vector3::dot(Vector3 const& other) const
{
	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z;
}

// Is longer than
bool LibMath::Vector3::isLongerThan(Vector3 const& other) const
{
	return magnitudeSquared() > other.magnitudeSquared();
}

// Is shorther than
bool LibMath::Vector3::isShorterThan(Vector3 const& other) const
{
	return magnitudeSquared() < other.magnitudeSquared();
}

// Check if vector is unit vector (magnitude = 1)
bool LibMath::Vector3::isUnitVector() const
{
	return almostEqual(magnitude(), 1.0f);
}

// Magnitude
float LibMath::Vector3::magnitude() const
{
	return squareRoot(magnitudeSquared());
}

float LibMath::Vector3::magnitudeSquared() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z;
}

// Normalize vector
void LibMath::Vector3::normalize()
{
	float mag = magnitude();
	if (mag > 0)
	{
		m_x /= mag;
		m_y /= mag;
		m_z /= mag;
	}
}

// Project onto another vector
void LibMath::Vector3::projectOnto(Vector3 const& other)
{
	float dotProduct = dot(other);
	float otherMagnitudeSquared = other.magnitudeSquared();

	if (otherMagnitudeSquared > 0)
	{
		float scale = dotProduct / otherMagnitudeSquared;
		m_x = other.m_x * scale;
		m_y = other.m_y * scale;
		m_z = other.m_z * scale;
	}
	else
	{
		m_x = 0;
		m_y = 0;
		m_z = 0;
	}
}

// Reflect onto another vector
void LibMath::Vector3::reflectOnto(Vector3 const& other)
{
	Vector3 projection = *this;

	projection.projectOnto(other);
	m_x = m_x - 2.0f * projection.m_x;
	m_y = m_y - 2.0f * projection.m_y;
	m_z = m_z - 2.0f * projection.m_z;
}

// Rotations
void LibMath::Vector3::rotate(Radian angleX, Radian angleY, Radian angleZ) // Euler angle
{
	float cosYaw = cos(angleZ);
	float sinYaw = sin(angleZ);
	float cosPitch = cos(angleX);
	float sinPitch = sin(angleX);
	float cosRoll = cos(angleY);
	float sinRoll = sin(angleY);

	LibMath::Matrix3 result;
	result[0][0] = cosYaw * cosRoll - sinYaw * sinPitch * sinRoll;
	result[0][1] = cosPitch * sinRoll;
	result[0][2] = -sinYaw * cosRoll + cosYaw * sinPitch * sinRoll;
	result[1][0] = -cosYaw * sinRoll + sinYaw * sinPitch * cosRoll;
	result[1][1] = cosPitch * cosRoll;
	result[1][2] = -sinYaw * sinRoll + cosYaw * sinPitch * cosRoll;
	result[2][0] = sinYaw * cosPitch;
	result[2][1] = -sinPitch;
	result[2][2] = cosYaw * cosPitch;

	// Apply the rotation to the vector
	*this = result * (*this);
}

void LibMath::Vector3::rotate(Radian angle, Vector3 const& axis)
{
	// Normalize the axis vector
	Vector3 normalizedAxis = axis;
	normalizedAxis.normalize();

	// Precompute trigonometric values
	float cosTheta = cos(angle);
	float sinTheta = sin(angle);
	float oneMinusCosTheta = 1 - cosTheta;

	// Extract axis components
	float x = normalizedAxis.m_x;
	float y = normalizedAxis.m_y;
	float z = normalizedAxis.m_z;

	// Compute the rotation matrix elements
	float r11 = cosTheta + x * x * oneMinusCosTheta;
	float r12 = x * y * oneMinusCosTheta - z * sinTheta;
	float r13 = x * z * oneMinusCosTheta + y * sinTheta;

	float r21 = y * x * oneMinusCosTheta + z * sinTheta;
	float r22 = cosTheta + y * y * oneMinusCosTheta;
	float r23 = y * z * oneMinusCosTheta - x * sinTheta;

	float r31 = z * x * oneMinusCosTheta - y * sinTheta;
	float r32 = z * y * oneMinusCosTheta + x * sinTheta;
	float r33 = cosTheta + z * z * oneMinusCosTheta;

	// Apply the rotation matrix to the vector
	float newX = m_x * r11 + m_y * r12 + m_z * r13;
	float newY = m_x * r21 + m_y * r22 + m_z * r23;
	float newZ = m_x * r31 + m_y * r32 + m_z * r33;

	// Update the vector components
	m_x = newX;
	m_y = newY;
	m_z = newZ;
}

// Scale
void LibMath::Vector3::scale(Vector3 const& scale)
{
	m_x *= scale.m_x;
	m_y *= scale.m_y;
	m_z *= scale.m_z;
}

// Strings
std::string LibMath::Vector3::string() const
{
	std::ostringstream oss;
	oss << "{" << m_x << "," << m_y << "," << m_z <<"}";

	return oss.str();
}

std::string LibMath::Vector3::stringLong() const
{
	std::ostringstream oss;
	oss << "Vector3{ x:" << m_x << ", y:" << m_y << ", z:" << m_z <<" }";

	return oss.str();
}

// Translate
void LibMath::Vector3::translate(Vector3 const& translation)
{
	m_x += translation.m_x;
	m_y += translation.m_y;
	m_z += translation.m_z;
}

// Comparators
bool LibMath::operator==(Vector3 const& lhs, Vector3 const& rhs)
{
	return lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y && lhs.m_z == rhs.m_z;
}

bool LibMath::operator!=(Vector3 const& lhs, Vector3 const& rhs)
{
	return !(lhs == rhs);
}

// Unary operator
LibMath::Vector3 LibMath::operator-(Vector3 vec)
{
	return Vector3(-vec.m_x, -vec.m_y, -vec.m_z);
}

// Binary operators
LibMath::Vector3 LibMath::operator+(Vector3 const& lhs, Vector3 const& rhs)
{
	return Vector3(lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y, lhs.m_z + rhs.m_z);
}

LibMath::Vector3 LibMath::operator-(Vector3 const& lhs, Vector3 const& rhs)
{
	return Vector3(lhs.m_x - rhs.m_x, lhs.m_y - rhs.m_y, lhs.m_z - rhs.m_z);
}

LibMath::Vector3 LibMath::operator*(Vector3 const& lhs, Vector3 const& rhs)
{
	return Vector3(lhs.m_x * rhs.m_x, lhs.m_y * rhs.m_y, lhs.m_z * rhs.m_z);
}

LibMath::Vector3 LibMath::operator*(Vector3 const& v3, float scalar)
{
	return Vector3(v3.m_x * scalar, v3.m_y * scalar, v3.m_z * scalar);
}

LibMath::Vector3 LibMath::operator/(Vector3 const& lhs, Vector3 const& rhs)
{
	return Vector3(lhs.m_x / rhs.m_x, lhs.m_y / rhs.m_y, lhs.m_z / rhs.m_z);
}

LibMath::Vector3 LibMath::operator/(Vector3 const& v3, float scalar)
{
	return Vector3(v3.m_x / scalar, v3.m_y / scalar, v3.m_z / scalar);
}

// Assignment operators
LibMath::Vector3& LibMath::operator+=(Vector3& lhs, Vector3 const& rhs)
{
	lhs.m_x += rhs.m_x;
	lhs.m_y += rhs.m_y;
	lhs.m_z += rhs.m_z;

	return lhs;
}

LibMath::Vector3& LibMath::operator-=(Vector3& lhs, Vector3 const& rhs)
{
	lhs.m_x -= rhs.m_x;
	lhs.m_y -= rhs.m_y;
	lhs.m_z -= rhs.m_z;

	return lhs;
}

LibMath::Vector3& LibMath::operator*=(Vector3& lhs, Vector3 const& rhs)
{
	lhs.m_x *= rhs.m_x;
	lhs.m_y *= rhs.m_y;
	lhs.m_z *= rhs.m_z;

	return lhs;
}

LibMath::Vector3& LibMath::operator/=(Vector3& lhs, Vector3 const& rhs)
{
	lhs.m_x /= rhs.m_x;
	lhs.m_y /= rhs.m_y;
	lhs.m_z /= rhs.m_z;

	return lhs;
}

// Stream operators
std::ostream& LibMath::operator<<(std::ostream& os, Vector3 const& vec)
{
	os << vec.string();

	return os;
}

std::istream& LibMath::operator>>(std::istream& is, Vector3& vec)
{
	char discard; // To skip characters like '{', ',', and '}'

	// Read the opening brace '{'
	is >> discard;
	if (discard != '{')
	{
		is.setstate(std::ios::failbit); // Set failbit if the format is incorrect
		return is;
	}

	// Read the x, y, and z components
	is >> vec.m_x >> discard >> vec.m_y >> discard >> vec.m_z;

	// Read the closing brace '}'
	is >> discard;
	if (discard != '}')
	{
		is.setstate(std::ios::failbit); // Set failbit if the format is incorrect
		return is;
	}

	return is;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// VECTOR4
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors
LibMath::Vector4::Vector4() : m_x(0), m_y(0), m_z(0), m_w(0) {}

LibMath::Vector4::Vector4(float value) : m_x(value), m_y(value), m_z(value), m_w(value) {}

LibMath::Vector4::Vector4(float x, float y, float z, float w) : m_x(x), m_y(y), m_z(z), m_w(w) {}

LibMath::Vector4::Vector4(Vector4 const& other) : m_x(other.m_x), m_y(other.m_y), m_z(other.m_z), m_w(other.m_w) {}

// Static factory methods
LibMath::Vector4 LibMath::Vector4::zero() 
{ 
	return Vector4(0, 0, 0, 0); 
}

LibMath::Vector4 LibMath::Vector4::one() 
{ 
	return Vector4(1, 1, 1, 1); 
}

LibMath::Vector4 LibMath::Vector4::unitX() 
{ 
	return Vector4(1, 0, 0, 0);
}

LibMath::Vector4 LibMath::Vector4::unitY() 
{ 
	return Vector4(0, 1, 0, 0);
}

LibMath::Vector4 LibMath::Vector4::unitZ() 
{ 
	return Vector4(0, 0, 1, 0);
}

LibMath::Vector4 LibMath::Vector4::unitW() 
{ 
	return Vector4(0, 0, 0, 1);
}

// Operators []
float& LibMath::Vector4::operator[](int index)
{
	switch (index)
	{
		case 0: return m_x;
		case 1: return m_y;
		case 2: return m_z;
		case 3: return m_w;
		default: throw std::out_of_range("Index out of range for Vector4");
	}
}

float LibMath::Vector4::operator[](int index) const
{
	switch (index)
	{
		case 0: return m_x;
		case 1: return m_y;
		case 2: return m_z;
		case 3: return m_w;
		default: throw std::out_of_range("Index out of range for Vector4");
	}
}

// Dot Product
float LibMath::Vector4::dot(Vector4 const& other) const
{
	return m_x * other.m_x + m_y * other.m_y + m_z * other.m_z + m_w * other.m_w;
}

// Magnitude
float LibMath::Vector4::magnitude() const
{
	return squareRoot(magnitudeSquared());
}

float LibMath::Vector4::magnitudeSquared() const
{
	return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

// Homogenize
LibMath::Vector3 LibMath::Vector4::homogenize() const
{
	if (almostEqual(m_w, 0.f))
	{
		throw std::runtime_error("Cannot homogenize a vector with w = 0.");
	}

	return Vector3(m_x / m_w, m_y / m_w, m_z / m_w);
}

// String representation
std::string LibMath::Vector4::string() const
{
	std::ostringstream oss;
	oss << "{" << m_x << "," << m_y << "," << m_z << "," << m_w << "}";

	return oss.str();
}

std::string LibMath::Vector4::stringLong() const
{
	std::ostringstream oss;
	oss << "Vector3{ x:" << m_x << ", y:" << m_y << ", z:" << m_z << ", w:" << m_w << " }";

	return oss.str();
}

// Comparison operators
bool LibMath::operator==(Vector4 const& lhs, Vector4 const& rhs)
{
	return lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y && lhs.m_z == rhs.m_z && lhs.m_w == rhs.m_w;
}

bool LibMath::operator!=(Vector4 const& lhs, Vector4 const& rhs)
{
	return !(lhs == rhs);
}

// Arithmetic operators
LibMath::Vector4 LibMath::operator-(Vector4 vec)
{
	return Vector4(-vec.m_x, -vec.m_y, -vec.m_z, -vec.m_w);
}

LibMath::Vector4 LibMath::operator+(Vector4 const& lhs, Vector4 const& rhs)
{
	return Vector4(lhs.m_x + rhs.m_x, lhs.m_y + rhs.m_y, lhs.m_z + rhs.m_z, lhs.m_w + rhs.m_w);
}

LibMath::Vector4 LibMath::operator-(Vector4 const& lhs, Vector4 const& rhs)
{
	return Vector4(lhs.m_x - rhs.m_x, lhs.m_y - rhs.m_y, lhs.m_z - rhs.m_z, lhs.m_w - rhs.m_w);
}

LibMath::Vector4 LibMath::operator*(Vector4 const& lhs, Vector4 const& rhs)
{
	return Vector4(lhs.m_x * rhs.m_x, lhs.m_y * rhs.m_y, lhs.m_z * rhs.m_z, lhs.m_w * rhs.m_w);
}

LibMath::Vector4 LibMath::operator*(Vector4 const& v4, float scalar)
{
	return Vector4(v4.m_x * scalar, v4.m_y * scalar, v4.m_z * scalar, v4.m_w * scalar);
}

LibMath::Vector4 LibMath::operator/(Vector4 const& lhs, Vector4 const& rhs)
{
	return Vector4(lhs.m_x / rhs.m_x, lhs.m_y / rhs.m_y, lhs.m_z / rhs.m_z, lhs.m_w / rhs.m_w);
}

LibMath::Vector4 LibMath::operator/(Vector4 const& v4, float scalar)
{
	return Vector4(v4.m_x / scalar, v4.m_y / scalar, v4.m_z / scalar, v4.m_w / scalar);
}

//Assignment operators
LibMath::Vector4& LibMath::operator+=(Vector4& lhs, Vector4 const& rhs)
{
	lhs.m_x += rhs.m_x;
	lhs.m_y += rhs.m_y;
	lhs.m_z += rhs.m_z;
	lhs.m_w += rhs.m_w;

	return lhs;
}

LibMath::Vector4& LibMath::operator-=(Vector4& lhs, Vector4 const& rhs)
{
	lhs.m_x -= rhs.m_x;
	lhs.m_y -= rhs.m_y;
	lhs.m_z -= rhs.m_z;
	lhs.m_w -= rhs.m_w;

	return lhs;
}

LibMath::Vector4& LibMath::operator*=(Vector4& lhs, Vector4 const& rhs)
{
	lhs.m_x *= rhs.m_x;
	lhs.m_y *= rhs.m_y;
	lhs.m_z *= rhs.m_z;
	lhs.m_w *= rhs.m_w;

	return lhs;
}

LibMath::Vector4& LibMath::operator/=(Vector4& lhs, Vector4 const& rhs)
{
	lhs.m_x /= rhs.m_x;
	lhs.m_y /= rhs.m_y;
	lhs.m_z /= rhs.m_z;
	lhs.m_w /= rhs.m_w;

	return lhs;
}

// Stream operators
std::ostream& LibMath::operator<<(std::ostream& os, Vector4 const& vec)
{
	os << vec.string();

	return os;
}

std::istream& LibMath::operator>>(std::istream& is, Vector4& vec)
{
	char discard;

	is >> discard;
	if (discard != '{')
	{
		is.setstate(std::ios::failbit); // Set failbit if the format is incorrect
		return is;
	}

	is >> vec.m_x >> discard >> vec.m_y >> discard >> vec.m_z >> discard >> vec.m_w;

	is >> discard;
	if (discard != '}')
	{
		is.setstate(std::ios::failbit); // Set failbit if the format is incorrect
		return is;
	}

	return is;
}



