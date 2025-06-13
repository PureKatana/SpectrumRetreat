#pragma once

#include "IResource.h"
#include "LibMath/vector.h"


class Material : public IResource
{
public:
    // 1) Parameterized constructor
    Material();

    // 2) Copy constructor
    Material(const Material& other) = default;

    // 3) Copy assignment
    Material& operator=(const Material& other) = default;

    // 4) Move constructor
    Material(Material&& other) noexcept = default;

    // 5) Move assignment
    Material& operator=(Material&& other) noexcept = default;

    // Destructor (override if IResource has a virtual dtor)
    ~Material() override = default;

    void set(const LibMath::Vector3& ambient,
        const LibMath::Vector3& diffuse,
        const LibMath::Vector3& specular,
        float shininess)
    {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->shininess = shininess;
    }

public:
    LibMath::Vector3 ambient;
    LibMath::Vector3 diffuse;
    LibMath::Vector3 specular;
    float shininess;
};