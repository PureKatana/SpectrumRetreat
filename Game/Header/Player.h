#pragma once

#include "Camera.h"
#include "Physics/Collider.h"
#include "imgui.h"
#include "Phone.h"

using namespace Physics;

class Player 
{
public:
    Player();
	~Player() = default;

    void                reset();

    void                handleInput(GLFWwindow* window, float deltaTime, const std::vector<Collider*>& colliders);
    void                updatePosition(float deltaTime);
    void                updateCamera();
    void                performRaycast(GLFWwindow* window, const std::vector<Collider*>& colliders);

    void                setCamera(Camera* camera);
    void                setCollider(std::unique_ptr<Collider> collider);
    void                AddVelocity(const Vector3& velocity);
    const Collider*     getCollider() const { return m_collider.get(); }

    // Getters for position/orientation if needed
    const Vector3&          getPosition() const { return m_position; }
    const Vector3&          getVelocity() const { return m_velocity; }
    const LibMath::Radian&  getYaw() const { return m_yaw; }
    const LibMath::Radian&  getPitch() const { return m_pitch; }
    const Phone&            getPhone()const { return m_phone; }
	void                    setPhoneColorState(const ColorState& state) { m_phone.setState(state); }

    std::optional<LibMath::Line3D>  getActiveRay() const { return m_activeRay; }

    bool m_grounded = false;
    float m_height = 1.0f;
    float m_radius = 0.2f;

private:

    void    applyGravity(Vector3& velocity, float deltaTime);
    void    updateCollider();

    Vector3                 m_position{ 0.0f, 0.0f, 0.0f };
    Vector3                 m_velocity{ 0.0f, 0.0f, 0.0f };
    LibMath::Radian         m_yaw{ 0.0f };
    LibMath::Radian         m_pitch{ 0.0f };

    float                   m_moveSpeed = 2.5f;
    float                   m_mouseSensitivity = 0.001f;

    float                   m_lastX = 0.0f;
    float                   m_lastY = 0.0f;
    bool                    m_firstMouse = true;

    Camera*                 m_camera = nullptr;

    Phone                   m_phone;
    
    std::unique_ptr<Collider>       m_collider = nullptr;

    bool                            m_spaceDown = false;
    bool                            m_clickDown = false;

    float                           m_gravity = 9.81f;

    float                           m_maxFallSpeed = 50.f;

    float                           m_jumpForce = 5.f;

    std::optional<LibMath::Line3D>  m_activeRay;
};