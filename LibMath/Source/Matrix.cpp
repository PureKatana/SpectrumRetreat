#include "LibMath/Matrix.h"
#include "LibMath/Trigonometry.h"
#include "LibMath/Arithmetic.h"
#include "LibMath/Angle.h"

#include <stdexcept>

// -------------------------------------------------------------------------------------------------------------------------------------------
// MATRIX2
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors
LibMath::Matrix2::Matrix2()
{
	m_data[0][0] = 0; m_data[0][1] = 0;
	m_data[1][0] = 0; m_data[1][1] = 0;
}

LibMath::Matrix2::Matrix2(float diagonal)
{
	m_data[0][0] = diagonal; m_data[0][1] = 0;
	m_data[1][0] = 0; m_data[1][1] = diagonal;
}

LibMath::Matrix2::Matrix2(float m00, float m01, float m10, float m11)
{
	m_data[0][0] = m00; m_data[0][1] = m01;
	m_data[1][0] = m10; m_data[1][1] = m11;
}

LibMath::Matrix2::Matrix2(Matrix2 const& other)
{
	m_data[0][0] = other.m_data[0][0]; m_data[0][1] = other.m_data[0][1];
	m_data[1][0] = other.m_data[1][0]; m_data[1][1] = other.m_data[1][1];
}

// Access operator
float* LibMath::Matrix2::operator[](int index)
{
	return m_data[index];
}

const float* LibMath::Matrix2::operator[](int index) const
{
	return m_data[index];
}

// Binary operators
LibMath::Matrix2 LibMath::operator+(Matrix2 const& lhs, Matrix2 const& rhs)
{
	return Matrix2
	(
		lhs[0][0] + rhs[0][0],lhs[0][1] + rhs[0][1],
		lhs[1][0] + rhs[1][0], lhs[1][1] + rhs[1][1]
	);
}

LibMath::Matrix2 LibMath::operator-(Matrix2 const& lhs, Matrix2 const& rhs)
{
	return Matrix2
	(
		lhs[0][0] - rhs[0][0], lhs[0][1] - rhs[0][1],
		lhs[1][0] - rhs[1][0], lhs[1][1] - rhs[1][1]
	);
}

LibMath::Matrix2 LibMath::operator*(Matrix2 const& lhs, Matrix2 const& rhs)
{
	return Matrix2
	(
		lhs[0][0] * rhs[0][0] + lhs[1][0] * rhs[0][1],
		lhs[0][1] * rhs[0][0] + lhs[1][1] * rhs[0][1],
		lhs[0][0] * rhs[1][0] + lhs[1][0] * rhs[1][1],
		lhs[0][1] * rhs[1][0] + lhs[1][1] * rhs[1][1]
	);
}

LibMath::Matrix2 LibMath::operator*(Matrix2 const& m2, float scalar)
{
	return Matrix2
	(
		m2[0][0] * scalar, m2[0][1] * scalar,
		m2[1][0] * scalar, m2[1][1] * scalar
	);
}

LibMath::Vector2 LibMath::operator*(Matrix2 const& m2, Vector2 const& vector)
{
	return Vector2
	(
		m2[0][0] * vector.m_x + m2[1][0] * vector.m_y,
		m2[0][1] * vector.m_x + m2[1][1] * vector.m_y
	);
}

// Assignment operators
LibMath::Matrix2& LibMath::operator+=(Matrix2& lhs, Matrix2 const& rhs)
{
	lhs[0][0] += rhs[0][0]; lhs[0][1] += rhs[0][1];
	lhs[1][0] += rhs[1][0]; lhs[1][1] += rhs[1][1];
	return lhs;
}

LibMath::Matrix2& LibMath::operator-=(Matrix2& lhs, Matrix2 const& rhs)
{
	lhs[0][0] -= rhs[0][0]; lhs[0][1] -= rhs[0][1];
	lhs[1][0] -= rhs[1][0]; lhs[1][1] -= rhs[1][1];
	return lhs;
}

LibMath::Matrix2& LibMath::operator*=(Matrix2& m2, float scalar)
{
	m2[0][0] *= scalar; m2[0][1] *= scalar;
	m2[1][0] *= scalar; m2[1][1] *= scalar;
	return m2;
}

// Comparison operators
bool LibMath::operator==(Matrix2 const& lhs, Matrix2 const& rhs)
{
	return lhs[0][0] == rhs[0][0] && lhs[0][1] == rhs[0][1] &&
		   lhs[1][0] == rhs[1][0] && lhs[1][1] == rhs[1][1];
}

