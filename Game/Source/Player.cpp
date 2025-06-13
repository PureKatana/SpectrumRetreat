#include "Player.h"
#include "GameObject.h"
#include "LibMath/Trigonometry.h"
#include "Audio_Manager.h"

Player::Player()
{
    LibMath::Point3D p1(0.0f, 0.0f, 0.0f);
    LibMath::Point3D p2(0.0f, m_height, 0.0f);

	setCollider(Collider::createCapsuleManualSet(p1, p2, m_radius));
}

void Player::reset()
{
    // Reset core player properties
    m_position = LibMath::Vector3(0.0f, 0.0f, 0.0f); 
    m_velocity = LibMath::Vector3(0.0f, 0.0f, 0.0f);
    m_grounded = false;

    // Reset input/camera related flags
    m_firstMouse = true;
    m_lastX = 0.0f; // These will be correctly updated on the next mouse input due to m_firstMouse
    m_lastY = 0.0f;
    m_yaw = LibMath::Radian(LibMath::Degree(-90)); // Reset player's horizontal orientation
    m_pitch = LibMath::Radian(0.0f); // Reset player's vertical orientation
    m_spaceDown = false;
    m_clickDown = false;

    // --- Reset the player's unique_ptr collider ---
    LibMath::Point3D p1(m_position.m_x, m_position.m_y, m_position.m_z);
    LibMath::Point3D p2(m_position.m_x, m_position.m_y + m_height, m_position.m_z);

    setCollider(Physics::Collider::createCapsuleManualSet(p1, p2, m_radius));

    m_phone.setState(ColorState::E_INACTIVE);
}

void Player::handleInput(GLFWwindow* window, float deltaTime, const std::vector<Collider*>& colliders)
{
    using namespace LibMath;

    // --- 1) Mouse look (radians) ---
    double dx_d, dy_d;
    glfwGetCursorPos(window, &dx_d, &dy_d);
    float xpos = static_cast<float>(dx_d);
    float ypos = static_cast<float>(dy_d);

    if (m_firstMouse)
    {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
    }

    float dx = xpos - m_lastX;
    float dy = m_lastY - ypos;
    m_lastX = xpos;
    m_lastY = ypos;

    // apply sensitivity (radians per pixel)
    m_yaw +=Radian( dx * m_mouseSensitivity);
    m_pitch += Radian(dy * m_mouseSensitivity);

    // clamp pitch to ±89°
    const float maxPitch = Degree(89).radian();
    if (m_pitch.raw() > maxPitch) m_pitch = Radian(maxPitch);
    if (m_pitch.raw() < -maxPitch) m_pitch = Radian(-maxPitch);

    // --- 2) Build view‐front (full 3D) and move‐front (flat) ---
    Vector3 viewFront;
    viewFront.m_x = cos(Radian(m_yaw)) * cos(Radian(m_pitch));
    viewFront.m_y = sin(Radian(m_pitch));
    viewFront.m_z = sin(Radian(m_yaw)) * cos(Radian(m_pitch));
    viewFront.normalize();

    Vector3 moveFront = viewFront;
    moveFront.m_y = 0.0f;      // lock Y for ground movement
    moveFront.normalize();

    Vector3 right = moveFront.cross(Vector3::up());
    right.normalize();

    // --- 3) Keyboard movement ---

    m_velocity.m_x = 0;
    m_velocity.m_z = 0;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_velocity += moveFront * m_moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_velocity -= moveFront * m_moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_velocity += -right * m_moveSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_velocity += right * m_moveSpeed;

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        m_spaceDown = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (!m_spaceDown && m_grounded)
        {
            m_velocity.m_y += m_jumpForce;
            Audio_Manager::getInstance()->playSound("../../Assets/Sounds/SFX/Jump.wav", false);
            m_spaceDown = true;
        }

    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		m_clickDown = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (!m_clickDown)
        {
            performRaycast(window, colliders);
        }
        m_clickDown = true;
    }

    applyGravity(m_velocity, deltaTime);

    if (m_grounded && m_velocity.m_y < 0.0f)
    {
        // if grounded, reset vertical velocity
        m_velocity.m_y = 0.0f;
	}
}


void Player::updatePosition(float deltaTime)
{
    m_position += m_velocity * deltaTime;
    updateCamera();
    updateCollider();
}

void Player::applyGravity(Vector3& velocity, float deltaTime)
{
    velocity += Vector3(0, -m_gravity * deltaTime, 0);
    velocity.m_y = std::clamp(velocity.m_y, -m_maxFallSpeed, 100.f);
}

void Player::AddVelocity(const Vector3& velocity)
{
    m_velocity += velocity;
}

void Player::updateCollider()
{
    if (!m_collider) return;

    if (m_collider -> getType() == ColliderType::CAPSULE)
    {
        CapsuleCollider* capsuleCollider = static_cast<CapsuleCollider*>(m_collider.get());

        LibMath::Point3D p1(m_position.m_x, m_position.m_y, m_position.m_z);
        LibMath::Point3D p2(m_position.m_x, m_position.m_y + m_height, m_position.m_z);

        capsuleCollider -> updateCapsule(p1, p2, m_radius);
    }
}

void Player::updateCamera()
{
    if (m_camera)
        m_camera -> setTransform(m_position + Vector3(0, m_height * 0.5, 0) , m_yaw, m_pitch);
}

void Player::performRaycast(GLFWwindow* window, const std::vector<Collider*>& colliders)
{
    // 1. Get camera position and forward direction
    Vector3 origin = m_camera->getPosition();
    Vector3 forward = m_camera->getForward();

    // 2. Construct a ray (Line3D)
    Point3D originPoint(origin.m_x, origin.m_y, origin.m_z);
    LibMath::Line3D ray(originPoint, forward);

    m_activeRay = ray;

    // 3. Perform performRaycast
    auto hit = Physics::PhysicsManager::Raycast(ray, colliders);

    if (hit && hit->m_collider != m_collider.get())
    {
        if (hit->m_collider -> getGameObject() -> m_type == GameObjectType::BUTTON || 
            hit->m_collider -> getGameObject() -> m_type == GameObjectType::MOVING_OBJECT)
        {
            m_phone.swapColorState(hit->m_collider -> getGameObject() -> m_colorState);
            hit->m_collider -> getGameObject() -> setMeshTexture();
			Audio_Manager::getInstance() -> playSound("../../Assets/Sounds/SFX/Ding.wav", false);
        }
    }
    else
    {
        std::cout << "Raycast missed.\n";
    }
}

void Player::setCamera(Camera* camera)
{
    m_camera = camera;
}

void Player::setCollider(std::unique_ptr<Collider> collider)
{
    m_collider = std::move(collider);
}
