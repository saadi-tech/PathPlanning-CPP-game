#pragma once

#include "Monster.h"

#include <vector>

class SampleMonsters
{
public:
   static SampleMonsters& GetInstance();

   SampleMonsters();

   void OutputInitialLocationInfo();

   // Marko C.
   Monster* GetMonsterByName(
      const std::string& monster_name);

private:
   friend class Monster;

   std::vector<Monster> m_monsters;
};
