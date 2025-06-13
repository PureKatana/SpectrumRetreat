#include "LibMath/Matrix4Vector4Operation.h"

LibMath::Vector4 LibMath::operator*(Matrix4 const& m4, Vector4 const& vector)
{
    float x = m4[0][0] * vector.m_x + m4[1][0] * vector.m_y + m4[2][0] * vector.m_z + m4[3][0] * vector.m_w;
    float y = m4[0][1] * vector.m_x + m4[1][1] * vector.m_y + m4[2][1] * vector.m_z + m4[3][1] * vector.m_w;
    float z = m4[0][2] * vector.m_x + m4[1][2] * vector.m_y + m4[2][2] * vector.m_z + m4[3][2] * vector.m_w;
    float w = m4[0][3] * vector.m_x + m4[1][3] * vector.m_y + m4[2][3] * vector.m_z + m4[3][3] * vector.m_w;

    return Vector4(x, y, z, w);
}
