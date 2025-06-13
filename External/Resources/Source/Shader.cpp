#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() 
{
    m_programID = glCreateProgram();
    std::cout << "Created program ID=" << m_programID << "\n";
}

Shader::~Shader() 
{
    for (auto shader : m_attachedShaders) 
    {
        glDeleteShader(shader);
    }
    glDeleteProgram(m_programID);
}

// Load a file into a string
std::string Shader::loadFile(const std::string& filepath) 
{
    std::ifstream file(filepath);
    if (!file) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Compile a shader from source code
bool Shader::compileShader(const std::string& source, GLenum shaderType, GLuint& shaderID) 
{
	// Check if shader type is valid
    const char* src = source.c_str();
    shaderID = glCreateShader(shaderType);
    std::cout << "[Shader] glCreateShader → shaderID=" << shaderID << "\n";

    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);
	// Check for compile errors
    GLint status = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buf[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, buf);
        std::cerr << "Compile error:\n" << buf << "\n";
        return false;
    }

    // Attach
    glAttachShader(m_programID, shaderID);

    // **immediately** check how many shaders are attached now
    {
        GLint count = 0;
        glGetProgramiv(m_programID, GL_ATTACHED_SHADERS, &count);
        std::cout << "[Shader] after attach, ATTACHED_SHADERS=" << count << "\n";
    }

    m_attachedShaders.push_back(shaderID);
    std::cout << "[Shader] CompileShader done\n";
    return true;
}

// Set the vertex shader from a file
bool Shader::setVertexShader(const std::string& filename) 
{
    std::string source = loadFile(filename);
    std::cout << "GL version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "Shader source length: " << source.size() << "\n";
    std::cout << "First line: " << source.substr(0, source.find('\n')) << "\n";
    if (source.empty()) return false;
    GLuint id;
    std::cout << "compiling vertex shader" << std::endl;
    return compileShader(source, GL_VERTEX_SHADER, id);
}

// Set the fragment shader from a file
bool Shader::setFragmentShader(const std::string& filename) 
{
    std::string source = loadFile(filename);
    if (source.empty()) return false;
    GLuint id;
    return compileShader(source, GL_FRAGMENT_SHADER, id);
}

// Set the geometry shader from a file
bool Shader::link() 
{
    // Check again before linking
    GLint count = 0;
    glGetProgramiv(m_programID, GL_ATTACHED_SHADERS, &count);
    std::cout << "[Shader] before link, ATTACHED_SHADERS=" << count
        << "  programID=" << m_programID << "\n";
    if (count == 0) 
    {
        std::cerr << "Shader::Link() error: no attached shader objects\n";
        return false;
    }

	// Link the program
    glLinkProgram(m_programID);
    GLint success = 0;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
        std::cerr << "Shader program link failed:\n" << infoLog << std::endl;
        return false;
    }
    std::cout << "shaders linking success" << std::endl;
    return true;
}

// Set the shader program to be used
void Shader::use() const 
{
    glUseProgram(m_programID);
}

GLuint Shader::getID() const 
{
    return m_programID;
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
}
