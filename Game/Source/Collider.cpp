#include "Physics/Collider.h"
#include "GameObject.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include "LibMath/Geometry3D.h"

// Helper to extract positions from mesh
static std::vector<LibMath::Vector3> GetTransformedMeshPositions(Mesh* mesh)
{
    std::vector<LibMath::Vector3> positions;
    if (mesh && mesh->getModel())
    {
        const LibMath::Matrix4& modelMatrix = mesh -> getModelMatrix();
        for (const auto& v : mesh->getModel() -> getVertices())
        {
            // Transform the position by the model matrix
            LibMath::Vector3 pos = v.m_position;
            // Homogeneous transform
            float x = pos.m_x, y = pos.m_y, z = pos.m_z;
            const float* m = modelMatrix.getData();
            float tx = m[0] * x + m[4] * y + m[8] * z + m[12];
            float ty = m[1] * x + m[5] * y + m[9] * z + m[13];
            float tz = m[2] * x + m[6] * y + m[10] * z + m[14];
            positions.emplace_back(tx, ty, tz);
        }
    }
    return positions;
}

namespace Physics
{
    // --- Base Collider Class Implementation ---

    // Constructor: Initializes the collider type.
    Collider::Collider(ColliderType type)
        : m_type(type)
    {
        
    }

    // Returns the type of the collider.
    ColliderType Collider::getType() const
    {
        return m_type;
    }

    // Returns a pointer to the underlying LibMath::Object3D.
    const LibMath::Object3D* Collider::getObject() const
    {
        return m_colliderObject.get();
    }

    void Collider::setGameObject(GameObject* gameObject)
    {
		m_gameObject = gameObject;
    }

    GameObject* Collider::getGameObject() const
    {
        return m_gameObject;
    }

    // --- Static Factory Methods (Delegate to Derived Classes) ---

    // Creates a specific Collider type from mesh data by delegating to derived class factories.
    std::unique_ptr<Collider> Collider::createFromMesh(ColliderType type, Mesh* mesh)
    {
        switch (type)
        {
        case ColliderType::BOX:
            return BoxCollider::createFromMesh(mesh);
        case ColliderType::SPHERE:
            return SphereCollider::createFromMesh(mesh);
        case ColliderType::CAPSULE:
            return CapsuleCollider::createFromMesh(mesh);
        default:
            std::cerr << "Error: Attempted to create unknown collider type from mesh.\n";
            return nullptr;
        }
    }

    // Creates a CapsuleCollider with manually set parameters by delegating to the derived class factory.
    std::unique_ptr<Collider> Collider::createCapsuleManualSet(LibMath::Point3D p1, LibMath::Point3D p2, float r)
    {
        return CapsuleCollider::createManualSet(p1, p2, r);
    }

    // --- BoxCollider Implementation ---

    // Constructor: Initializes the base Collider part and its own AABB.
    BoxCollider::BoxCollider(const LibMath::Prism3DAABB& aabb)
        : Collider(ColliderType::BOX), m_aabb(aabb)
    {
        m_colliderObject = std::make_unique<LibMath::Prism3DAABB>(aabb);
    }

    // Static Factory Method: Creates a BoxCollider from a Mesh.
    std::unique_ptr<BoxCollider> BoxCollider::createFromMesh(Mesh* mesh)
    {
        auto positions = GetTransformedMeshPositions(mesh);
        if (positions.empty()) return nullptr;

        LibMath::Vector3 min = positions[0], max = positions[0];
        for (const auto& v : positions)
        {
            min.m_x = std::min(min.m_x, v.m_x);
            min.m_y = std::min(min.m_y, v.m_y);
            min.m_z = std::min(min.m_z, v.m_z);
            max.m_x = std::max(max.m_x, v.m_x);
            max.m_y = std::max(max.m_y, v.m_y);
            max.m_z = std::max(max.m_z, v.m_z);

        }
        LibMath::Prism3DAABB aabb(LibMath::Point3D(min.m_x, min.m_y, min.m_z),
            LibMath::Point3D(max.m_x, max.m_y, max.m_z));

        std::unique_ptr<BoxCollider> box = std::make_unique<BoxCollider>(aabb);
        return box;
    }

