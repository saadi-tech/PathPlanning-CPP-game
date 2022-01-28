#include "CollisionMapCollection.h"
#include "CollisionMap.h"
#include "Location.h"

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#pragma hdrstop

// Marko C.
// Comment this (For the demo only) to read the map folder from the exe's path.
// Uncomment if you're debugging from the IDE.
#define MAPS_FOLDER_DEBUG

#define COMPILE_FOR_64BIT

static CollisionMapCollection* s_pInstance = 0;

CollisionMapCollection& CollisionMapCollection::GetInstance()
{
   if (!s_pInstance)
   {
      printf("CollisionMapCollection::GetInstance(): Instance doesn't exist");
      exit(1);
   }

   return *s_pInstance;
}

CollisionMapCollection::CollisionMapCollection() :
   m_collision_maps(LoadCollisionMaps())
{
   if (s_pInstance)
   {
      printf("CollisionMapCollection::CollisionMapCollection(): Only one instance can exist.");
      exit(1);
   }

   s_pInstance = this;
}

CollisionMapCollection::~CollisionMapCollection()
{
   for (CollisionMaps::const_iterator it = m_collision_maps.begin(); it != m_collision_maps.end(); it++)
   {
      delete *it;
   }
}

const CollisionMap* CollisionMapCollection::GetCollisionMap(
   const Location& location) const
{
   return GetCollisionMap(location.m_x, location.m_y);
}

const CollisionMap* CollisionMapCollection::GetCollisionMap(
   const int x,
   const int y) const
{
   for (CollisionMaps::const_iterator it = m_collision_maps.begin(); it != m_collision_maps.end(); it++)
   {
      if ((*it)->IsInside(x, y))
      {
         return *it;
      }
   }

   return NULL;
}

const int CollisionMapCollection::GetIntFromFile(
   std::ifstream& file_stream) const
{
   int value;
   file_stream.read(reinterpret_cast<char*>(&value), sizeof(value));

   return value;
}

const CollisionMapCollection::CollisionMaps CollisionMapCollection::LoadCollisionMaps() const
{
   CollisionMaps collision_maps;

#ifdef MAPS_FOLDER_DEBUG
   const std::filesystem::path current_path = std::filesystem::current_path();
   std::cout << "Current Path: " << current_path.string() << std::endl;
#else
   std::filesystem::path current_path = std::filesystem::current_path();
   current_path += "\\Maps";
#endif

#ifdef MAPS_FOLDER_DEBUG
#ifdef COMPILE_FOR_64BIT
   const std::string relative_path = "..\\x64\\Debug\\Maps\\";
#else
   const std::string relative_path = "..\\Debug\\Maps\\";
#endif

   if (std::filesystem::exists(relative_path))
#else
   if (std::filesystem::exists(current_path))
#endif
   {
#ifdef MAPS_FOLDER_DEBUG
      for (const auto& entry : std::filesystem::directory_iterator(relative_path))
#else
      for (const auto& entry : std::filesystem::directory_iterator(current_path))
#endif
      {
         const std::string file_name = entry.path().stem().string();
         const std::string file_path = entry.path().string();

         std::ifstream file_stream;

         file_stream.open(file_path, std::ios::binary);
         if (file_stream.is_open())
         {
            int scale_x = GetIntFromFile(file_stream);
            int scale_y = GetIntFromFile(file_stream);
            int x1 = GetIntFromFile(file_stream);
            int y1 = GetIntFromFile(file_stream);
            int x2 = GetIntFromFile(file_stream);
            int y2 = GetIntFromFile(file_stream);
            int cell_length = GetIntFromFile(file_stream);
            int row_size1 = GetIntFromFile(file_stream);
            int row_size2 = GetIntFromFile(file_stream);
            int u1 = GetIntFromFile(file_stream);

            std::vector<char> file_contents((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
            file_stream.close();

            
            CollisionMap::CellMap cell_map;
            const int row_size_to_use = std::max(row_size1, row_size2);
            while (!file_contents.empty())
            {
               int row_size = std::min(static_cast<int>(file_contents.size()), row_size_to_use);

               CollisionMap::CellRow cell_row(file_contents.begin(), file_contents.begin() + row_size);
               file_contents.erase(file_contents.begin(), file_contents.begin() + row_size);

               cell_map.push_back(cell_row);
            }


            CollisionMap* collision_map = new CollisionMap(
               file_name,
               scale_x,
               scale_y,
               x1,
               y1,
               x2,
               y2,
               cell_length,
               row_size1,
               row_size2,
               u1,
               cell_map);
            
            collision_maps.push_back(collision_map);
            std::cout << "Loaded CollisionMap: " << entry.path() << std::endl;
         }
         else
         {
            std::cout << "Failed to Load CollisionMap: " << entry.path() << std::endl;
         }
      }
   }
   else
   {
#ifdef MAPS_FOLDER_DEBUG
       std::cout << "Collision Map Files missing, expected at '" << current_path.string() << "\\" << relative_path << "'." << std::endl;
#else
      std::cout << "Collision Map Files missing, expected at '" << current_path.string() << "\\Maps" << "'." << std::endl;
#endif
   }

   return collision_maps;
}