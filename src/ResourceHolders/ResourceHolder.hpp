//
// Created by pavel on 6/2/20.
//

#ifndef BATTLECITY_RESOURCEHOLDER_HPP
#define BATTLECITY_RESOURCEHOLDER_HPP

#include <SFML/Graphics/Rect.hpp>
#include "../Utils/Utils.hpp"

template<typename Identifier, typename Resource>
class ResourceHolder
{
public:
  void LoadFromFile(Identifier, const std::string&, const std::vector<sf::IntRect>& aRec = {});
  void Insert(Identifier, const Resource&);
  std::shared_ptr<Resource> Get(Identifier) const;

private:
  std::unordered_map<Identifier, std::shared_ptr<Resource>> mResourceMap;
};

template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::LoadFromFile(Identifier aId, const std::string& aFilename, const std::vector<sf::IntRect>& aRec)
{
  if (mResourceMap.find(aId) == mResourceMap.end())
  {
    if constexpr(Utils::is_vector<Resource>::value)
    {
      for(auto i = 0u; i < aRec.size(); ++i)
      {
        typename Resource::value_type resource;
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
  }
  else
  {
    SPDLOG_WARN("Resource: {} is already loaded", aId._to_string());
  }
}

template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::Insert(Identifier aId, const Resource& aRes)
{
  if constexpr(Utils::is_vector<Resource>::value)
  {
    auto it = mResourceMap.find(aId);
    if (it != mResourceMap.end() && it->second)
    {
      it->second->insert(it->second->end(), aRes.begin(), aRes.end());
    }
    else
    {
      mResourceMap.insert(std::make_pair(aId, std::make_shared<std::vector<typename Resource::value_type>>(aRes)));
    }
  }
  else
  {
    mResourceMap.insert(std::make_pair(aId, std::make_shared<Resource>(aRes)));
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

#endif // BATTLECITY_RESOURCEHOLDER_HPP