bool LibMath::operator!=(Matrix2 const& lhs, Matrix2 const& rhs)
{
	return lhs[0][0] != rhs[0][0] || lhs[0][1] != rhs[0][1] ||
		   lhs[1][0] != rhs[1][0] || lhs[1][1] != rhs[1][1];
}

// Tranposed Matrix
LibMath::Matrix2 LibMath::Matrix2::tranpose() const
{
	return Matrix2
	(
		m_data[0][0], m_data[1][0],
		m_data[0][1], m_data[1][1]
	);
}

// Determinant 
float LibMath::Matrix2::determinant() const
{
	return m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0];
}

// Minors Matrix
LibMath::Matrix2 LibMath::Matrix2::minors() const
{
	return Matrix2
	(
		m_data[1][1], m_data[1][0],
		m_data[0][1], m_data[0][0]
	);
}

// Cofactors Matrix
LibMath::Matrix2 LibMath::Matrix2::cofactors() const
{
	return Matrix2
	(
		m_data[1][1], -m_data[1][0],
		-m_data[0][1], m_data[0][0]
	);
}

// Adjugate Matrix
LibMath::Matrix2 LibMath::Matrix2::adjugate() const
{
	return cofactors().tranpose();
}

// Inverse Matrix
LibMath::Matrix2 LibMath::Matrix2::inverse() const
{
	float det = determinant();

	if (almostEqual(det, 0))
	{
		throw std::runtime_error("Matrix is not invertible.\n");
	}
	return adjugate() * (1.0f / det);
}

// create Transform Matrix
LibMath::Matrix2 LibMath::Matrix2::createTransform(Radian const& rotation, Vector2 const& scale)
{
	float c = cos(rotation);
	float s = sin(rotation);

	return Matrix2
	(
		c * scale.m_x, s * scale.m_x,
		-s * scale.m_y, c * scale.m_y
	);
}

// create Rotation Matrix
LibMath::Matrix2 LibMath::Matrix2::createRotation(Radian const& angle)
{
	float c = cos(angle);
	float s = sin(angle);

	return Matrix2
	(
		c, s,
		-s, c
	);
}

// create Scale Matrix
LibMath::Matrix2 LibMath::Matrix2::createScale(Vector2 const& scale)
{
	return Matrix2
	(
		scale.m_x, 0,
		0, scale.m_y
	);
}

