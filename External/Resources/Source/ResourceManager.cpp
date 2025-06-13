#include"ResourceManager.h"
#include<iostream>

ResourceManager::~ResourceManager() 
{
    for (auto& pair : resources) 
    {
        delete pair.second;
    }
    resources.clear();
}
// Delete a resource by name
void ResourceManager::delete_resource(const std::string& name) 
{
    auto it = resources.find(name);
    if (it != resources.end()) 
    {
        delete it->second;
        resources.erase(it);
    }
}