    // Raycast Intersection: Implements the virtual intersect method for BoxCollider.
    std::optional<RaycastHit> BoxCollider::intersect(const LibMath::Line3D& ray, float maxDistance) const
    {
        std::optional<RaycastHit> hit = PhysicsManager::intersectRayAABB(ray, m_aabb, maxDistance);

        if (hit.has_value())
        {
            hit -> m_collider = this;
        }
        return hit;
    }

    // --- Collision Check Implementations for BoxCollider (Double Dispatch) ---

    // First Dispatch: Delegates to the specific collision check on the 'other' collider.
    bool BoxCollider::checkCollision(const Collider& other, LibMath::Vector3& outNormal) const
    {
        // Here, 'this' is a BoxCollider. We want to ask 'other' to check collision
        // with *this BoxCollider*.
        switch (other.getType())
        {
        case ColliderType::BOX:
            // If 'other' is also a BoxCollider, call its checkCollisionWithBox method
            // and pass *this (a BoxCollider) as the 'self' argument.
            return static_cast<const BoxCollider&>(other).checkCollisionWithBox(*this, outNormal);
        case ColliderType::SPHERE:
            // If 'other' is a SphereCollider, call its checkCollisionWithSphere method
            // and pass *this (a BoxCollider) as the 'self' argument.
            return static_cast<const SphereCollider&>(other).checkCollisionWithBox(*this, outNormal);
        case ColliderType::CAPSULE:
            // If 'other' is a CapsuleCollider, call its checkCollisionWithCapsule method
            // and pass *this (a BoxCollider) as the 'self' argument.
            return static_cast<const CapsuleCollider&>(other).checkCollisionWithBox(*this, outNormal);
        default:
            std::cerr << "Error: Unknown collider type in BoxCollider::checkCollision\n";
            return false;
        }
    }

    // Second Dispatch: Box-Box collision check.
    bool BoxCollider::checkCollisionWithBox(const BoxCollider& other, LibMath::Vector3& outNormal) const
    {
        // Actual collision logic between this->m_aabb and other.m_aabb
        //return LibMath::isColliding(this->m_aabb, other.m_aabb, outNormal);
		return false; // Placeholder, replace with actual collision logic
    }

    // Second Dispatch: Box-Sphere collision check.
    bool BoxCollider::checkCollisionWithSphere(const SphereCollider& other, LibMath::Vector3& outNormal) const
    {
        // Placeholder: Implement Box-Sphere collision using your LibMath functions.
        // return LibMath::isColliding(this->m_aabb, other.getSphere(), outNormal);
        return false;
    }

    // Second Dispatch: Box-Capsule collision check.
    bool BoxCollider::checkCollisionWithCapsule(const CapsuleCollider& other, LibMath::Vector3& outNormal) const
    {
        // Placeholder: Implement Box-Capsule collision using your LibMath functions.
        //return LibMath::isColliding(this->m_aabb, other.getCapsule(), outNormal);
        return false;
    }

    void BoxCollider::updateBounds()
    {
        // Handle cases where there's no game object or mesh
        if (!m_gameObject || !m_gameObject -> m_mesh)
        {
            return;
        }

        // Get the transformed vertices using the shared helper function
        std::vector<LibMath::Vector3> positions = GetTransformedMeshPositions(m_gameObject -> m_mesh);

        // If no positions are returned, there's nothing to do
        if (positions.empty())
        {
            return;
        }

        // Initialize min and max with the first vertex
        LibMath::Vector3 min = positions[0];
        LibMath::Vector3 max = positions[0];

        // Iterate through all transformed vertices to find the AABB
        for (const auto& v : positions)
        {
            min.m_x = std::min(min.m_x, v.m_x);
            min.m_y = std::min(min.m_y, v.m_y);
            min.m_z = std::min(min.m_z, v.m_z);
            max.m_x = std::max(max.m_x, v.m_x);
            max.m_y = std::max(max.m_y, v.m_y);
            max.m_z = std::max(max.m_z, v.m_z);
        }

        // Update the existing AABB (Axis-Aligned Bounding Box)
        m_aabb = LibMath::Prism3DAABB(LibMath::Point3D(min.m_x, min.m_y, min.m_z),
            LibMath::Point3D(max.m_x, max.m_y, max.m_z));
    }

