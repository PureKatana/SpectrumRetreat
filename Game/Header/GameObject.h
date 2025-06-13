#pragma once

#include "Physics/Collider.h"
#include "Color.h"
#include "ResourceManager.h"
#include <memory>

enum class GameObjectType
{
	OBSTACLE,
	DOOR,
	BUTTON,
	MOVING_OBJECT,
	END_POINT,
	DEATH_ZONE,
};

class GameObject
{
public:
	Mesh*								m_mesh = nullptr;
	std::unique_ptr<Physics::Collider>	m_collider = nullptr;
	ColorState							m_colorState = ColorState::E_INACTIVE;
	GameObjectType						m_type;

	// Constructor
	explicit GameObject(Mesh* mesh, std::unique_ptr<Physics::Collider> collider, ColorState colorState, GameObjectType type, ResourceManager& resourceManager)
		: m_mesh(mesh), m_collider(std::move(collider)), m_colorState(colorState), m_type(type), m_resourceManager(resourceManager)
	{
		m_collider->setGameObject(this);
		m_startTransform = mesh->getModelMatrix();
	}

	void setMeshTexture();
	void updateTransform(float deltaTime, const LibMath::Matrix4& endTransform);
	
private:
	ResourceManager&		m_resourceManager; // Reference to the resource manager for texture handling
	LibMath::Matrix4		m_startTransform;
	float					m_interpT = 0.0f;
	bool					m_goingToEnd = true;
};