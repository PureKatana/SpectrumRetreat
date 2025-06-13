#include "Camera.h"
#include "LibMath/Trigonometry.h"

Camera::Camera(const Vector3& eye,
    const Vector3& center,
    const Vector3& up,
    float fovYRadians,
    float aspect,
    float nearPlane,
    float farPlane)
    : m_position(eye),
    m_worldUp(up)
{
    Vector3 dir = center - eye;
    dir.normalize();
    m_yaw = LibMath::atan(dir.m_z, dir.m_x);
    m_pitch = LibMath::asin(dir.m_y);
    m_projection = Matrix4::perspective(fovYRadians, aspect, nearPlane, farPlane);
    updateCameraVectors();
    m_view = Matrix4::lookAt(m_position, m_position + m_front, m_up);
}

// Handle input for the camera
//void Camera::handleInput(GLFWwindow* window, float deltaTime)
//{
//    float velocity = m_moveSpeed * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        m_position += m_front * velocity;
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        m_position -= m_front * velocity;
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        m_position -= m_right * velocity;
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        m_position += m_right * velocity;
//    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
//        m_position += m_worldUp * velocity;
//    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
//        m_position -= m_worldUp * velocity;
//
//	// Mouse movement
//    double xpos, ypos;
//    glfwGetCursorPos(window, &xpos, &ypos);
//    if (m_firstMouse) 
//    {
//        m_lastX = static_cast<float>(xpos);
//        m_lastY = static_cast<float>(ypos);
//        m_firstMouse = false;
//    }
//    float xoffset = static_cast<float>(xpos) - m_lastX;
//    float yoffset = m_lastY - static_cast<float>(ypos);
//    m_lastX = static_cast<float>(xpos);
//    m_lastY = static_cast<float>(ypos);
//
//    xoffset *= m_mouseSensitivity;
//    yoffset *= m_mouseSensitivity;
//
//    m_yaw += LibMath::Degree(xoffset);
//    m_pitch += LibMath::Degree(yoffset);
//
//    // Convert to degrees for clamping, then back to radians
//    /*float pitchDegrees = m_pitch.degree();
//    if (pitchDegrees > 89.0f) m_pitch = LibMath::Degree(89.0f);
//    if (pitchDegrees < -89.0f) m_pitch = LibMath::Degree(-89.0f);*/
//
//    updateCameraVectors();
//    m_view = Matrix4::lookAt(m_position, m_position + m_front, m_up);
//}

void Camera::setTransform(const Vector3& position, const Radian& yaw, const Radian& pitch)
{
    m_position = position;
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
    m_view = Matrix4::lookAt(m_position, m_position + m_front, m_up);
}

const Matrix4& Camera::getViewMatrix() const
{
    return m_view;
}

const Matrix4& Camera::getProjectionMatrix() const
{
    return m_projection;
}

// Update the camera's front, right, and up vectors based on yaw and pitch
void Camera::updateCameraVectors()
{
    Vector3 front;
    front.m_x = LibMath::cos(m_yaw) * LibMath::cos(m_pitch);
    front.m_y = LibMath::sin(m_pitch);
    front.m_z = LibMath::sin(m_yaw) * LibMath::cos(m_pitch);
    front.normalize();

    m_front = front;
    m_right = m_front.cross(m_worldUp);
    m_right.normalize();
    m_up = m_right.cross(m_front);
    m_up.normalize();
}
