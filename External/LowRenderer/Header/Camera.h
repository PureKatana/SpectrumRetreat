#pragma once

#include"glad/glad.h"
#include <GLFW/glfw3.h>
#include "LibMath/Vector/Vector3.h"
#include "LibMath/Matrix/Matrix4.h"
#include "LibMath/Constants.h"
#include "LibMath/Angle.h"

using namespace LibMath;

class Camera {
public:
    Camera(const Vector3& eye,
        const Vector3& center,
        const Vector3& up,
        float fovYRadians,
        float aspect,
        float nearPlane,
        float farPlane);

    //void            handleInput(GLFWwindow* window, float deltaTime);
	void            setTransform(const Vector3& position, const Radian& yaw, const Radian& pitch);
    const Matrix4&  getViewMatrix() const;
    const Matrix4&  getProjectionMatrix() const;
    Vector3         getPosition() const { return m_position; }
    Vector3         getForward() const { return m_front; }

private:
    void updateCameraVectors();

    Vector3             m_position;
    Vector3             m_front;
    Vector3             m_worldUp;
    Vector3             m_up;
    Vector3             m_right;
    LibMath::Radian     m_yaw;
    LibMath::Radian     m_pitch;

    float               m_moveSpeed = 5.0f;
    float               m_mouseSensitivity = 0.1f;

    bool                m_firstMouse = true;
    float               m_lastX = 0.0f;
    float               m_lastY = 0.0f;

    Matrix4             m_view;
    Matrix4             m_projection;
};