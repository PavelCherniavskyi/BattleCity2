//
// Created by pavel on 6/2/20.
//

#ifndef BATTLECITY_RESOURCEHOLDER_HPP
#define BATTLECITY_RESOURCEHOLDER_HPP

template<typename Identifier, typename Resource>
class ResourceHolder
{
public:
  void load(Identifier id, const std::string &filename);
  std::shared_ptr<Resource> get(Identifier id) const;

private:
  void insertResource(Identifier id, std::shared_ptr<Resource> resource);

private:
  std::map<Identifier, std::shared_ptr<Resource>> mResourceMap;
};


template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::load(Identifier id, const std::string &filename)
{
  Resource resource;

  if (resource.loadFromFile(filename))
  {
    insertResource(id, std::make_shared<Resource>(resource));
  }
  else
  {
    spdlog::error("Failed to load from file");
  }
}

template<typename Identifier, typename Resource>
std::shared_ptr<Resource> ResourceHolder<Identifier, Resource>::get(Identifier id) const
{
  auto it = mResourceMap.find(id);
  if(it == mResourceMap.end())
  {
    spdlog::error("Resource doesn't exist");
    return nullptr;
  }
  else
  {
    return it->second;
  }
}

template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::insertResource(Identifier id, std::shared_ptr<Resource> resource)
{
  auto inserted = mResourceMap.insert(std::make_pair(id, resource));
  if (!inserted.second)
  {
    spdlog::warn("Resource already exist");
  }
}


#endif // BATTLECITY_RESOURCEHOLDER_HPP
