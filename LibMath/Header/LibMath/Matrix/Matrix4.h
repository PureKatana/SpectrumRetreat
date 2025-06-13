#ifndef LIBMATH_MATRIX_MATRIX4_H_
#define LIBMATH_MATRIX_MATRIX4_H_

#include "LibMath/Vector/Vector3.h"

namespace LibMath
{
	class Matrix4
	{
	public:
                             Matrix4();                                              // Set all components to 0
        explicit             Matrix4(float diagonal);                                // Set value in a diagonal from top-left to bottom-right
                             Matrix4(float m00, float m01, float m02, float m03,
                                     float m10, float m11, float m12, float m13,
                                     float m20, float m21, float m22, float m23,
                                     float m30, float m31, float m32, float m33);    // Set all components individually
                             Matrix4(Matrix4 const& other);                          // Copy all components
                            ~Matrix4() = default;

        Matrix4&            operator=(Matrix4 const&) = default;

        float*              operator[](int index);
        const float*        operator[](int index) const;

        friend Matrix4      operator+(Matrix4 const& lhs, Matrix4 const& rhs);
        friend Matrix4      operator-(Matrix4 const& lhs, Matrix4 const& rhs);
        friend Matrix4      operator*(Matrix4 const& lhs, Matrix4 const& rhs);      // Column-based multiplication
        friend Matrix4      operator*(Matrix4 const& m4, float scalar);

        friend Matrix4&     operator+=(Matrix4& lhs, Matrix4 const& rhs);
        friend Matrix4&     operator-=(Matrix4& lhs, Matrix4 const& rhs);
        friend Matrix4&     operator*=(Matrix4& m4, float scalar);

        friend bool         operator==(Matrix4 const& lhs, Matrix4 const& rhs);
        friend bool         operator!=(Matrix4 const& lhs, Matrix4 const& rhs);

		const float*        getData() const { return &m_data[0][0]; }               // Get the raw data of the matrix
        

        Matrix4             transpose() const;                                      // Transpose the matrix
        float               determinant() const;                                    // Compute the determinant
        Matrix4             minors() const;                                         // Compute the matrix of minors
        Matrix4             cofactors() const;                                      // Compute the matrix of cofactors
        Matrix4             adjugate() const;                                       // Compute the adjugate matrix
        Matrix4             inverse() const;                                        // Compute the inverse matrix
        void                Print() const;

        static Matrix4      createTransform(Vector3 const& translation, Radian const& rotation, Vector3 const& scale);  // create a 3D transform matrix
        static Matrix4      createTranslation(Vector3 const& translation);                                              // create a 3D translation matrix (Column-major)
        static Matrix4      createRotationX(Radian const& angle);                                                       // create a 3D rotation matrix around the X-axis (Column-major)
        static Matrix4      createRotationY(Radian const& angle);                                                       // create a 3D rotation matrix around the Y-axis (Column-major)
        static Matrix4      createRotationZ(Radian const& angle);                                                       // create a 3D rotation matrix around the Z-axis (Column-major)
        static Matrix4      createScale(Vector3 const& scale);                                                          // create a 3D scale matrix
        static Matrix4      perspective(float fovY, float aspectRatio, float near, float far);                          // create a perspective Matrix
        static Matrix4      lookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
        static Matrix4      identity();                                                                                 // Identity matrix


    private:
        float m_data[4][4];
	};
}

#ifdef LIBMATH_VECTOR_VECTOR4_H_
#include "LibMath/Matrix4Vector4Operation.h"
#endif // LIBMATH_MATRIX_H_

#endif // !LIBMATH_MATRIX_MATRIX4_H_
