//
// Created by pavel on 6/2/20.
//

#ifndef BATTLECITY_RESOURCEHOLDER_HPP
#define BATTLECITY_RESOURCEHOLDER_HPP

#include <SFML/Graphics/Rect.hpp>

template<typename Identifier, typename Resource>
class ResourceHolder
{
public:
  void LoadFromFile(Identifier, const std::string&);
  void Insert(Identifier, const Resource&);
  std::shared_ptr<Resource> Get(Identifier) const;

private:
  std::unordered_map<Identifier, std::shared_ptr<Resource>> mResourceMap;
};

template<typename Identifier, typename T>
class ResourceHolder<Identifier, std::vector<T>>
{
public:
  void LoadFromFile(Identifier, const std::string&, const std::vector<sf::IntRect>&);
  void Insert(Identifier, const std::vector<T>&);
  std::shared_ptr<std::vector<T>> Get(Identifier) const;

private:
  std::unordered_map<Identifier, std::shared_ptr<std::vector<T>>> mResourceMap;
};


template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::LoadFromFile(Identifier aId, const std::string& aFilename)
{
  if (mResourceMap.find(aId) == mResourceMap.end())
  {
    Resource resource;
    if (resource.loadFromFile(aFilename))
    {
      Insert(aId, resource);
    }
    else
    {
      SPDLOG_ERROR("Failed to load from file");
    }
  }
  else
  {
    SPDLOG_WARN("Resource: {} is already loaded", aId._to_string());
  }
}

template<typename Identifier, typename T>
void ResourceHolder<Identifier, std::vector<T>>::LoadFromFile(Identifier aId, const std::string& aFilename, const std::vector<sf::IntRect>& aRec)
{
  if (mResourceMap.find(aId) == mResourceMap.end())
  {
    for(auto i = 0u; i < aRec.size(); ++i)
    {
      T resource;
      if (resource.loadFromFile(aFilename, aRec[i]))
      {
        Insert(aId, {resource});
      }
      else
      {
        SPDLOG_ERROR("Failed to load from file");
      }
    }
  }
  else
  {
    SPDLOG_WARN("Resource: {} is already loaded", aId._to_string());
  }
}

template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::Insert(Identifier aId, const Resource& aRes)
{
  mResourceMap.insert(std::make_pair(aId, std::make_shared<Resource>(aRes)));
}

template<typename Identifier, typename T>
void ResourceHolder<Identifier, std::vector<T>>::Insert(Identifier aId, const std::vector<T>& aRes)
{
  auto it = mResourceMap.find(aId);
  if (it != mResourceMap.end() && it->second)
  {
    it->second->insert(it->second->end(), aRes.begin(), aRes.end());
  }
  else
  {
    mResourceMap.insert(std::make_pair(aId, std::make_shared<std::vector<T>>(aRes)));
  }
  
}

template<typename Identifier, typename Resource>
std::shared_ptr<Resource> ResourceHolder<Identifier, Resource>::Get(Identifier aId) const
{
  auto it = mResourceMap.find(aId);
  if (it == mResourceMap.end())
  {
    SPDLOG_ERROR("Resource doesn't exist");
    return nullptr;
  }
  else
  {
    return it->second;
  }
}

template<typename Identifier, typename T>
std::shared_ptr<std::vector<T>> ResourceHolder<Identifier, std::vector<T>>::Get(Identifier aId) const
{
  auto it = mResourceMap.find(aId);
  if (it == mResourceMap.end())
  {
    SPDLOG_ERROR("Resource doesn't exist");
    return nullptr;
  }
  else
  {
    return it->second;
  }
}

#endif // BATTLECITY_RESOURCEHOLDER_HPP