// Identity Matrix
LibMath::Matrix2 LibMath::Matrix2::identity()
{
	return Matrix2(1.0f);
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// MATRIX3 
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors
LibMath::Matrix3::Matrix3()
{
	m_data[0][0] = 0; m_data[0][1] = 0; m_data[0][2] = 0;
	m_data[1][0] = 0; m_data[1][1] = 0; m_data[1][2] = 0;
	m_data[2][0] = 0; m_data[2][1] = 0; m_data[2][2] = 0;
}

LibMath::Matrix3::Matrix3(float diagonal)
{
	m_data[0][0] = diagonal; m_data[0][1] = 0; m_data[0][2] = 0;
	m_data[1][0] = 0; m_data[1][1] = diagonal; m_data[1][2] = 0;
	m_data[2][0] = 0; m_data[2][1] = 0; m_data[2][2] = diagonal;
}

LibMath::Matrix3::Matrix3(float m00, float m01, float m02, float m10, 
							 float m11, float m12, float m20, float m21, float m22)
{
	m_data[0][0] = m00; m_data[0][1] = m01; m_data[0][2] = m02;
	m_data[1][0] = m10; m_data[1][1] = m11; m_data[1][2] = m12;
	m_data[2][0] = m20; m_data[2][1] = m21; m_data[2][2] = m22;
}

LibMath::Matrix3::Matrix3(Matrix3 const& other)
{
	m_data[0][0] = other.m_data[0][0]; m_data[0][1] = other.m_data[0][1]; m_data[0][2] = other.m_data[0][2];
	m_data[1][0] = other.m_data[1][0]; m_data[1][1] = other.m_data[1][1]; m_data[1][2] = other.m_data[1][2];
	m_data[2][0] = other.m_data[2][0]; m_data[2][1] = other.m_data[2][1]; m_data[2][2] = other.m_data[2][2];
}

// Access operator
float* LibMath::Matrix3::operator[](int index)
{
	return m_data[index];
}

const float* LibMath::Matrix3::operator[](int index) const
{
	return m_data[index];
}

// Binary operators
LibMath::Matrix3 LibMath::operator+(Matrix3 const& lhs, Matrix3 const& rhs)
{
	return Matrix3
	(
		lhs[0][0] + rhs[0][0], lhs[0][1] + rhs[0][1], lhs[0][2] + rhs[0][2],
		lhs[1][0] + rhs[1][0], lhs[1][1] + rhs[1][1], lhs[1][2] + rhs[1][2],
		lhs[2][0] + rhs[2][0], lhs[2][1] + rhs[2][1], lhs[2][2] + rhs[2][2]
	);
}

LibMath::Matrix3 LibMath::operator-(Matrix3 const& lhs, Matrix3 const& rhs)
{
	return Matrix3
	(
		lhs[0][0] - rhs[0][0], lhs[0][1] - rhs[0][1], lhs[0][2] - rhs[0][2],
		lhs[1][0] - rhs[1][0], lhs[1][1] - rhs[1][1], lhs[1][2] - rhs[1][2],
		lhs[2][0] - rhs[2][0], lhs[2][1] - rhs[2][1], lhs[2][2] - rhs[2][2]
	);
}

LibMath::Matrix3 LibMath::operator*(Matrix3 const& lhs, Matrix3 const& rhs)
{
	Matrix3 result;

	for (int col = 0; col < 3; ++col) 
	{
		for (int row = 0; row < 3; ++row) 
		{
			result[col][row] = 0;
			for (int k = 0; k < 3; ++k) 
			{
				result[col][row] += lhs[k][row] * rhs[col][k];
			}
		}
	}
	return result;
}

LibMath::Matrix3 LibMath::operator*(Matrix3 const& m3, float scalar)
{
	return Matrix3
	(
		m3[0][0] * scalar, m3[0][1] * scalar, m3[0][2] * scalar,
		m3[1][0] * scalar, m3[1][1] * scalar, m3[1][2] * scalar,
		m3[2][0] * scalar, m3[2][1] * scalar, m3[2][2] * scalar
	);
}

LibMath::Vector3 LibMath::operator*(Matrix3 const& m3, Vector3 const& vector)
{
	float x = m3[0][0] * vector.m_x + m3[1][0] * vector.m_y + m3[2][0] * vector.m_z;
	float y = m3[0][1] * vector.m_x + m3[1][1] * vector.m_y + m3[2][1] * vector.m_z;
	float z = m3[0][2] * vector.m_x + m3[1][2] * vector.m_y + m3[2][2] * vector.m_z;

	return Vector3(x, y, z);
}

// Assignment operators
LibMath::Matrix3& LibMath::operator+=(Matrix3& lhs, Matrix3 const& rhs)
{
	lhs[0][0] += rhs[0][0]; lhs[0][1] += rhs[0][1]; lhs[0][2] += rhs[0][2];
	lhs[1][0] += rhs[1][0]; lhs[1][1] += rhs[1][1]; lhs[1][2] += rhs[1][2];
	lhs[2][0] += rhs[2][0]; lhs[2][1] += rhs[2][1]; lhs[2][2] += rhs[2][2];
	return lhs;
}

LibMath::Matrix3& LibMath::operator-=(Matrix3& lhs, Matrix3 const& rhs)
{
	lhs[0][0] -= rhs[0][0]; lhs[0][1] -= rhs[0][1]; lhs[0][2] -= rhs[0][2];
	lhs[1][0] -= rhs[1][0]; lhs[1][1] -= rhs[1][1]; lhs[1][2] -= rhs[1][2];
	lhs[2][0] -= rhs[2][0]; lhs[2][1] -= rhs[2][1]; lhs[2][2] -= rhs[2][2];
	return lhs;
}

LibMath::Matrix3& LibMath::operator*=(Matrix3& m3, float scalar)
{
	m3[0][0] *= scalar; m3[0][1] *= scalar; m3[0][2] *= scalar;
	m3[1][0] *= scalar; m3[1][1] *= scalar; m3[1][2] *= scalar;
	m3[2][0] *= scalar; m3[2][1] *= scalar; m3[2][2] *= scalar;
	return m3;
}

// Comparison operators
bool LibMath::operator==(Matrix3 const& lhs, Matrix3 const& rhs)
{
	return lhs[0][0] == rhs[0][0] && lhs[0][1] == rhs[0][1] && lhs[0][2] == rhs[0][2] &&
		   lhs[1][0] == rhs[1][0] && lhs[1][1] == rhs[1][1] && lhs[1][2] == rhs[1][2] &&
		   lhs[2][0] == rhs[2][0] && lhs[2][1] == rhs[2][1] && lhs[2][2] == rhs[2][2];
}

bool LibMath::operator!=(Matrix3 const& lhs, Matrix3 const& rhs)
{
	return lhs[0][0] != rhs[0][0] || lhs[0][1] != rhs[0][1] || lhs[0][2] != rhs[0][2] ||
		   lhs[1][0] != rhs[1][0] || lhs[1][1] != rhs[1][1] || lhs[1][2] != rhs[1][2] ||
		   lhs[2][0] != rhs[2][0] || lhs[2][1] != rhs[2][1] || lhs[2][2] != rhs[2][2];
}

// Tranpose Matrix
LibMath::Matrix3 LibMath::Matrix3::transpose() const
{
	return Matrix3
	(
		m_data[0][0], m_data[1][0], m_data[2][0],
		m_data[0][1], m_data[1][1], m_data[2][1],
		m_data[0][2], m_data[1][2], m_data[2][2]
	);
}

// Determinant Matrix
float LibMath::Matrix3::determinant() const
{
	return m_data[0][0] * (m_data[1][1] * m_data[2][2] - m_data[1][2] * m_data[2][1])
		 - m_data[0][1] * (m_data[1][0] * m_data[2][2] - m_data[1][2] * m_data[2][0])
		 + m_data[0][2] * (m_data[1][0] * m_data[2][1] - m_data[1][1] * m_data[2][0]);
}

// Minors Matrix
LibMath::Matrix3 LibMath::Matrix3::minors() const
{
	return Matrix3
	(
		m_data[1][1] * m_data[2][2] - m_data[1][2] * m_data[2][1], // Minor for [0][0]
		m_data[1][0] * m_data[2][2] - m_data[1][2] * m_data[2][0], // Minor for [0][1]
		m_data[1][0] * m_data[2][1] - m_data[1][1] * m_data[2][0], // Minor for [0][2]

		m_data[0][1] * m_data[2][2] - m_data[0][2] * m_data[2][1], // Minor for [1][0]
		m_data[0][0] * m_data[2][2] - m_data[0][2] * m_data[2][0], // Minor for [1][1]
		m_data[0][0] * m_data[2][1] - m_data[0][1] * m_data[2][0], // Minor for [1][2]

		m_data[0][1] * m_data[1][2] - m_data[0][2] * m_data[1][1], // Minor for [2][0]
		m_data[0][0] * m_data[1][2] - m_data[0][2] * m_data[1][0], // Minor for [2][1]
		m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0]  // Minor for [2][2]
	);
}

