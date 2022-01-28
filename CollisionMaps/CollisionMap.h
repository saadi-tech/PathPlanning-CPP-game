#pragma once

#include "Location.h"

#include <string>
#include <vector>

class CollisionMap
{
   friend class Monster;

public:
   typedef char BYTE;
   typedef std::vector<BYTE> CellRow;
   typedef std::vector<CellRow> CellMap;

   CollisionMap(
      const std::string& name,
      const int scale_x,
      const int scale_y,
      const int start_x,
      const int start_y,
      const int end_x,
      const int end_y,
      const int cell_length,
      const int row_size1,
      const int row_size2,
      const int u1,
      const CellMap& cell_map);

   bool IsInside(
      const Location& location) const;

   bool IsInside(
      const int x,
      const int y) const;

   bool IsValidLocation(
      const Location& location) const;

   bool IsValidLocation(
      const int x,
      const int y) const;

   bool IsValidCell(
       const int x,
       const int y) const;

   // Used to select a VALID location within max_move_distance of origin_location, in any direction.
   const Location GetNextWanderLocation(
      const Location& origin_location,
      const int max_move_distance) const;

   // Used to select a VALID location within max_move_distance of origin_location, in the direction of the target_location.
   const Location GetNextLocationToMoveTo(
      const Location& origin_location,
      const Location& target_location,
      const int max_move_distance) const;

   // Used to determine if the origin_location has a clear line of site to the target_location (no invalid cells blocking the way).
   bool IsLocationVisible(
      const Location& origin_location,
      const Location& target_location) const;

   void PathPlanning(const Location& origin_location,
       const Location& target_location);

private:
   const int GetCellXFromLocation(
      const Location& location) const;

   const int GetCellXFromLocation(
      const int x) const;

   const int GetCellYFromLocation(
      const Location& location) const;

   const int GetCellYFromLocation(
      const int x,
      const int y) const;

   const int GetLocationXFromCell(
      const int cell_x) const;

   const int GetLocationYFromCell(
      const int cell_x,
      const int cell_y) const;

   const Location GetLocationFromCell(
      const int cell_x,
      const int cell_y) const;

public:
   const std::string m_name;


private:
   const int m_scale_x;
   const int m_scale_y;
   const int m_start_x;
   const int m_start_y;
   const int m_end_x;
   const int m_end_y;
   const int m_width;
   const int m_height;
   const int m_cell_length;
   const int m_cell_perimeter;
   const int m_row_size1;
   const int m_row_size2;
   const int m_real_row_size;
   const int m_u1;

public:
   CellMap m_cell_map;

   // Marko C.
   int m_cell_max_width;
   int m_cell_max_height;
};

