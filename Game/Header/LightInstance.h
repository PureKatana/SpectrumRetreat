#pragma once

#include "Light.h"
#include "LibMath/Matrix/Matrix4.h"
#include "Shader.h"
#include "LightInstance.h"
#include <vector>
#include <string>


class DirectionalLightInstance;
class PointLightInstance;
class SpotLightInstance;

class LightInstance
{
public:
    explicit LightInstance(Light* lightResource);
    virtual ~LightInstance() = default;

    void                        setTransform(const LibMath::Matrix4& t);
    const LibMath::Matrix4&     getTransform() const;
    Light*                      getLight() const;


    /// Upload this light’s uniforms into the currently-bound shader
    /// idx is the index in the corresponding uniform array
    virtual void                uploadUniforms(Shader& shader, int idx) const = 0;

    static bool LoadInstances(
        const std::string& filename,
        ResourceManager& manager,
        std::vector<DirectionalLightInstance*>& outDir,
        std::vector<PointLightInstance*>& outPoint,
        std::vector<SpotLightInstance*>& outSpot
    );

    static std::string          stripInvisible(const std::string& in);

protected:
    Light*               m_light;
    LibMath::Matrix4     m_transform;
};

class DirectionalLightInstance : public LightInstance
{
public:
    explicit DirectionalLightInstance(Light* lightResource);
    void    uploadUniforms(Shader& shader, int idx) const override;
};

class PointLightInstance : public LightInstance
{
public:
    explicit PointLightInstance(Light* lightResource);
    void    uploadUniforms(Shader& shader, int idx) const override;
};

class SpotLightInstance : public LightInstance
{
public:
    explicit SpotLightInstance(Light* lightResource);
    void    uploadUniforms(Shader& shader, int idx) const override;
};