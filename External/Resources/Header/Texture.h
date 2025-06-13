#pragma once

#include <IResource.h>
#include <filesystem>
#include <glad/glad.h>

class Texture : public IResource
{
public:
	Texture();
	~Texture();

	GLuint							getID() const { return m_textureID; }
	bool							loadTexture(std::filesystem::path const& filename);
	const std::filesystem::path&	getPath() const { return m_path; }
	void setOpacity(float opacity)
	{
		m_opacity = opacity;
	}
	float getOpacity()
	{
		return m_opacity;
	}
private:
	GLuint							m_textureID;
	std::filesystem::path			m_path;
	float m_opacity = 1;
};