    // --- SphereCollider Implementation ---

    // Constructor: Initializes the base Collider part and its own Sphere.
    SphereCollider::SphereCollider(const LibMath::Sphere3D& sphere)
        : Collider(ColliderType::SPHERE), m_sphere(sphere)
    {
        m_colliderObject = std::make_unique<LibMath::Sphere3D>(sphere);
    }

    // Static Factory Method: Creates a SphereCollider from a Mesh.
    std::unique_ptr<SphereCollider> SphereCollider::createFromMesh(Mesh* mesh)
    {
        auto positions = GetTransformedMeshPositions(mesh);
        if (positions.empty()) return nullptr;

        LibMath::Vector3 center{ 0,0,0 };
        for (const auto& v : positions)
            center = center + v;
        center = center * (1.0f / positions.size());

        float maxDistSq = 0.0f;
        for (const auto& v : positions)
        {
            float distSq = (v - center).magnitudeSquared();
            if (distSq > maxDistSq)
                maxDistSq = distSq;
        }
        float radius = std::sqrt(maxDistSq);

        LibMath::Sphere3D sphere(LibMath::Point3D(center.m_x, center.m_y, center.m_z), radius);

		std::unique_ptr<SphereCollider> sphereCollider = std::make_unique<SphereCollider>(sphere);
        return sphereCollider;
    }

    // Raycast Intersection: Implements the virtual intersect method for SphereCollider.
    std::optional<RaycastHit> SphereCollider::intersect(const LibMath::Line3D& ray, float maxDistance) const
    {
        // Placeholder: Implement LibMath::intersectRaySphere.
        // std::optional<RaycastHit> hit = LibMath::intersectRaySphere(ray, m_sphere, maxDistance);
        std::optional<RaycastHit> hit = std::nullopt; // Placeholder

        if (hit.has_value())
        {
            hit->m_collider = this;
        }
        return hit;
    }

    // --- Collision Check Implementations for SphereCollider (Double Dispatch) ---

    // First Dispatch: Delegates to the specific collision check on the 'other' collider.
    bool SphereCollider::checkCollision(const Collider& other, LibMath::Vector3& outNormal) const
    {
        // Here, 'this' is a SphereCollider. We want to ask 'other' to check collision
        // with *this SphereCollider*.
        switch (other.getType())
        {
        case ColliderType::BOX:
            return static_cast<const BoxCollider&>(other).checkCollisionWithSphere(*this, outNormal);
        case ColliderType::SPHERE:
            return static_cast<const SphereCollider&>(other).checkCollisionWithSphere(*this, outNormal);
        case ColliderType::CAPSULE:
            return static_cast<const CapsuleCollider&>(other).checkCollisionWithSphere(*this, outNormal);
        default:
            std::cerr << "Error: Unknown collider type in SphereCollider::checkCollision\n";
            return false;
        }
    }

    // Second Dispatch: Sphere-Box collision check.
    bool SphereCollider::checkCollisionWithBox(const BoxCollider& other, LibMath::Vector3& outNormal) const
    {
        // Placeholder: Implement Sphere-Box collision.
        // return LibMath::isColliding(this->m_sphere, other.getAABB(), outNormal);
        return false;
    }

    // Second Dispatch: Sphere-Sphere collision check.
    bool SphereCollider::checkCollisionWithSphere(const SphereCollider& other, LibMath::Vector3& outNormal) const
    {
        // Placeholder: Implement Sphere-Sphere collision.
        // return LibMath::isColliding(this->m_sphere, other.m_sphere, outNormal);
        return false;
    }

    // Second Dispatch: Sphere-Capsule collision check.
    bool SphereCollider::checkCollisionWithCapsule(const CapsuleCollider& other, LibMath::Vector3& outNormal) const
    {
        // Placeholder: Implement Sphere-Capsule collision.
        // return LibMath::isColliding(this->m_sphere, other.getCapsule(), outNormal);
        return false;
    }

