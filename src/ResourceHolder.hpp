//
// Created by pavel on 6/2/20.
//

#ifndef BATTLECITY_RESOURCEHOLDER_HPP
#define BATTLECITY_RESOURCEHOLDER_HPP

#include <map>
#include <memory>
#include <spdlog/spdlog.h>

template<typename Resource, typename Identifier>
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


template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string &filename)
{
   std::unique_ptr<Resource> resource(new Resource());
   if (!resource.loadFromFile(filename))
   {
      spdlog::critical("loadFromFile failed");
      abort();
   }
   insertResource(id, std::move(resource));
}

template<typename Resource, typename Identifier>
const Resource &ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
   auto it = mResourceMap.find(id);
   if(it != mResourceMap.end())
   {
      return *it->second;
   }
   else
   {
      spdlog::warn("Resource not found");
      return Resource();
   }
}

template<typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
   auto inserted = mResourceMap.insert(std::make_pair(id, resource));
   if(!inserted)
   {
      spdlog::warn("Resource wasn't inserted");
   }
}


#endif// BATTLECITY_RESOURCEHOLDER_HPP
