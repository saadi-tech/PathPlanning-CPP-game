#pragma once

struct Location
{
public:
   Location()
   {
      m_x = 0;
      m_y = 0;
      m_z = 0;
   }

   Location(
      const int x,
      const int y,
      const int z = 0)
   {
      m_x = x;
      m_y = y;
      m_z = z;
   }

   int m_x;
   int m_y;
   int m_z;
};