// Cofactors Matrix
LibMath::Matrix3 LibMath::Matrix3::cofactors() const
{
	Matrix3 minorsMatrix = minors();
	return Matrix3
	(
		minorsMatrix[0][0], -minorsMatrix[0][1], minorsMatrix[0][2],
		-minorsMatrix[1][0], minorsMatrix[1][1], -minorsMatrix[1][2],
		minorsMatrix[2][0], -minorsMatrix[2][1], minorsMatrix[2][2]
	);
}

// Adjugate Matrix
LibMath::Matrix3 LibMath::Matrix3::adjugate() const
{
	return cofactors().transpose();
}

// Inverse Matrix
LibMath::Matrix3 LibMath::Matrix3::inverse() const
{
	float det = determinant();
	if (almostEqual(det, 0))
	{
		throw std::runtime_error("Matrix is not invertible.\n");
	}
	return adjugate() * (1.0f / det);
}

// create a 2D Transform Matrix
LibMath::Matrix3 LibMath::Matrix3::createTransform(Vector2 const& translation, Radian const& rotation, Vector2 const& scale)
{
	float c = cos(rotation);
	float s = sin(rotation);

	return Matrix3
	(
		c * scale.m_x, s * scale.m_x, 0,
		-s * scale.m_y, c * scale.m_y, 0,
		translation.m_x, translation.m_y, 1
	);
}

// create a 3D Transform Matrix
LibMath::Matrix3 LibMath::Matrix3::createTransform(Radian const& rotation, Vector3 const& scale)
{
	// create rotation, and scale matrices

	Matrix3 rotationMatrix = createRotationZ(rotation); // Assuming rotation around Z-axis
	Matrix3 scaleMatrix = createScale(Vector3(scale.m_x, scale.m_y, scale.m_z));

	// Combine matrices: R * S
	return rotationMatrix * scaleMatrix;
}

