#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>

Texture::Texture() : m_textureID(0) {}


Texture::~Texture()
{
	if (m_textureID)
	{
		glDeleteTextures(1, &m_textureID);
		m_textureID = 0;
	}
}
/// Load a texture from file
	bool Texture::loadTexture(std::filesystem::path const& filename)
	{
		// Check if the file exists
		if (m_textureID) 
		{
			glDeleteTextures(1, &m_textureID);
			m_textureID = 0;
		}
		m_path = filename;
		// Load the image
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(filename.string().c_str(), &width, &height, &nrChannels, 0);
		if (!data)
		{
			throw std::runtime_error("Failed to load texture: " + filename.string());
		}
		// Determine the format
		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		// Generate texture ID and bind it
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		// Set texture wrapping and filtering options
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Unbind the texture
		stbi_image_free(data);

		return true;
	}