    void SphereCollider::updateBounds()
    {
        // Handle cases where there's no game object or mesh
        if (!m_gameObject || !m_gameObject -> m_mesh)
        {
            return;
        }

        // Get the transformed vertices using the shared helper function
        std::vector<LibMath::Vector3> positions = GetTransformedMeshPositions(m_gameObject -> m_mesh);

        // If no positions are returned, there's nothing to do
        if (positions.empty())
        {
            return;
        }

        // Calculate the center of the bounding sphere by averaging all transformed vertices
        LibMath::Vector3 center{ 0, 0, 0 };
        for (const auto& v : positions)
        {
            center = center + v;
        }
        center = center * (1.0f / positions.size()); // Divide by the number of vertices

        // Calculate the radius of the bounding sphere
        // Find the maximum squared distance from the calculated center to any vertex
        float maxDistSq = 0.0f;
        for (const auto& v : positions)
        {
            float distSq = (v - center).magnitudeSquared();
            if (distSq > maxDistSq)
            {
                maxDistSq = distSq;
            }
        }
        float radius = std::sqrt(maxDistSq); // Take the square root to get the actual radius

        // Update the existing Sphere3D object with the new center and radius
        m_sphere = LibMath::Sphere3D(LibMath::Point3D(center.m_x, center.m_y, center.m_z), radius);
    }

    // --- CapsuleCollider Implementation ---

    // Constructor: Initializes the base Collider part and its own Capsule.
    CapsuleCollider::CapsuleCollider(const LibMath::Capsule3D& capsule)
        : Collider(ColliderType::CAPSULE), m_capsule(capsule)
    {
        m_colliderObject = std::make_unique<LibMath::Capsule3D>(capsule);
    }

    // Static Factory Method: Creates a CapsuleCollider from a Mesh.
    std::unique_ptr<CapsuleCollider> CapsuleCollider::createFromMesh(Mesh* mesh)
    {
        auto positions = GetTransformedMeshPositions(mesh);
        if (positions.empty()) return nullptr;

        float maxDistSq = 0.0f;
        LibMath::Vector3 a = positions[0], b = positions[0];
        for (const auto& v1 : positions)
        {
            for (const auto& v2 : positions)
            {
                float distSq = (v1 - v2).magnitudeSquared();
                if (distSq > maxDistSq)
                {
                    maxDistSq = distSq;
                    a = v1;
                    b = v2;
                }
            }
        }
        float maxRadius = 0.0f;
        LibMath::Vector3 ab = b - a;
        float abLenSq = ab.magnitudeSquared();
        for (const auto& v : positions)
        {
            LibMath::Vector3 av = v - a;
            float t = abLenSq > 0 ? std::clamp(av.dot(ab) / abLenSq, 0.0f, 1.0f) : 0.0f;
            LibMath::Vector3 closest = a + ab * t;
            float dist = (v - closest).magnitude();
            if (dist > maxRadius)
                maxRadius = dist;
        }
        LibMath::Capsule3D capsule(
            LibMath::Point3D(a.m_x, a.m_y, a.m_z),
            LibMath::Point3D(b.m_x, b.m_y, b.m_z),
            maxRadius
        );

		std::unique_ptr<CapsuleCollider> capsuleCollider = std::make_unique<CapsuleCollider>(capsule);
		return capsuleCollider;
    }

    // Static Factory Method: Creates a CapsuleCollider with manually set parameters.
    std::unique_ptr<CapsuleCollider> CapsuleCollider::createManualSet(LibMath::Point3D p1, LibMath::Point3D p2, float r)
    {
        LibMath::Capsule3D capsule(p1, p2, r);
        return std::make_unique<CapsuleCollider>(capsule);
    }

    // Raycast Intersection: Implements the virtual intersect method for CapsuleCollider.
    std::optional<RaycastHit> CapsuleCollider::intersect(const LibMath::Line3D& ray, float maxDistance) const
    {
        // Placeholder: Implement LibMath::intersectRayCapsule.
        // std::optional<RaycastHit> hit = LibMath::intersectRayCapsule(ray, m_capsule, maxDistance);
        std::optional<RaycastHit> hit = std::nullopt; // Placeholder

        if (hit.has_value())
        {
            hit -> m_collider = this;
        }
        return hit;
    }

