#include "Monster.h"
#include "CollisionMapCollection.h"
#include "CollisionMap.h"
#include "SampleMonsters.h"
#include "CellMonster.h"

#include <iostream>

Monster::Monster(
   const int id,
   const std::string& name,
   const int x,
   const int y,
   const int move_rate) :
   m_id(id),
   m_name(name),
   m_collision_map(CollisionMapCollection::GetInstance().GetCollisionMap(x, y)),
   m_original_location(x, y),
   m_location(x, y),
   m_move_rate(move_rate),
   m_render_object(NULL)
{
   
}

const Location Monster::GetLocation() const
{
   return m_location;
}

const int Monster::GetCellX() const
{
   if (m_collision_map)
      return m_collision_map->GetCellXFromLocation(m_location.m_x);
   else
      return 0;
}

const int Monster::GetCellY() const
{
   if (m_collision_map)
      return m_collision_map->GetCellYFromLocation(m_location.m_x, m_location.m_y);
   else
      return 0;
}

bool Monster::CanDraw() const {

   return m_collision_map != NULL;
}

bool Monster::InValidCell() const {

   if (m_collision_map)
   {
      return m_collision_map->IsValidLocation(m_location);
   }

   return false;
}

void Monster::Roam()
{
   if (m_collision_map)
   {
      // If monster is stuck inside an invalid cell, don't roam.
      if (!m_collision_map->IsValidLocation(m_location))
         return;

      const Location next_location = m_collision_map->GetNextWanderLocation(m_location, m_move_rate);

      SetLocation(next_location);
   }

   ValidateOrUpdateCollisionMap();
}

void Monster::Chase(
   const Location& target_location)
{
   if (m_collision_map)
   {
      const Location next_location = m_collision_map->GetNextLocationToMoveTo(m_location, target_location, m_move_rate);

      SetLocation(next_location);
   }

   ValidateOrUpdateCollisionMap();
}

void Monster::ReturnHome()
{
   if (m_collision_map)
   {
      const Location next_location = m_collision_map->GetNextLocationToMoveTo(m_location, m_original_location, m_move_rate);

      SetLocation(next_location);
   }

   ValidateOrUpdateCollisionMap();
}

bool Monster::CanView(
   const Location& target_location) const
{
   if (m_collision_map)
   {
      return m_collision_map->IsLocationVisible(m_location, target_location);
   }

   return false;
}

void Monster::ValidateOrUpdateCollisionMap()
{
   if (m_collision_map == NULL || !m_collision_map->IsInside(m_location))
   {
      m_collision_map = CollisionMapCollection::GetInstance().GetCollisionMap(m_location);
   }
}

void Monster::SetLocation(
   const Location& new_location)
{
   m_location.m_x = new_location.m_x;
   m_location.m_y = new_location.m_y;

   // Iterate over all monsters and notify if we can see them or can no longer see them...
   // This is for demo purposes...
   const SampleMonsters& sample_monsters = SampleMonsters::GetInstance();
   for (size_t i = 0; i < sample_monsters.m_monsters.size(); i++)
   {
      if (sample_monsters.m_monsters[i].m_name != m_name)
      {
         if (CanView(sample_monsters.m_monsters[i].GetLocation()))
         {
            m_visible_monster_ids.insert(sample_monsters.m_monsters[i].m_id);
            std::cout << m_name << " can now see " << sample_monsters.m_monsters[i].m_name<<std::endl;
         }
         else if (m_visible_monster_ids.contains(sample_monsters.m_monsters[i].m_id))
         {
            m_visible_monster_ids.erase(sample_monsters.m_monsters[i].m_id);
            std::cout << m_name << " can NO LONGER see " << sample_monsters.m_monsters[i].m_name << std::endl;
         }
      }
   }

   if (m_render_object != nullptr)
      m_render_object->UpdatePosition();
}