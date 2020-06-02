//
// Created by pavel on 6/2/20.
//

#ifndef BATTLECITY_RESOURCEHOLDER_HPP
#define BATTLECITY_RESOURCEHOLDER_HPP

#include <map>
#include <memory>
#include <utility>
#include <spdlog/spdlog.h>

enum class SpriteId
{
   MAIN_TANK1 = 0,
   MAIN_TANK2
};

enum class Textures
{
   MAIN_SHEET = 0
};

template<typename Identifier, typename Resource>
class ResourceHolder
{
public:
   void load(Identifier id, const std::string &filename);
   const Resource &get(Identifier id) const;

private:
   void insertResource(Identifier id, std::unique_ptr<Resource> resource);

private:
   std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};


template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::load(Identifier id, const std::string &filename)
{
   std::unique_ptr<Resource> resource(new Resource());

   assert(resource->loadFromFile(filename));

   insertResource(id, std::move(resource));
}

template<typename Identifier, typename Resource>
const Resource &ResourceHolder<Identifier, Resource>::get(Identifier id) const
{
   auto it = mResourceMap.find(id);

   assert(it != mResourceMap.end());

   return *it->second;
}

template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
   auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
   if (!inserted.second)
   {
      spdlog::warn("Resource already exist");
   }
}


#endif// BATTLECITY_RESOURCEHOLDER_HPP