// create a 2D Translation Matrix
LibMath::Matrix3 LibMath::Matrix3::createTranslation(Vector2 const& translation)
{
	return Matrix3
	(
		1, 0, 0,
		0, 1, 0,
		translation.m_x, translation.m_y, 1
	);
}

// create a 2D Rotation Matrix around a point
LibMath::Matrix3 LibMath::Matrix3::createRotation(Point2D const& center, Radian const& angle)
{
	float c = cos(angle);
	float s = sin(angle);

	return Matrix3
	(
		c, s, 0,
		-s, c, 0,
		center.getX() * (1 - c) + center.getY() * s, center.getY() * (1 - c) - center.getX() * s, 1
	);
}

// create a 3D rotation matrix around X axis (Column-major)
LibMath::Matrix3 LibMath::Matrix3::createRotationX(Radian const& angle)
{
	float cosA = cos(angle);
	float sinA = sin(angle);

	return Matrix3
	(
		1, 0, 0,
		0, cosA, sinA,
		0, -sinA, cosA
	);
}

LibMath::Matrix3 LibMath::Matrix3::createRotationY(Radian const& angle)
{
	float cosA = cos(angle);
	float sinA = sin(angle);

	return Matrix3
	(
		cosA, 0, -sinA,
		0, 1, 0,
		sinA, 0, cosA
	);
}

LibMath::Matrix3 LibMath::Matrix3::createRotationZ(Radian const& angle)
{
	float cosA = cos(angle);
	float sinA = sin(angle);

	return Matrix3
	(
		cosA, sinA, 0,
		-sinA, cosA, 0,
		0, 0, 1
	);
}

LibMath::Matrix3 LibMath::Matrix3::createScale(Vector2 const& scale)
{
	return Matrix3
	(
		scale.m_x, 0, 0,
		0, scale.m_y, 0,
		0, 0, 1
	);
}

LibMath::Matrix3 LibMath::Matrix3::createScale(Vector3 const& scale)
{
	return Matrix3
	(
		scale.m_x, 0, 0,
		0, scale.m_y, 0,
		0, 0, scale.m_z
	);
}

LibMath::Matrix3 LibMath::Matrix3::identity()
{
	return Matrix3(1.0f);
}

// -------------------------------------------------------------------------------------------------------------------------------------------
// MATRIX4 
// -------------------------------------------------------------------------------------------------------------------------------------------

// Constructors
LibMath::Matrix4::Matrix4()
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m_data[i][j] = 0.0f;
}

LibMath::Matrix4::Matrix4(float diagonal)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m_data[i][j] = (i == j) ? diagonal : 0.0f;
}

LibMath::Matrix4::Matrix4(float m00, float m01, float m02, float m03, 
						  float m10, float m11, float m12, float m13, 
						  float m20, float m21, float m22, float m23, 
						  float m30, float m31, float m32, float m33)
{
	m_data[0][0] = m00; m_data[0][1] = m01; m_data[0][2] = m02; m_data[0][3] = m03;
	m_data[1][0] = m10; m_data[1][1] = m11; m_data[1][2] = m12; m_data[1][3] = m13;
	m_data[2][0] = m20; m_data[2][1] = m21; m_data[2][2] = m22; m_data[2][3] = m23;
	m_data[3][0] = m30; m_data[3][1] = m31; m_data[3][2] = m32; m_data[3][3] = m33;
}

LibMath::Matrix4::Matrix4(Matrix4 const& other)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m_data[i][j] = other.m_data[i][j];
}

// Operators []
float* LibMath::Matrix4::operator[](int index)
{
	return m_data[index];
}

const float* LibMath::Matrix4::operator[](int index) const
{
	return m_data[index];
}

// Arithmetic operators
LibMath::Matrix4 LibMath::operator+(Matrix4 const& lhs, Matrix4 const& rhs)
{
	Matrix4 result;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			result[i][j] = lhs[i][j] + rhs[i][j];

	return result;
}

LibMath::Matrix4 LibMath::operator-(Matrix4 const& lhs, Matrix4 const& rhs)
{
	Matrix4 result;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			result[i][j] = lhs[i][j] - rhs[i][j];

	return result;
}

