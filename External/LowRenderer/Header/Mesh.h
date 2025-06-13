#pragma once

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "LibMath/Matrix/Matrix4.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>  
#include "LibMath/Angle.h"
#include "Mesh.h"
#include"ResourceManager.h"



using namespace LibMath;

class Mesh {
public:
    Mesh(Model* model, Texture* texture = nullptr);

    // Set which texture this mesh should use
    void    setTexture(Texture* texture) 
    { 
        m_texture = texture;
    }

    // Set the mesh’s model‐to‐world transform
    void    setModelMatrix(const LibMath::Matrix4& m) { m_modelMatrix = m; }

    // Draw with this shader and precomputed VP matrix
    void    draw(Shader* shader, const LibMath::Matrix4& viewProj) const;

    // Get the model this mesh is based on
    Model*  getModel() const { return m_model; }
    // Get the model matrix
    const LibMath::Matrix4& getModelMatrix() const { return m_modelMatrix; }
    // Get the texture used by this mesh
    Texture* getTexture() const { return m_texture; }

    /// ------------------------------
    /// Static loader: “Instanced Meshes”
    ///
    /// Reads `modelOBJPath` (a .obj file) into a single Model*, uploads it, then
    /// parses `transformFilePath` line by line. Each non‐empty, non‐comment line
    /// must contain exactly six floats:
    ///
    ///   px  py  pz   rx  ry  rz
    ///
    /// where (px,py,pz) is the position in world‐space, and (rx,ry,rz) are the
    /// Euler angles (in degrees) about X, Y, Z (apply X→Y→Z in that order).
    ///
    /// For every valid line, this will:
    ///   1) Create one new `Mesh*`, sharing the same `Model*`.
    ///   2) Optionally assign the given `texture` to it.
    ///   3) Compute `Mat = T(px,py,pz) * Rz(rz) * Ry(ry) * Rx(rx)` and call
    ///      `mesh->SetModelMatrix(Mat)`.
    ///   4) Push that `Mesh*` into `outMeshes`.
    ///
    /// Returns `true` if everything succeeded. On failure (cannot open .obj, or
    /// transform file missing/corrupt), returns `false`.  In that case, *no*
    /// meshes are left in `outMeshes` and any temporary Model* is cleaned up.
    ///
    /// Usage example:
    /// ```
    /// std::vector<Mesh*> levelMeshes;
    /// bool ok = Mesh::LoadInstances(
    ///     "Assets/Meshes/cube.obj",
    ///     "Assets/Levels/cubeLayout.txt",
    ///     myTexturePointer,
    ///     levelMeshes
    /// );
    /// if (!ok) {
    ///     // error loading .obj or parsing file
    /// }
    /// // else: levelMeshes now holds one Mesh* per line in cubeLayout.txt
    /// ```
    static bool loadInstances(
        const std::string& transformFilePath,
        std::unordered_multimap<int, Mesh*>& outMeshes,
        ResourceManager& manager
    );

private:
    Model*              m_model;
    Texture*            m_texture;
    LibMath::Matrix4    m_modelMatrix;
};