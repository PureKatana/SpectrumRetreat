#pragma once

#pragma once

#include "RaycastHit.h"
#include "Collider.h"
#include <vector>
#include <memory> 
#include <optional>

namespace Physics
{
    class Collider;

    class PhysicsManager
    {
    public:
        // Raycast: returns the first hit, if any, along the ray
        static std::optional<RaycastHit>    Raycast(
            const LibMath::Line3D& ray,
            const std::vector<Collider*>& colliders,
            float maxDistance = 1000.0f
        );

        // Implementation of intersectRayAABB
        static std::optional<RaycastHit>    intersectRayAABB(const LibMath::Line3D& ray, const LibMath::Prism3DAABB& aabb, float maxDistance);
    };
}