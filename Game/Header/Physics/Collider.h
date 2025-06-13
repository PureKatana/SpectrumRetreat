#pragma once 

#include "LibMath/Geometry3D.h"
#include "Mesh.h"               
#include "Physics.h" 
#include <memory>               
#include <optional>             

class GameObject;
namespace Physics
{
    // Forward declarations for derived collider types.
    class BoxCollider;
    class SphereCollider;
    class CapsuleCollider;

    enum class ColliderType
    {
        BOX,
        SPHERE,
        CAPSULE
    };

    class Collider
    {
    public:
        // Constructor initializes the collider type.
        Collider(ColliderType type);
        // Virtual destructor ensures proper cleanup of derived objects.
        virtual ~Collider() = default;

        // --- Core Polymorphic Interface ---

        // Ray Intersection: Every derived Collider must implement how it intersects with a ray.
        // It returns an optional RaycastHit if a hit occurs within maxDistance.
        virtual std::optional<RaycastHit>   intersect(const LibMath::Line3D& ray, float maxDistance) const = 0;

        // Base Collision Check: Delegates to the appropriate specific collision check in the derived class.
        virtual bool                        checkCollision(const Collider& other, LibMath::Vector3& outNormal) const = 0;

        // Double-Dispatch Methods: These are called by `checkCollision` to perform type-specific checks.
        // Each derived class will implement these to handle `this_type` vs `other_type` collisions.
        virtual bool                        checkCollisionWithBox(const BoxCollider& self, LibMath::Vector3& outNormal) const = 0;
        virtual bool                        checkCollisionWithSphere(const SphereCollider& self, LibMath::Vector3& outNormal) const = 0;
        virtual bool                        checkCollisionWithCapsule(const CapsuleCollider& self, LibMath::Vector3& outNormal) const = 0;

        // --- Static Factory Methods for creating Collider instances ---

        // Creates a specific Collider type from mesh data.
        static std::unique_ptr<Collider>    createFromMesh(ColliderType type, Mesh* mesh);
        // Creates a CapsuleCollider with manually set parameters.
        static std::unique_ptr<Collider>    createCapsuleManualSet(LibMath::Point3D p1, LibMath::Point3D p2, float r);

        // --- Getters ---

        // Returns the type of the collider.
        ColliderType                        getType() const;
        // Returns a pointer to the underlying LibMath::Object3D.
        // Its utility is reduced in this polymorphic design, but kept for compatibility.
        const LibMath::Object3D*            getObject() const;

		// Returns a pointer to the mesh data, if applicable.
        GameObject*                         getGameObject() const;
		void                                setGameObject(GameObject* gameObject);

        virtual void                        updateBounds() = 0;

    protected:
        // m_colliderObject is now primarily for compatibility with `getObject()`.
        // Derived classes will directly hold their specific LibMath objects.
        std::unique_ptr<LibMath::Object3D>  m_colliderObject;
		GameObject*                         m_gameObject; // Pointer to the mesh data, if applicable.

    private:
        ColliderType                        m_type; // Type is now private and managed internally.
    };

    // --- Derived Collider Classes ---

    class BoxCollider : public Collider
    {
    public:
        BoxCollider(const LibMath::Prism3DAABB& aabb);

        // Factory method specific to BoxCollider.
        static std::unique_ptr<BoxCollider>     createFromMesh(Mesh* mesh);

        // Implementations of virtual methods from base Collider.
        std::optional<RaycastHit>               intersect(const LibMath::Line3D& ray, float maxDistance) const override;
        bool                                    checkCollision(const Collider& other, LibMath::Vector3& outNormal) const override;
        bool                                    checkCollisionWithBox(const BoxCollider& other, LibMath::Vector3& outNormal) const override;
        bool                                    checkCollisionWithSphere(const SphereCollider& other, LibMath::Vector3& outNormal) const override;
        bool                                    checkCollisionWithCapsule(const CapsuleCollider& other, LibMath::Vector3& outNormal) const override;

        // Getter for the specific AABB data.
        const LibMath::Prism3DAABB&             getAABB() const { return m_aabb; }

		// Update the AABB bounds based on the current state of the collider.
        void                                    updateBounds() override;

    private:
        LibMath::Prism3DAABB    m_aabb; // BoxCollider directly owns its AABB.
    };

    class SphereCollider : public Collider
    {
    public:
        SphereCollider(const LibMath::Sphere3D& sphere);

        // Factory method specific to SphereCollider.
        static std::unique_ptr<SphereCollider>  createFromMesh(Mesh* mesh);

        // Implementations of virtual methods.
        std::optional<RaycastHit>               intersect(const LibMath::Line3D& ray, float maxDistance) const override;
        bool                                    checkCollision(const Collider& other, LibMath::Vector3& outNormal) const override;
        bool                                    checkCollisionWithBox(const BoxCollider& other, LibMath::Vector3& outNormal) const override;
        bool                                    checkCollisionWithSphere(const SphereCollider& other, LibMath::Vector3& outNormal) const override;
        bool                                    checkCollisionWithCapsule(const CapsuleCollider& other, LibMath::Vector3& outNormal) const override;

        // Getter for the specific Sphere data.
        const LibMath::Sphere3D&                getSphere() const { return m_sphere; }

		// Update the Sphere bounds based on the current state of the collider.
		void                                    updateBounds() override;

    private:
        LibMath::Sphere3D   m_sphere; // SphereCollider directly owns its Sphere.
    };

    class CapsuleCollider : public Collider
    {
    public:
        CapsuleCollider(const LibMath::Capsule3D& capsule);

        // Factory methods specific to CapsuleCollider.
        static std::unique_ptr<CapsuleCollider>     createFromMesh(Mesh* mesh);
        static std::unique_ptr<CapsuleCollider>     createManualSet(LibMath::Point3D p1, LibMath::Point3D p2, float r);

        // Implementations of virtual methods.
        std::optional<RaycastHit>                   intersect(const LibMath::Line3D& ray, float maxDistance) const override;
        bool                                        checkCollision(const Collider& other, LibMath::Vector3& outNormal) const override;
        bool                                        checkCollisionWithBox(const BoxCollider& other, LibMath::Vector3& outNormal) const override;
        bool                                        checkCollisionWithSphere(const SphereCollider& other, LibMath::Vector3& outNormal) const override;
        bool                                        checkCollisionWithCapsule(const CapsuleCollider& other, LibMath::Vector3& outNormal) const override;

        // Getter for the specific Capsule data.
        const LibMath::Capsule3D&                   getCapsule() const { return m_capsule; }

		// Update the Capsule bounds based on the current state of the collider.
		void                                        updateBounds() override;

		void                                        updateCapsule(const LibMath::Point3D& p1, const LibMath::Point3D& p2, float r);

    private:
        LibMath::Capsule3D m_capsule; // CapsuleCollider directly owns its Capsule.
    };

} // namespace PhysicsManager
