#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <LibMath/Matrix/Matrix4.h>
#include "Mesh.h"

template<typename T>
class SceneGraphNode
{
public:
	using NodePtr = std::unique_ptr<SceneGraphNode<T>>;

	explicit SceneGraphNode(const T& data)
		: m_data(data), m_parent(nullptr),
		m_localTransform(LibMath::Matrix4::identity()),
		m_globalTransform(LibMath::Matrix4::identity()) 
	{
	}

	// Add a child node (takes ownership)
	void addChild(NodePtr child) 
	{
		child -> m_parent = this;
		m_children.push_back(std::move(child));
	}

	// Remove a child node
	void removeChild(SceneGraphNode<T>* child) 
	{
		// Identify Elements to Remove: std::remove_if() iterates through the m_children container. 
		// For each element (each NodePtr), it executes the lambda function. 
		// If the lambda returns true (meaning the NodePtr points to the child you want to remove), 
		// that element is marked for removal (by being moved towards the end of the container).

		// get the Removal Boundary : std::remove_if() returns an iterator 
		// pointing to the first element that should be removed(the beginning of the "garbage" section at the end of the container).

	    // Actually Remove the Elements : m_children.erase() is then called with the range starting 
		// from the iterator returned by std::remove_if() and ending at the original m_children.end().
		// This effectively removes all the elements that std::remove_if() moved to the end.
		m_children.erase
		(
			std::remove_if(m_children.begin(), m_children.end(),
				[child](const NodePtr& ptr) 
				{ return ptr.get() == child; }
			),
			m_children.end()
		);
	}

	// Update the global transform of this node and its children
	void updateGlobalTransform()
	{
		if (m_parent)
		{
			m_globalTransform = m_parent -> m_globalTransform * m_localTransform;
		}
		else
		{
			m_globalTransform = m_localTransform;
		}
		for (auto& child : m_children)
		{
			child -> updateGlobalTransform();
		}
	}

	// Accessors
	T&							getData() { return m_data; }
	const T&					getData() const { return m_data; }
	const LibMath::Matrix4&		getLocalTransform() const { return m_localTransform; }
	void						setLocalTransform(const LibMath::Matrix4& transform) { m_localTransform = transform; }
	const LibMath::Matrix4&		getGlobalTransform() const { return m_globalTransform; }
	SceneGraphNode<T>*			getParent() const { return m_parent; }
	const std::vector<NodePtr>& getChildren() const { return m_children; }

private:
	T						m_data;
	SceneGraphNode<T>*		m_parent = nullptr;
	std::vector<NodePtr>	m_children;
	LibMath::Matrix4		m_localTransform;
	LibMath::Matrix4		m_globalTransform;
};

struct SceneNode {
	std::string				m_name;
	Mesh*					m_mesh = nullptr;
};