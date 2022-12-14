#pragma once
#include "GResource.h"
#include "Logger.h"
#include <unordered_map>
#include <memory>
#include <functional>

/*
	Resourcemanager is a singleton and can be used everywhere

	----Code example----
	add - resource get added to manager
	A resource has to be inherited by resource::GResource
	class Material : public GResource
	Material material;
	* ResourceManager::Get().AddResource("testMat", material);
	 
	get - resource will be returned if found. If not it will try to create it
		  if create is succesfull, return resource, otherwise return no resource
	* std::shared_ptr<RModel> mesh = ResourceManager::Get().GetResource<RModel>("Cube.fbx");
	
	copy - will return a copy of a resource from the manager.
	* std::shared_ptr<RModel> meshCopy = ResourceManager::Get().CopyResource<RModel>("Cube.fbx");

	free - remove any resources that is not in use
	* ResourceManager::Get().FreeResources();


	----Future work (if needed)----
	* If the resource manager is performaning slow - dynamic_pointer_cast
	* Optimize freeresources
*/

class ResourceManager
{
public:	
	/*
		Add a resource to the manager
		Returns true on success and false if it already exist
	*/
	template <class T>
	static bool AddResource(const std::string& filepath, const T& resource);

	/*
		Retrieve any resource with the name (key).
		Second parameter is an option to if you want to create
		the resource if it does not exist. Uses: GResource::Create().
		Uses the appropiate T class when retrieving a resource.
		Returns a shared_ptr that is nullptr if failed
	*/
	template <class T>
	static T* GetResource(const std::string& filepath);

	/*
		Copy a resource with the name (key).
		Return nullptr if the resource does not exist.
	*/
	//template <class T>
	//static T* CopyResource(const std::string& filepath);

	/*
		Removes every resource from the the manager.
		Each resource that is still being used on other places will
		keep living and get destroyed when all is gone
	*/
	static void Destroy();

	/*
		Clearing up all the resources that is not currently being used by anyone
	*/
	//static void FreeResources();

private:
	std::unordered_map<size_t, std::unique_ptr<resource::GResource>> m_resources;

	//Delete copy constructors
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager& rm) = delete;
	ResourceManager(const ResourceManager&& rm) = delete;
	ResourceManager& operator=(const ResourceManager& rm) = delete;
	ResourceManager& operator=(const ResourceManager&& rm) = delete;
	static ResourceManager& Get()
	{
		static ResourceManager s_instance;
		return s_instance;
	}
};

template<class T>
inline bool ResourceManager::AddResource(const std::string& filepath, const T& resource)
{
	size_t key = std::hash<std::string>{}(filepath);

	if (ResourceManager::Get().m_resources.find(key) == ResourceManager::Get().m_resources.end())
	{
		ResourceManager::Get().m_resources.emplace(key, std::make_unique<T>(resource));
		Logger::Log("RM added '" + filepath + "'");
		return true;
	}

	return false;
}

/*
	Tries to get a valid resource, it it's in the store it returns the resource
	otherwise it returns nullptr
*/
template<class T>
inline T* ResourceManager::GetResource(const std::string& filepath)
{
	size_t key = std::hash<std::string>{}(filepath);

	//Check if the resource exists
	auto f = ResourceManager::Get().m_resources.find(key);
	
	//Return the resource if it exists
	if (f != ResourceManager::Get().m_resources.end())
	{
		Logger::Log("RM successfully returned existing resource.");
		
		// Cast the raw-ptr to its intended type
		return dynamic_cast<T*>(f->second.get());
	}

	return nullptr;
}

inline void ResourceManager::Destroy()
{
	for (auto it = ResourceManager::Get().m_resources.begin(); it != ResourceManager::Get().m_resources.end();)
	{
		it = ResourceManager::Get().m_resources.erase(it);
		Logger::Log("RM removed resource.");
	}

	Logger::Log("RM Resources left: " + std::to_string(ResourceManager::Get().m_resources.size()));
	ResourceManager::Get().m_resources.clear();
}