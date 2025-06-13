#pragma once

#include "IResource.h"
#include <string>
#include <filesystem>
#include <glad/glad.h>

class Shader : public IResource {
public:
    Shader();
    ~Shader();

    bool    setVertexShader(const std::string& filename);
    bool    setFragmentShader(const std::string& filename);
    bool    link();

    void    use() const;
    GLuint  getID() const;
	void    setInt(const std::string& name, int value) const;

private:
    GLuint              m_programID;
    std::vector<GLuint> m_attachedShaders;

    bool            compileShader(const std::string& source, GLenum shaderType, GLuint& shaderID);
    std::string     loadFile(const std::string& filepath);
};