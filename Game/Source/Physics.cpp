#include "Physics/Physics.h"

std::optional<Physics::RaycastHit> Physics::PhysicsManager::Raycast(const LibMath::Line3D& ray, const std::vector<Collider*>& colliders, float maxDistance)
{
    std::optional<RaycastHit> closestHit = std::nullopt;
    float currentClosestDistance = maxDistance; // Initialize with max allowed distance

    for (const auto& colliderPtr : colliders)
    {
        if (colliderPtr == nullptr)
            continue; // Skip null colliders

        // Call the virtual intersect method on each collider
        // This will dispatch to BoxCollider::intersect, SphereCollider::intersect, etc.
        std::optional<RaycastHit> hit = colliderPtr->intersect(ray, currentClosestDistance);

        if (hit.has_value())
        {
            // If a hit occurred and it's closer than the current closest
            if (hit->m_distance < currentClosestDistance)
            {
                currentClosestDistance = hit->m_distance;
                closestHit = hit;
                // Ensure the collider pointer in the hit struct points to the actual collider
                // (though it should already be set by the individual intersect methods)
                closestHit->m_collider = colliderPtr;
            }
        }
    }
    return closestHit;
}

std::optional<Physics::RaycastHit> Physics::PhysicsManager::intersectRayAABB(const LibMath::Line3D& ray, const LibMath::Prism3DAABB& aabb, float maxDistance)
{
    // Ray origin and direction using Line3D getters
    const LibMath::Point3D& rayOrigin = ray.getOrigin();
    const LibMath::Vector3& rayDirection = ray.getDirection();

    // AABB min and max points using Prism3DAABB getters
    const LibMath::Point3D& aabbMin = aabb.getMin();
    const LibMath::Point3D& aabbMax = aabb.getMax();

    float tNear = 0.0f; // Represents the entry point (distance along ray)
    float tFar = maxDistance; // Represents the exit point (distance along ray), limited by maxDistance

    // Check for intersection with X-planes
    if (!LibMath::axisAABBIntersection(rayOrigin.getX(), rayDirection.m_x, aabbMin.getX(), aabbMax.getX(), tNear, tFar))
        return std::nullopt;

    // Check for intersection with Y-planes
    if (!LibMath::axisAABBIntersection(rayOrigin.getY(), rayDirection.m_y, aabbMin.getY(), aabbMax.getY(), tNear, tFar))
        return std::nullopt;

    // Check for intersection with Z-planes
    if (!LibMath::axisAABBIntersection(rayOrigin.getZ(), rayDirection.m_z, aabbMin.getZ(), aabbMax.getZ(), tNear, tFar))
        return std::nullopt;

    // If tNear is greater than maxDistance, the hit is beyond the allowed range.
    if (tNear > maxDistance)
    {
        return std::nullopt;
    }

    // If the ray origin is inside the AABB (tNear < 0), the actual hit distance is 0.
    // The hit point is the ray origin. This handles initial penetration.
    if (tNear < 0.0f)
    {
        RaycastHit hit;
        hit.m_distance = 0.0f; // Ray starts inside.
        hit.m_point = rayOrigin;
        // Normal is tricky if starting inside.
		LibMath::Vector3 rayDirectionNormalized = rayDirection;
		rayDirectionNormalized.normalize(); // Ensure the direction is normalized
        hit.m_normal = -rayDirectionNormalized; // Pointing opposite to ray direction if starting inside
        return hit;
    }

    // Calculate intersection point using getPoint(tNear)
    LibMath::Point3D hitPoint = ray.getPoint(tNear);

    // Calculate normal vector (based on which axis was intersected first and ray direction)
    LibMath::Vector3 normal;
    const float epsilon = 1e-4f; // Small value for float comparison

    // Determine which face was hit
    if (std::fabs(hitPoint.getX() - aabbMin.getX()) < epsilon) normal = { -1, 0, 0 };
    else if (std::fabs(hitPoint.getX() - aabbMax.getX()) < epsilon) normal = { 1, 0, 0 };
    else if (std::fabs(hitPoint.getY() - aabbMin.getY()) < epsilon) normal = {0, -1, 0};
    else if (std::fabs(hitPoint.getY() - aabbMax.getY()) < epsilon) normal = { 0, 1, 0 };
    else if (std::fabs(hitPoint.getZ() - aabbMin.getZ()) < epsilon) normal = {0, 0, -1};
    else if (std::fabs(hitPoint.getZ() - aabbMax.getZ()) < epsilon) normal = { 0, 0, 1 };
    else {
        normal = { 0, 0, 0 }; // Should ideally not happen with precise floating points if hit is detected.
    }

    // Ensure the normal points *out* of the AABB relative to the incoming ray.
    // This is crucial for correct collision response.
    if (normal.dot(rayDirection) > 0)
    {
        normal = -normal;
    }
    normal.normalize(); // Ensure unit length

    RaycastHit hit;
    hit.m_distance = tNear;
    hit.m_point = hitPoint;
    hit.m_normal = normal;
    hit.m_collider = nullptr; // Will be set by the calling Collider::intersect method

    return hit;
}
