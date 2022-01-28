#pragma once

#include <vector>
#include <string>

class CollisionMap;
struct Location;

class CollisionMapCollection {
public:

   static CollisionMapCollection& GetInstance();

   CollisionMapCollection();

   ~CollisionMapCollection();

   const CollisionMap* GetCollisionMap(
      const Location& location) const;

   const CollisionMap* GetCollisionMap(
      const int x,
      const int y) const;

private:

   typedef std::vector<CollisionMap*> CollisionMaps;

   const int GetIntFromFile(
      std::ifstream& file_stream) const;

   const CollisionMaps LoadCollisionMaps() const;

   const CollisionMaps m_collision_maps;

   // No copies, do not implement
   CollisionMapCollection(const CollisionMapCollection& rhs) = delete;
   CollisionMapCollection& operator=(const CollisionMapCollection& rhs) = delete;
};