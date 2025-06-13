#ifndef LIBMATH_MATRIX_MATRIX2_H_
#define LIBMATH_MATRIX_MATRIX2_H_

#include "LibMath/Geometry2D.h"

namespace LibMath
{
	class Matrix2
	{
	public:
							Matrix2();													// Set all component to 0
		explicit			Matrix2(float diagonal);									// Set value in a diagonal from top-left to bottom-right
							Matrix2(float m00, float m01, float m10, float m11);		// Set all components individually
							Matrix2(Matrix2 const& other);								// Copy all components
							~Matrix2() = default;

		Matrix2&			operator=(Matrix2 const&) = default;

		float*				operator[](int index);										// Access element 
		const float*		operator[](int index) const;								// Access element
			
		friend Matrix2		operator+(Matrix2 const& lhs, Matrix2 const& rhs);
		friend Matrix2		operator-(Matrix2 const& lhs, Matrix2 const& rhs);
		friend Matrix2		operator*(Matrix2 const& lhs, Matrix2 const& rhs);			// Column-based multiplication like GLM
		friend Matrix2		operator*(Matrix2 const& m2, float scalar);
		friend Vector2		operator*(Matrix2 const& m2, Vector2 const& vector);		// Column-based multiplication like GLM

		friend Matrix2&		operator+=(Matrix2& lhs, Matrix2 const& rhs);
		friend Matrix2&		operator-=(Matrix2& lhs, Matrix2 const& rhs);
		friend Matrix2&		operator*=(Matrix2& m2, float scalar);

		friend bool			operator==(Matrix2 const& lhs, Matrix2 const& rhs);
		friend bool			operator!=(Matrix2 const& lhs, Matrix2 const& rhs);

		Matrix2				tranpose() const;											// Return a tranposed Matrix	
		float				determinant() const;										// Return determinant of Matrix
		Matrix2				minors() const;												// Return minors matrix
		Matrix2				cofactors() const;											// Return cofactors matrix
		Matrix2				adjugate() const;											// Return adjugate matrix
		Matrix2				inverse() const;											// Return inverse matrix

		static Matrix2		createTransform(Radian const& rotation, Vector2 const& scale);		// create a transform matrix(column-major)
		static Matrix2		createRotation(Radian const& angle);								// create a rotation matrix(column-major)
		static Matrix2		createScale(Vector2 const& scale);									// create a scale matrix
		static Matrix2		identity();															// Identity matrix
		
	private:
		float	m_data[2][2];
	};
}

#endif // !LIBMATH_MATRIX_MATRIX2_H_