LibMath::Matrix4 LibMath::operator*(Matrix4 const& lhs, Matrix4 const& rhs)
{
	Matrix4 result;

	// Perform matrix multiplication in column-major order
	for (int i = 0; i < 4; ++i) 
		for (int j = 0; j < 4; ++j) 
			for (int k = 0; k < 4; ++k) 
				result[j][i] += lhs[k][i] * rhs[j][k];
		
	return result;
}

LibMath::Matrix4 LibMath::operator*(Matrix4 const& m4, float scalar)
{
	Matrix4 result;

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			result[i][j] = m4[i][j] * scalar;

	return result;
}

// Assignment operators
LibMath::Matrix4& LibMath::operator+=(Matrix4& lhs, Matrix4 const& rhs)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			lhs[i][j] += rhs[i][j];

	return lhs;
}

LibMath::Matrix4& LibMath::operator-=(Matrix4& lhs, Matrix4 const& rhs)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			lhs[i][j] -= rhs[i][j];

	return lhs;
}

LibMath::Matrix4& LibMath::operator*=(Matrix4& m4, float scalar)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			m4[i][j] *= scalar;

	return m4;
}

// Comparison operators
bool LibMath::operator==(Matrix4 const& lhs, Matrix4 const& rhs)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			if (lhs[i][j] != rhs[i][j])
				return false;

	return true;
}

bool LibMath::operator!=(Matrix4 const& lhs, Matrix4 const& rhs)
{
	return !(lhs == rhs);
}

// Transpose Matrix
LibMath::Matrix4 LibMath::Matrix4::transpose() const
{
	return Matrix4
	(
		m_data[0][0], m_data[1][0], m_data[2][0], m_data[3][0],
		m_data[0][1], m_data[1][1], m_data[2][1], m_data[3][1],
		m_data[0][2], m_data[1][2], m_data[2][2], m_data[3][2],
		m_data[0][3], m_data[1][3], m_data[2][3], m_data[3][3]
	);
}

// Determinant

// Helper function to compute the determinant of a 3x3 matrix
float determinant3x3(float m00, float m01, float m02,
					float m10, float m11, float m12,
					float m20, float m21, float m22)
{
	return m00 * (m11 * m22 - m12 * m21)
		- m01 * (m10 * m22 - m12 * m20)
		+ m02 * (m10 * m21 - m11 * m20);
}

float LibMath::Matrix4::determinant() const
{
	float det = 0.0f;

	// Compute the determinant using Laplace expansion along the first row
	det += m_data[0][0] * determinant3x3
	(
		m_data[1][1], m_data[1][2], m_data[1][3],
		m_data[2][1], m_data[2][2], m_data[2][3],
		m_data[3][1], m_data[3][2], m_data[3][3]
	);

	det -= m_data[0][1] * determinant3x3
	(
		m_data[1][0], m_data[1][2], m_data[1][3],
		m_data[2][0], m_data[2][2], m_data[2][3],
		m_data[3][0], m_data[3][2], m_data[3][3]
	);

	det += m_data[0][2] * determinant3x3
	(
		m_data[1][0], m_data[1][1], m_data[1][3],
		m_data[2][0], m_data[2][1], m_data[2][3],
		m_data[3][0], m_data[3][1], m_data[3][3]
	);

	det -= m_data[0][3] * determinant3x3
	(
		m_data[1][0], m_data[1][1], m_data[1][2],
		m_data[2][0], m_data[2][1], m_data[2][2],
		m_data[3][0], m_data[3][1], m_data[3][2]
	);

	return det;
}

