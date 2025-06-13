#pragma once

#include "IResource.h"
#pragma once

#include "LibMath/Vector/Vector4.h"
#include "LibMath/Angle/Radian.h"
#include "ResourceManager.h"
#include "IResource.h"


class Light : public IResource {
public:
    Light();

    // Colors
    const LibMath::Vector4&     getAmbient() const;
    void                        setAmbient(const LibMath::Vector4& color);

    const LibMath::Vector4&     getDiffuse() const;
    void                        setDiffuse(const LibMath::Vector4& color);

    const LibMath::Vector4&     getSpecular() const;
    void                        setSpecular(const LibMath::Vector4& color);

    // Attenuation (point & spot only)
    void                        setAttenuation(float constant, float linear, float quadratic);
    void                        getAttenuation(float& constant, float& linear, float& quadratic) const;

    // Spot parameters
    void                        setSpotCutoff(const LibMath::Radian& inner, const LibMath::Radian& outer);
    void                        getSpotCutoff(LibMath::Radian& inner, LibMath::Radian& outer) const;



private:
    LibMath::Vector4    m_ambient;
    LibMath::Vector4    m_diffuse;
    LibMath::Vector4    m_specular;

    float               m_constant;
    float               m_linear;
    float               m_quadratic;

    LibMath::Radian     m_cutOff;
    LibMath::Radian     m_outerCutOff;
};
