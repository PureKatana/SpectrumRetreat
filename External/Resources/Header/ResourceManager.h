#pragma once
#include<unordered_map>
#include<string>
#include"IResource.h"

class ResourceManager
{

private:
	std::unordered_map<std::string, IResource*> resources;

public:

	~ResourceManager();

	template<typename T>
	T*      create(const std::string& name);

	template<typename T>
	T*      get(const std::string& name);

	void    delete_resource(const std::string& name);
};


template<typename T>
T* ResourceManager::create(const std::string& name) 
{
	// Ensure T is derived from IResource
    static_assert(std::is_base_of<IResource, T>::value, "T must inherit from IResource");

	// Check if the resource already exists
    auto it = resources.find(name);
    if (it != resources.end()) {
        delete it -> second;
        resources.erase(it);
    }

	// create a new resource of type T
    T* resource = new T();
    resources[name] = resource;
    return resource;
}

template<typename T>
T* ResourceManager::get(const std::string& name) 
{
    static_assert(std::is_base_of<IResource, T>::value, "T must inherit from IResource");

    auto it = resources.find(name);
    if (it != resources.end()) {
        return dynamic_cast<T*>(it -> second);
    }

    return nullptr;
}