    // --- Collision Check Implementations for CapsuleCollider (Double Dispatch) ---

    // First Dispatch: Delegates to the specific collision check on the 'other' collider.
    bool CapsuleCollider::checkCollision(const Collider& other, LibMath::Vector3& outNormal) const
    {
        // Here, 'this' is a CapsuleCollider. We want to ask 'other' to check collision
        // with *this CapsuleCollider*.
        switch (other.getType())
        {
        case ColliderType::BOX:
            return static_cast<const BoxCollider&>(other).checkCollisionWithCapsule(*this, outNormal);
        case ColliderType::SPHERE:
            return static_cast<const SphereCollider&>(other).checkCollisionWithCapsule(*this, outNormal);
        case ColliderType::CAPSULE:
            return static_cast<const CapsuleCollider&>(other).checkCollisionWithCapsule(*this, outNormal);
        default:
            std::cerr << "Error: Unknown collider type in CapsuleCollider::checkCollision\n";
            return false;
        }
    }

    // Second Dispatch: Capsule-Box collision check.
    bool CapsuleCollider::checkCollisionWithBox(const BoxCollider& other, LibMath::Vector3& outNormal) const
    {
        // Placeholder: Implement Capsule-Box collision.
        return LibMath::isColliding(this -> m_capsule, other.getAABB(), outNormal);
        //return false;
    }

    // Second Dispatch: Capsule-Sphere collision check.
    bool CapsuleCollider::checkCollisionWithSphere(const SphereCollider& other, LibMath::Vector3& outNormal) const
    {
        // Placeholder: Implement Capsule-Sphere collision.
        // return LibMath::isColliding(this->m_capsule, other.getSphere(), outNormal);
        return false;
    }

    // Second Dispatch: Capsule-Capsule collision check.
    bool CapsuleCollider::checkCollisionWithCapsule(const CapsuleCollider& other, LibMath::Vector3& outNormal) const
    {
        // Placeholder: Implement Capsule-Capsule collision.
        // return LibMath::isColliding(this->m_capsule, other.m_capsule, outNormal);
        return false;
    }

    void CapsuleCollider::updateBounds()
    {
        if (!m_gameObject || !m_gameObject -> m_mesh)
        {
            return;
        }

        auto positions = GetTransformedMeshPositions(m_gameObject -> m_mesh);
        if (positions.empty()) return;

        float maxDistSq = 0.0f;
        LibMath::Vector3 a = positions[0], b = positions[0];
        for (const auto& v1 : positions)
        {
            for (const auto& v2 : positions)
            {
                float distSq = (v1 - v2).magnitudeSquared();
                if (distSq > maxDistSq)
                {
                    maxDistSq = distSq;
                    a = v1;
                    b = v2;
                }
            }
        }
        float maxRadius = 0.0f;
        LibMath::Vector3 ab = b - a;
        float abLenSq = ab.magnitudeSquared();
        for (const auto& v : positions)
        {
            LibMath::Vector3 av = v - a;
            float t = abLenSq > 0 ? std::clamp(av.dot(ab) / abLenSq, 0.0f, 1.0f) : 0.0f;
            LibMath::Vector3 closest = a + ab * t;
            float dist = (v - closest).magnitude();
            if (dist > maxRadius)
                maxRadius = dist;
        }
        LibMath::Capsule3D capsule(
            LibMath::Point3D(a.m_x, a.m_y, a.m_z),
            LibMath::Point3D(b.m_x, b.m_y, b.m_z),
            maxRadius
        );

		// Update the existing Capsule3D object with the new endpoints and radius
		m_capsule = capsule;
    }

    void CapsuleCollider::updateCapsule(const LibMath::Point3D& p1, const LibMath::Point3D& p2, float r)
    {
		m_capsule = LibMath::Capsule3D(p1, p2, r);
    }

} // namespace PhysicsManager

