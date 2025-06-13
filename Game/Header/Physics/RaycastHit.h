// PhysicsManager/RaycastHit.h
#pragma once

#include "LibMath/Geometry3D.h"  
#include "LibMath/Vector/Vector3.h" 

namespace Physics
{
    // Forward declaration for Collider, as RaycastHit contains a pointer to it.
    class Collider;

    // Definition of the RaycastHit struct
    struct RaycastHit
    {
        const Collider*     m_collider = nullptr; // Pointer to the hit collider
        LibMath::Point3D    m_point;             // World-space intersection point
        float               m_distance = 0.0f;              // Distance from ray origin to hit point
        LibMath::Vector3    m_normal;            // Surface normal at the hit point
    };
}
