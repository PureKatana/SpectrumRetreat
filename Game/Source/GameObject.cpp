#include "GameObject.h"
#include "LibMath/Arithmetic.h"

void GameObject::setMeshTexture()
{
	if (m_mesh)
	{
		switch (m_colorState)
	    {
			case ColorState::E_INACTIVE:
			{
				Texture* textureInactive = m_resourceManager.get<Texture>("gray_color");
				m_mesh -> setTexture(textureInactive);
				break;
			}
			case ColorState::E_RED:
			{
				Texture* textureRed = m_resourceManager.get<Texture>("red_color");
				m_mesh -> setTexture(textureRed);
				break;
			}
			case ColorState::E_BLUE:
			{
				Texture* textureBlue = m_resourceManager.get<Texture>("blue_color");
				m_mesh -> setTexture(textureBlue);
				break;
			}
			case ColorState::E_YELLOW:
			{
				Texture* textureYellow = m_resourceManager.get<Texture>("yellow_color");
				m_mesh -> setTexture(textureYellow);
				break;
			}
			default:
			{
				Texture* textureDefault = m_resourceManager.get<Texture>("gray_color");
				m_mesh -> setTexture(textureDefault);
				break;
			}
		}
	}
}

void GameObject::updateTransform(float deltaTime, const LibMath::Matrix4& endTransform)
{
	if (m_mesh && m_collider)
	{
		// Update t based on direction
		const float speed = 0.25f; // Adjust speed as needed
		if (m_goingToEnd)
			m_interpT += deltaTime * speed;
		else
			m_interpT -= deltaTime * speed;

		// Clamp and reverse direction if needed
		if (m_interpT >= 1.0f)
		{
			m_interpT = 1.0f;
			m_goingToEnd = false;
		}
		else if (m_interpT <= 0.0f)
		{
			m_interpT = 0.0f;
			m_goingToEnd = true;
		}

		// Interpolate between start and end
		LibMath::Matrix4 interpolatedTransform = m_startTransform * (1.0f - m_interpT) + endTransform * m_interpT;

		// Update mesh transform
		m_mesh -> setModelMatrix(interpolatedTransform);

		m_collider -> updateBounds();
	}
}
