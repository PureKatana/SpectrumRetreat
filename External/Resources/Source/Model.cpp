#include "Model.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

// --- Model destructor ---
Model::~Model() = default;

// --- GPU Buffer implementations ---
Model::Buffer::Buffer(GLenum type) : m_type(type) {
    glGenBuffers(1, &m_id);
}
Model::Buffer::~Buffer() {
    if (m_id) glDeleteBuffers(1, &m_id);
}
void Model::Buffer::bind() const {
    glBindBuffer(m_type, m_id);
}
void Model::Buffer::unbind() const {
    glBindBuffer(m_type, 0);
}
void Model::Buffer::setData(GLsizeiptr size, const void* data, GLenum usage) {
    glBindBuffer(m_type, m_id);
    glBufferData(m_type, size, data, usage);
}

// --- VertexAttributes implementations ---
Model::VertexAttributes::VertexAttributes() {
    glGenVertexArrays(1, &m_id);
}
Model::VertexAttributes::~VertexAttributes() {
    if (m_id) glDeleteVertexArrays(1, &m_id);
}
void Model::VertexAttributes::bind() const {
    glBindVertexArray(m_id);
}
void Model::VertexAttributes::unbind() const {
    glBindVertexArray(0);
}

// --- uploadToGPU & draw ---
void Model::uploadToGPU() {
    if (m_isUploaded || m_vertices.empty() || m_indices.empty()) return;

    m_vao.bind();
    m_vbo.setData(
        GLsizeiptr(m_vertices.size() * sizeof(Vertex)),
        m_vertices.data(),
        GL_STATIC_DRAW
    );
    m_ebo.setData(
        GLsizeiptr(m_indices.size() * sizeof(uint32_t)),
        m_indices.data(),
        GL_STATIC_DRAW
    );

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        GLsizei(sizeof(Vertex)),
        (void*)offsetof(Vertex, m_position)
    );

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        GLsizei(sizeof(Vertex)),
        (void*)offsetof(Vertex, m_normal)
    );

    // UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        GLsizei(sizeof(Vertex)),
        (void*)offsetof(Vertex, m_uv)
    );

    m_vao.unbind();
    m_isUploaded = true;
}

void Model::draw() const {
    if (!m_isUploaded) return;
    m_vao.bind();
    glDrawElements(
        GL_TRIANGLES,
        GLsizei(m_indices.size()),
        GL_UNSIGNED_INT,
        nullptr
    );
    m_vao.unbind();
}

// --- loadFromOBJ ---
bool Model::loadFromOBJ(const std::string & filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filename << "\n";
        return false;
    }
    std::vector<LibMath::Vector3> tempPositions;
    std::vector<LibMath::Vector2> tempUVs;
    std::vector<LibMath::Vector3> tempNormals;
    std::unordered_map<std::string, uint32_t> vertexMap;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            parseVertexPosition(line, tempPositions);
        }
        else if (prefix == "vt") {
            parseVertexUV(line, tempUVs);
        }
        else if (prefix == "vn") {
            parseVertexNormal(line, tempNormals);
        }
        else if (prefix == "f") {
            parseFace(line,
                tempPositions, tempUVs, tempNormals,
                vertexMap, m_vertices, m_indices);
        }
    }
    return true;
}

// --- parsing helpers ---
void Model::parseVertexPosition(
    const std::string & line,
    std::vector<LibMath::Vector3>&outPositions
) {
    std::istringstream iss(line);
    std::string tag;
    LibMath::Vector3 v;
    iss >> tag >> v.m_x >> v.m_y >> v.m_z;
    outPositions.push_back(v);
}

void Model::parseVertexUV(
    const std::string & line,
    std::vector<LibMath::Vector2>&outUVs
) {
    std::istringstream iss(line);
    std::string tag;
    LibMath::Vector2 uv;
    iss >> tag >> uv.m_x >> uv.m_y;
    outUVs.push_back(uv);
}

void Model::parseVertexNormal(
    const std::string & line,
    std::vector<LibMath::Vector3>&outNormals
) {
    std::istringstream iss(line);
    std::string tag;
    LibMath::Vector3 n;
    iss >> tag >> n.m_x >> n.m_y >> n.m_z;
    outNormals.push_back(n);
}

void Model::parseFaceVertex(
    const std::string & token,
    const std::vector<LibMath::Vector3>&tempPositions,
    const std::vector<LibMath::Vector2>&tempUVs,
    const std::vector<LibMath::Vector3>&tempNormals,
    std::unordered_map<std::string, uint32_t>&vertexMap,
    std::vector<Vertex>&outVertices,
    std::vector<uint32_t>&outIndices
) {
    auto it = vertexMap.find(token);
    if (it != vertexMap.end()) {
        outIndices.push_back(it->second);
        return;
    }

    std::istringstream vss(token);
    std::string vi_s, ti_s, ni_s;
    std::getline(vss, vi_s, '/');
    std::getline(vss, ti_s, '/');
    std::getline(vss, ni_s, '/');

    int vi = vi_s.empty() ? 0 : std::stoi(vi_s);
    int ti = ti_s.empty() ? 0 : std::stoi(ti_s);
    int ni = ni_s.empty() ? 0 : std::stoi(ni_s);

    // convert 1-based or negative
    if (vi < 0) vi = int(tempPositions.size()) + vi;
    else         vi -= 1;
    if (ti < 0) ti = int(tempUVs.size()) + ti;
    else         ti -= 1;
    if (ni < 0) ni = int(tempNormals.size()) + ni;
    else         ni -= 1;

    Vertex vert{};
    if (vi >= 0 && vi < int(tempPositions.size()))
        vert.m_position = tempPositions[vi];
    if (ti >= 0 && ti < int(tempUVs.size()))
        vert.m_uv = tempUVs[ti];
    if (ni >= 0 && ni < int(tempNormals.size()))
        vert.m_normal = tempNormals[ni];

    uint32_t newIndex = uint32_t(outVertices.size());
    outVertices.push_back(vert);
    outIndices.push_back(newIndex);
    vertexMap[token] = newIndex;
}

void Model::parseFace(
    const std::string & line,
    const std::vector<LibMath::Vector3>&tempPositions,
    const std::vector<LibMath::Vector2>&tempUVs,
    const std::vector<LibMath::Vector3>&tempNormals,
    std::unordered_map<std::string, uint32_t>&vertexMap,
    std::vector<Vertex>&outVertices,
    std::vector<uint32_t>&outIndices
) {
    std::istringstream iss(line);
    std::string tag;
    iss >> tag; // skip "f"

    std::vector<std::string> tokens;
    std::string tok;
    while (iss >> tok)
        tokens.push_back(tok);

    if (tokens.size() < 3) return;

    // fan-triangulate
    for (size_t i = 1; i + 1 < tokens.size(); ++i) {
        parseFaceVertex(tokens[0], tempPositions, tempUVs, tempNormals, vertexMap, outVertices, outIndices);
        parseFaceVertex(tokens[i], tempPositions, tempUVs, tempNormals, vertexMap, outVertices, outIndices);
        parseFaceVertex(tokens[i + 1], tempPositions, tempUVs, tempNormals, vertexMap, outVertices, outIndices);
    }
}