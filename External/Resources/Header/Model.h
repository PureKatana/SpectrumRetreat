#pragma once

#include"LibMath//Vector.h"
#include <IResource.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <glad/glad.h>

struct Vertex {
    LibMath::Vector3    m_position;
    LibMath::Vector3    m_normal;
    LibMath::Vector2    m_uv;
};

class Model : public IResource {
public:
    Model() = default;
    ~Model() override;

    /// Load any polygonal OBJ (triangles, quads, n-gons) with automatic fan-triangulation
    bool loadFromOBJ(const std::string& filename);

    /// Sends vertex/index data once to the GPU
    void uploadToGPU();

    /// Draws using the currently bound shader
    void draw() const;

    const std::vector<Vertex>&   getVertices() const { return m_vertices; }
    const std::vector<uint32_t>& getIndices()  const { return m_indices;  }

private:
    // --- OBJ parsing helpers ---
    void parseVertexPosition(const std::string&        line,
                             std::vector<LibMath::Vector3>& outPositions);

    void parseVertexUV(const std::string&     line,
                       std::vector<LibMath::Vector2>& outUVs);

    void parseVertexNormal(const std::string&        line,
                           std::vector<LibMath::Vector3>& outNormals);

    void parseFaceVertex(
        const std::string&                         token,
        const std::vector<LibMath::Vector3>&       tempPositions,
        const std::vector<LibMath::Vector2>&       tempUVs,
        const std::vector<LibMath::Vector3>&       tempNormals,
        std::unordered_map<std::string,uint32_t>&  vertexMap,
        std::vector<Vertex>&                       outVertices,
        std::vector<uint32_t>&                     outIndices
    );

    /// Read all tokens on the face line, then fan-triangulate any n-gon
    void parseFace(
        const std::string&                         line,
        const std::vector<LibMath::Vector3>&       tempPositions,
        const std::vector<LibMath::Vector2>&       tempUVs,
        const std::vector<LibMath::Vector3>&       tempNormals,
        std::unordered_map<std::string,uint32_t>&  vertexMap,
        std::vector<Vertex>&                       outVertices,
        std::vector<uint32_t>&                     outIndices
    );

    // --- GPU buffer wrappers ---
    class Buffer {
    public:
        Buffer(GLenum type);
        ~Buffer();
        void bind()   const;
        void unbind() const;
        void setData(GLsizeiptr size, const void* data, GLenum usage);

    private:
        GLuint  m_id   = 0;
        GLenum  m_type;
    };

    class VertexAttributes {
    public:
        VertexAttributes();
        ~VertexAttributes();
        void bind()   const;
        void unbind() const;

    private:
        GLuint m_id = 0;
    };

    // --- Stored data ---
    std::vector<Vertex>      m_vertices;
    std::vector<uint32_t>    m_indices;

    VertexAttributes         m_vao;
    Buffer                   m_vbo{ GL_ARRAY_BUFFER };
    Buffer                   m_ebo{ GL_ELEMENT_ARRAY_BUFFER };

    bool                     m_isUploaded = false;
};