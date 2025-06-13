#include"Mesh.h"


Mesh::Mesh(Model* model, Texture* texture)
    : m_model(model)
    , m_texture(texture)
    , m_modelMatrix(LibMath::Matrix4::identity())
{}


void Mesh::draw(Shader* shader, const LibMath::Matrix4& viewProj) const
{
    // 1) Activate shader
    shader->use();

    // 2) Bind the mesh�s texture (or a default white if nullptr)
    glActiveTexture(GL_TEXTURE0);
    if (m_texture) {
        glBindTexture(GL_TEXTURE_2D, m_texture->getID());
    }
    else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // Tell the shader which texture unit �u_Texture� lives on:
    glUniform1i(
        glGetUniformLocation(shader->getID(), "u_Texture"),
        0
    );

    shader->setInt("u_Texture", 0);

    // 3) Compute & upload matrices
    auto mvp = viewProj * m_modelMatrix;
    GLint locMVP = glGetUniformLocation(shader->getID(), "uMVP");
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, mvp.getData());

    GLint locModel = glGetUniformLocation(shader->getID(), "uModel");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, m_modelMatrix.getData());

    // Normal matrix = inverse-transpose of model
    LibMath::Matrix4 normalMat = m_modelMatrix.inverse().transpose();
    GLint locNorm = glGetUniformLocation(shader->getID(), "uNormalMatrix");
    glUniformMatrix4fv(locNorm, 1, GL_FALSE, normalMat.getData());

    GLint opacity = glGetUniformLocation(shader->getID(), "u_opacity");
    glUniform1f(opacity, m_texture->getOpacity());

    // 4) Draw the underlying model
    m_model->draw();

    // 5) Unbind texture (optional)
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Mesh::loadInstances(
    const std::string& transformFilePath,
    std::unordered_multimap<int, Mesh*>& outMeshes,
    ResourceManager& manager)
{
    std::ifstream infile(transformFilePath);
    if (!infile.is_open()) {
        std::cerr << "Mesh::LoadInstances error: cannot open file: "
            << transformFilePath << "\n";
        return false;
    }

    // Temporary multimap to accumulate results
    std::unordered_multimap<int, Mesh*> tempMap;
    tempMap.reserve(128);

    std::string line;
    int        lineNumber = 0;

    while (std::getline(infile, line)) {
        ++lineNumber;

        // 1) Trim leading whitespace
        size_t i = 0;
        while (i < line.size() && std::isspace((unsigned char)line[i])) {
            ++i;
        }
        // 2) Skip blank lines
        if (i >= line.size()) {
            continue;
        }
        // 3) Skip comments (lines starting with '#')
        if (line[i] == '#') {
            continue;
        }

        // 4) Parse: id objectType textureName px py pz rx ry rz
        std::istringstream iss(line);
        int         id;
        std::string objectType, textureName;
        float       px, py, pz, rx, ry, rz, sx, sy, sz;
        float ppx, ppy, ppz;

        if (!(iss >> id >> objectType >> textureName >> px >> py >> pz >> rx >> ry >> rz >> sx >> sy >> sz)) {
            std::cerr << "Mesh::LoadInstances parse error at line "
                << lineNumber << ": expected format:\n"
                << "    <id> <objectType> <textureName> <px> <py> <pz> <rx> <ry> <rz>\n"
                << "  got: \"" << line << "\"\n";
            for (auto& kv : tempMap) {
                delete kv.second;
            }
            return false;
        }

        if (id >= 8 && id <= 11 && (iss >> ppx >> ppy >> ppz)) { /*moving cube endpoint*/ }

        // 5) Look up Model* by objectType
        Model* model = manager.get<Model>(objectType);
        if (!model) {
            std::cerr << "Mesh::LoadInstances error at line "
                << lineNumber << ": no Model named \""
                << objectType << "\" in ResourceManager\n";
            for (auto& kv : tempMap) {
                delete kv.second;
            }
            return false;
        }

        // 6) Look up Texture* by textureName (or allow "none" for no texture)
        Texture* texture = nullptr;
        if (textureName != "none") {
            texture = manager.get<Texture>(textureName);
            if (!texture) {
                std::cerr << "Mesh::LoadInstances error at line "
                    << lineNumber << ": no Texture named \""
                    << textureName << "\" in ResourceManager\n";
                for (auto& kv : tempMap) {
                    delete kv.second;
                }
                return false;
            }
        }

        // 7) Build the world‐space transform:
        Matrix4 worldMat = Matrix4::identity();
        worldMat = worldMat * Matrix4::createTranslation(Vector3(px, py, pz));
        worldMat = worldMat * Matrix4::createRotationX(Degree(rx));
        worldMat = worldMat * Matrix4::createRotationY(Degree(ry));
        worldMat = worldMat * Matrix4::createRotationZ(Degree(rz));
        worldMat = worldMat * Matrix4::createScale(LibMath::Vector3(sx, sy, sz));

        // 8) Create the new Mesh instance and insert into tempMap
        Mesh* mesh = new Mesh(model, texture);
        mesh->setModelMatrix(worldMat);
        tempMap.emplace(id, mesh);
    }

    // 9) Success: commit into outMeshes
    outMeshes.swap(tempMap);
    return true;
}