// Minors Matrix
LibMath::Matrix4 LibMath::Matrix4::minors() const
{
	Matrix4 result;

	result[0][0] = determinant3x3(m_data[1][1], m_data[1][2], m_data[1][3], m_data[2][1], m_data[2][2], m_data[2][3], m_data[3][1], m_data[3][2], m_data[3][3]);
	result[0][1] = determinant3x3(m_data[1][0], m_data[1][2], m_data[1][3], m_data[2][0], m_data[2][2], m_data[2][3], m_data[3][0], m_data[3][2], m_data[3][3]);
	result[0][2] = determinant3x3(m_data[1][0], m_data[1][1], m_data[1][3], m_data[2][0], m_data[2][1], m_data[2][3], m_data[3][0], m_data[3][1], m_data[3][3]);
	result[0][3] = determinant3x3(m_data[1][0], m_data[1][1], m_data[1][2], m_data[2][0], m_data[2][1], m_data[2][2], m_data[3][0], m_data[3][1], m_data[3][2]);

	result[1][0] = determinant3x3(m_data[0][1], m_data[0][2], m_data[0][3], m_data[2][1], m_data[2][2], m_data[2][3], m_data[3][1], m_data[3][2], m_data[3][3]);
	result[1][1] = determinant3x3(m_data[0][0], m_data[0][2], m_data[0][3], m_data[2][0], m_data[2][2], m_data[2][3], m_data[3][0], m_data[3][2], m_data[3][3]);
	result[1][2] = determinant3x3(m_data[0][0], m_data[0][1], m_data[0][3], m_data[2][0], m_data[2][1], m_data[2][3], m_data[3][0], m_data[3][1], m_data[3][3]);
	result[1][3] = determinant3x3(m_data[0][0], m_data[0][1], m_data[0][2], m_data[2][0], m_data[2][1], m_data[2][2], m_data[3][0], m_data[3][1], m_data[3][2]);

	result[2][0] = determinant3x3(m_data[0][1], m_data[0][2], m_data[0][3], m_data[1][1], m_data[1][2], m_data[1][3], m_data[3][1], m_data[3][2], m_data[3][3]);
	result[2][1] = determinant3x3(m_data[0][0], m_data[0][2], m_data[0][3], m_data[1][0], m_data[1][2], m_data[1][3], m_data[3][0], m_data[3][2], m_data[3][3]);
	result[2][2] = determinant3x3(m_data[0][0], m_data[0][1], m_data[0][3], m_data[1][0], m_data[1][1], m_data[1][3], m_data[3][0], m_data[3][1], m_data[3][3]);
	result[2][3] = determinant3x3(m_data[0][0], m_data[0][1], m_data[0][2], m_data[1][0], m_data[1][1], m_data[1][2], m_data[3][0], m_data[3][1], m_data[3][2]);

	result[3][0] = determinant3x3(m_data[0][1], m_data[0][2], m_data[0][3], m_data[1][1], m_data[1][2], m_data[1][3], m_data[2][1], m_data[2][2], m_data[2][3]);
	result[3][1] = determinant3x3(m_data[0][0], m_data[0][2], m_data[0][3], m_data[1][0], m_data[1][2], m_data[1][3], m_data[2][0], m_data[2][2], m_data[2][3]);
	result[3][2] = determinant3x3(m_data[0][0], m_data[0][1], m_data[0][3], m_data[1][0], m_data[1][1], m_data[1][3], m_data[2][0], m_data[2][1], m_data[2][3]);
	result[3][3] = determinant3x3(m_data[0][0], m_data[0][1], m_data[0][2], m_data[1][0], m_data[1][1], m_data[1][2], m_data[2][0], m_data[2][1], m_data[2][2]);

	return result;
}

// Cofactors Matrix
LibMath::Matrix4 LibMath::Matrix4::cofactors() const
{
	Matrix4 minorsMatrix = minors();
	
	return Matrix4
	(
		minorsMatrix[0][0], -minorsMatrix[0][1], minorsMatrix[0][2], -minorsMatrix[0][3],
		-minorsMatrix[1][0], minorsMatrix[1][1], -minorsMatrix[1][2], minorsMatrix[1][3],
		minorsMatrix[2][0], -minorsMatrix[2][1], minorsMatrix[2][2], -minorsMatrix[2][3],
		-minorsMatrix[3][0], minorsMatrix[3][1], -minorsMatrix[3][2], minorsMatrix[3][3]
	);

	
}

// Adjugate Matrix
LibMath::Matrix4 LibMath::Matrix4::adjugate() const
{
	return cofactors().transpose();
}

// Inverse Matrix
LibMath::Matrix4 LibMath::Matrix4::inverse() const
{
	float det = determinant();

	if (almostEqual(det, 0))
	{
		throw std::runtime_error("Matrix is not invertible.\n");
	}

	Matrix4 adj = adjugate();

	return adj * (1.0f / det);
}

// create a 3D Transformation Matrix
LibMath::Matrix4 LibMath::Matrix4::createTransform(Vector3 const& translation, Radian const& rotation, Vector3 const& scale)
{
	// create translation, rotation, and scale matrices
	Matrix4 translationMatrix = createTranslation(translation);
	Matrix4 rotationMatrix = createRotationZ(rotation); // Assuming rotation around Z-axis
	Matrix4 scaleMatrix = createScale(scale);

	// Combine matrices: T * R * S
	return translationMatrix * rotationMatrix * scaleMatrix;
}

