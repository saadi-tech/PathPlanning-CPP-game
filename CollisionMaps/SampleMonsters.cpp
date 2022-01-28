#include "SampleMonsters.h"
#include "CollisionMapCollection.h"
#include "CollisionMap.h"
#include "Location.h"

#include <iostream>

static SampleMonsters* s_pInstance = 0;

SampleMonsters& SampleMonsters::GetInstance()
{
   if (!s_pInstance)
   {
      printf("SampleMonsters::GetInstance(): Instance doesn't exist");
      exit(1);
   }

   return *s_pInstance;
}

SampleMonsters::SampleMonsters()
{ 
   if (s_pInstance)
   {
      printf("SampleMonsters::SampleMonsters(): Only one instance can exist.");
      exit(1);
   }

   s_pInstance = this;

   //m_monsters.push_back(Monster(0, "ExpectedVillage", -302573, 825687, 100));
   m_monsters.push_back(Monster(1, "ExpectedValid1", -446895, 209885, -9300));
   //m_monsters.push_back(Monster(1, "ExpectedValid2", -445295, 200884, -9300));
   //m_monsters.push_back(Monster(1, "ExpectedValid2", -445295, 200884, -9300)); //invisible
   m_monsters.push_back(Monster(1, "ExpectedValid2", -447295, 200084, -9300)); //visible
   //m_monsters.push_back(Monster(2, "ExpectedMaybedInValid", -448568, 207672, 300));
   //m_monsters.push_back(Monster(3, "ExpectedInValid", -455168, 195072, 400));
   //m_monsters.push_back(Monster(4, "ExpectedNone", 0, 0, 500));
}

void SampleMonsters::OutputInitialLocationInfo()
{
   for (size_t i = 0; i < m_monsters.size(); i++)
   {
      const Location monster_location = m_monsters[i].GetLocation();

      const CollisionMap* collision_map = CollisionMapCollection::GetInstance().GetCollisionMap(monster_location);
      if (collision_map)
      {
         if (collision_map->IsValidLocation(monster_location))
         {
            std::cout << "Monster (" << std::to_string(m_monsters[i].m_id) << "-" << m_monsters[i].m_name << "): " << "Valid Collision Location (" << collision_map->m_name << ")" << std::endl;
         }
         else
         {
            std::cout << "Monster (" << std::to_string(m_monsters[i].m_id) << "-" << m_monsters[i].m_name << "): " << "Invalid Collision Location (" << collision_map->m_name << ")" << std::endl;
         }
      }
      else
      {
         std::cout << "Monster (" << std::to_string(m_monsters[i].m_id) << "-" << m_monsters[i].m_name << "): " << "No Collision Map" << std::endl;
      }
   }
}

Monster* SampleMonsters::GetMonsterByName(
   const std::string& monster_name)
{
    for (size_t i = 0; i < m_monsters.size(); i++)
    {
        if (m_monsters[i].m_name == monster_name)
            return &m_monsters[i];
    }

    return NULL;
}
