#pragma once

#include "Location.h"

#include <string>
#include <set>

class CollisionMap;
class CellMonster;

class Monster
{
public:
   Monster(
      const int id,
      const std::string& name,
      const int x,
      const int y,
      const int move_rate);

   const Location GetLocation() const;

   const int GetCellX() const;

   const int GetCellY() const;

   bool CanDraw() const;

   bool InValidCell() const;

   void Roam();

   void Chase(
      const Location& target_location);

   void ReturnHome();

   bool CanView(
      const Location& target_location) const;

   const int m_id;
   const std::string m_name;

private:
   friend class CellMonster;

   void ValidateOrUpdateCollisionMap();

   void SetLocation(
      const Location& new_location);

   const CollisionMap* m_collision_map;
   const Location m_original_location;
   Location m_location;
   const int m_move_rate;
   std::set<int> m_visible_monster_ids;

   CellMonster* m_render_object;
};