// create a 3D Translation Matrix (Column-major)
LibMath::Matrix4 LibMath::Matrix4::createTranslation(Vector3 const& translation)
{
	return Matrix4
	(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translation.m_x, translation.m_y, translation.m_z, 1
	);
}

// create a 3D Rotation around X axis (Column-major)
LibMath::Matrix4 LibMath::Matrix4::createRotationX(Radian const& angle)
{
	float cosA = cos(angle);
	float sinA = sin(angle);

	return Matrix4
	(
		1, 0, 0, 0,
		0, cosA, sinA, 0,
		0, -sinA, cosA, 0,
		0, 0, 0, 1
	);
}

// create a 3D Rotation around Y axis (Column-major)
LibMath::Matrix4 LibMath::Matrix4::createRotationY(Radian const& angle)
{
	float cosA = cos(angle);
	float sinA = sin(angle);

	return Matrix4
	(
		cosA, 0, -sinA, 0,
		0, 1, 0, 0,
		sinA, 0, cosA, 0,
		0, 0, 0, 1
	);
}

// create a 3D Rotation around Z axis (Column-major)
LibMath::Matrix4 LibMath::Matrix4::createRotationZ(Radian const& angle)
{
	float cosA = cos(angle);
	float sinA = sin(angle);

	return Matrix4
	(
		cosA, sinA, 0, 0,
		-sinA, cosA, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

// create a 3D scale matrix
LibMath::Matrix4 LibMath::Matrix4::createScale(Vector3 const& scale)
{
	return Matrix4
	(
		scale.m_x, 0, 0, 0,
		0, scale.m_y, 0, 0,
		0, 0, scale.m_z, 0,
		0, 0, 0, 1
	);
}

void LibMath::Matrix4::Print() const
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t y = 0; y < 4; y++)
		{
			std::cout << " " << m_data[i][y];
		}

		std::cout << std::endl;
	}
}

// create a perspective Matrix
LibMath::Matrix4 LibMath::Matrix4::perspective(float fovY, float aspectRatio, float near, float far)
{
	// Ensure valid input
	if (fovY <= 0 || aspectRatio <= 0 || near <= 0 || far <= 0 || near >= far)
	{
		throw std::invalid_argument("Invalid arguments for perspective projection.");
	}

	// Compute the tangent of half the vertical field of view
	float tanHalfFovY = tan(Radian(fovY / 2.0f));

	// Initialize the perspective projection matrix
	Matrix4 result;

	// Set the elements of the perspective matrix
	result[0][0] = 1.0f / (aspectRatio * tanHalfFovY);
	result[1][1] = 1.0f / tanHalfFovY;
	result[2][2] = -(far + near) / (far - near);
	result[2][3] = -1.0f;
	result[3][2] = -(2.0f * far * near) / (far - near);
	result[3][3] = 0.0f;

	return result;
}

LibMath::Matrix4 LibMath::Matrix4::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	// Compute the forward direction (Z-axis) in a left-handed system
	Vector3 forward = (target - eye);
	forward.normalize();
	// Compute the right direction (X-axis)
	Vector3 right = up.cross(forward);
	right.normalize();
	// Compute the up direction (Y-axis)
	Vector3 cameraUp = forward.cross(right);

	// Construct the view matrix
	Matrix4 viewMatrix;
	viewMatrix.m_data[0][0] = -right.m_x;
	viewMatrix.m_data[0][1] = cameraUp.m_x;
	viewMatrix.m_data[0][2] = -forward.m_x;
	viewMatrix.m_data[0][3] = 0.0f;

	viewMatrix.m_data[1][0] = -right.m_y;
	viewMatrix.m_data[1][1] = cameraUp.m_y;
	viewMatrix.m_data[1][2] = -forward.m_y;
	viewMatrix.m_data[1][3] = 0.0f;

	viewMatrix.m_data[2][0] = -right.m_z;
	viewMatrix.m_data[2][1] = cameraUp.m_z;
	viewMatrix.m_data[2][2] = -forward.m_z;
	viewMatrix.m_data[2][3] = 0.0f;

	viewMatrix.m_data[3][0] = right.dot(eye);
	viewMatrix.m_data[3][1] = -cameraUp.dot(eye);
	viewMatrix.m_data[3][2] = forward.dot(eye);
	viewMatrix.m_data[3][3] = 1.0f;

	return viewMatrix;
}


// Identity Matrix
LibMath::Matrix4 LibMath::Matrix4::identity()
{
	return Matrix4(1.0f);
}


