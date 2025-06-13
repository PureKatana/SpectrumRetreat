#ifndef LIBMATH_MATRIX_MATRIX3_H_
#define LIBMATH_MATRIX_MATRIX3_H_

#include "LibMath/Vector/Vector3.h"
#include "LibMath/Geometry2D.h"

namespace LibMath
{
	class Matrix3
	{
	public:
							Matrix3();														// Set all component to 0
		explicit			Matrix3(float diagonal);										// Set value in a diagonal from top-left to bottom-right
							Matrix3(float m00, float m01, float m02, float m10, float m11,	// Set all components individually
									float m12, float m20, float m21, float m22);
							Matrix3(Matrix3 const& other);									// Copy all components
							~Matrix3() = default;

		Matrix3&			operator=(Matrix3 const&) = default;

		float*				operator[](int index);
		const float*		operator[](int index) const;

		friend Matrix3		operator+(Matrix3 const& lhs, Matrix3 const& rhs);
		friend Matrix3		operator-(Matrix3 const& lhs, Matrix3 const& rhs);
		friend Matrix3		operator*(Matrix3 const& lhs, Matrix3 const& rhs);				// Column-based multiplication
		friend Matrix3		operator*(Matrix3 const& m3, float scalar);
		friend Vector3		operator*(Matrix3 const& m3, Vector3 const& vector);			// Column-based multiplication

		friend Matrix3&		operator+=(Matrix3& lhs, Matrix3 const& rhs);
		friend Matrix3&		operator-=(Matrix3& lhs, Matrix3 const& rhs);
		friend Matrix3&		operator*=(Matrix3& m3, float scalar);

		friend bool			operator==(Matrix3 const& lhs, Matrix3 const& rhs);
		friend bool			operator!=(Matrix3 const& lhs, Matrix3 const& rhs);


		Matrix3				transpose() const;												// Transpose the matrix
		float				determinant() const;											// Compute the determinant
		Matrix3				minors() const;													// Compute the matrix of minors
		Matrix3				cofactors() const;												// Compute the matrix of cofactors
		Matrix3				adjugate() const;												// Compute the adjugate matrix
		Matrix3				inverse() const;												// Compute the inverse matrix

		// create a 2D transform matrix
		static Matrix3		createTransform(Vector2 const& translation, Radian const& rotation, Vector2 const& scale);	 
		// create a 3D transform matrix
		static Matrix3		createTransform(Radian const& rotation, Vector3 const& scale);								 
		static Matrix3		createTranslation(Vector2 const& translation);												// create a translation matrix (Column-major)
		static Matrix3		createRotation(Point2D const& center, Radian const& angle);									// create a 2D rotation matrix around a point
		static Matrix3		createRotationX(Radian const& angle);														// create a 3D rotation matrix around the X-axis (Column-major)
		static Matrix3		createRotationY(Radian const& angle);														// create a 3D rotation matrix around the Y-axis (Column-major)
		static Matrix3		createRotationZ(Radian const& angle);														// create a 3D rotation matrix around the Z-axis (Column-major)
		static Matrix3		createScale(Vector2 const& scale);															// create a 2D scale matrix
		static Matrix3		createScale(Vector3 const& scale);															// create a 3D scale matrix
		static Matrix3		identity();																					// Identity matrix

	private:
		float	m_data[3][3];
	};
}

#endif // !LIBMATH_MATRIX_MATRIX3_H_
