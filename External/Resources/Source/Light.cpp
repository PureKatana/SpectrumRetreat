#include "Light.h"

Light::Light()
    : m_ambient(0.1f, 0.1f, 0.1f, 1.0f)
    , m_diffuse(1.0f, 1.0f, 1.0f, 1.0f)
    , m_specular(1.0f, 1.0f, 1.0f, 1.0f)
    , m_constant(1.0f)
    , m_linear(0.09f)
    , m_quadratic(0.032f)
    , m_cutOff(LibMath::Radian(12.5f))
    , m_outerCutOff(LibMath::Radian(17.5f))
{}
// Get the ambient color of the light
const LibMath::Vector4& Light::getAmbient() const 
{
    return m_ambient;
}
// Set the ambient color of the light
void Light::setAmbient(const LibMath::Vector4& color) 
{
    m_ambient = color;
}
// Get the diffuse color of the light
const LibMath::Vector4& Light::getDiffuse() const 
{
    return m_diffuse;
}
// Set the diffuse color of the light
void Light::setDiffuse(const LibMath::Vector4& color) 
{
    m_diffuse = color;
}

// Get the specular color of the light
const LibMath::Vector4& Light::getSpecular() const 
{
    return m_specular;
}

// Set the specular color of the light
void Light::setSpecular(const LibMath::Vector4& color) 
{
    m_specular = color;
}

// Set the attenuation factors for the light
void Light::setAttenuation(float constant, float linear, float quadratic) 
{
    m_constant = constant;
    m_linear = linear;
    m_quadratic = quadratic;
}

// Get the attenuation factors for the light
void Light::getAttenuation(float& constant, float& linear, float& quadratic) const 
{
    constant = m_constant;
    linear = m_linear;
    quadratic = m_quadratic;
}

// Set the inner and outer cutoff angles for the spotlight
void Light::setSpotCutoff(const LibMath::Radian& inner, const LibMath::Radian& outer) 
{
    m_cutOff = inner;
    m_outerCutOff = outer;
}

// Get the inner and outer cutoff angles for the spotlight
void Light::getSpotCutoff(LibMath::Radian& inner, LibMath::Radian& outer) const 
{
    inner = m_cutOff;
    outer = m_outerCutOff;
}