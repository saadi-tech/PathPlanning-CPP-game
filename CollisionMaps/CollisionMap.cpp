#include "CollisionMap.h"
#include "Cell.h"

#include <iostream>

CollisionMap::CollisionMap(
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
   const CellMap& cell_map) :
   m_name(name),
   m_scale_x(scale_x),
   m_scale_y(scale_y),
   m_start_x(start_x),
   m_start_y(start_y),
   m_end_x(end_x),
   m_end_y(end_y),
   m_width(std::abs(start_x - end_x)),
   m_height(std::abs(start_y - end_y)),
   m_cell_length(cell_length),
   m_cell_perimeter(cell_length * 4),
   m_row_size1(row_size1),
   m_row_size2(row_size2),
   m_real_row_size(std::max(row_size1, row_size2)),
   m_u1(u1),
   m_cell_map(cell_map)
{
    m_cell_max_width = static_cast<int>(m_cell_map.size());
    m_cell_max_height = 0;
    for (int x = 0; x < m_cell_max_width; x++)
    {
        int h = static_cast<int>(m_cell_map[x].size());
        if (h > m_cell_max_height)
            m_cell_max_height = h;
    }
}

bool CollisionMap::IsInside(
   const Location& location) const
{
   return IsInside(location.m_x, location.m_y);
}

bool CollisionMap::IsInside(
   const int x,
   const int y) const
{
   if (x >= m_start_x && x <= m_end_x)
   {
      if (y >= m_start_y && y <= m_end_y)
      {
         return true;
      }
   }

   return false;
}

bool CollisionMap::IsValidLocation(
   const Location& location) const
{
   return IsValidLocation(location.m_x, location.m_y);
}

bool CollisionMap::IsValidLocation(
   const int x,
   const int y) const
{
    const int cell_x = GetCellXFromLocation(x);
    const int cell_y = GetCellYFromLocation(x, y);

    if (cell_x >= 0 && cell_y >= 0)
    {
        if (m_cell_map[cell_x][cell_y] == 0x00)
        {
            return true;
        }
    }

    return false;
}

bool CollisionMap::IsValidCell(
    const int x,
    const int y) const
{
    if (x < 0 || x >= m_cell_map.size())
        return false;
    else if (y < 0 || y >= m_cell_map[x].size())
        return false;

    return m_cell_map[x][y] == 0x00;
}

// Used to select a VALID location within max_move_distance of origin_location, in any direction.
const Location CollisionMap::GetNextWanderLocation(
   const Location& origin_location,
   const int max_move_distance) const
{   

   // Randomly pick a cell amoung all the cells around origin_location up to the cell that does not exceed the max_move_distance.
   // Once in the cell, pick a random location within it... (Convert to real world location, calculate the cells end points, pick a random coordinate inside the square)

   return origin_location;
}

// Used to select a VALID location within max_move_distance of origin_location, in the direction of the target_location.
const Location CollisionMap::GetNextLocationToMoveTo(
   const Location& origin_location,
   const Location& target_location,
   const int max_move_distance) const
{   
    
    
   // Get each locations 2D Array coordinate.
   // Use a search algorithm on the array (such as BFS) to find the path (if any exists) to the desired location.
   // Only move the number of cells that combine to being <= max_move_distance.
   // Once in the cell, pick a random location within it... (Convert to real world location, calculate the cells end points, pick a random coordinate inside the square)
   // If none exists, return the current origin_location (monster code will choose what to do from there).
   return origin_location;
}


void CollisionMap::PathPlanning(const Location& origin_location,
    const Location& target_location ) {
    std::vector<Cell> list;


    
}
// Used to determine if the origin_location has a clear line of site to the target_location (no invalid cells blocking the way).
bool CollisionMap::IsLocationVisible(
   const Location& origin_location,
   const Location& target_location) const
{   
    //return false;
    //std::cout << GetCellXFromLocation(origin_location) << "," << GetCellYFromLocation(origin_location) << "  to  " << GetCellXFromLocation(target_location) << "," << GetCellYFromLocation(target_location) << std::endl;

    /*
    int x1 = origin_location.m_x;
    int y1 = origin_location.m_y;
    int x2 = target_location.m_x;
    int y2 = target_location.m_y;
    */
    int x1 = GetCellXFromLocation(origin_location.m_x);
    int y1 = GetCellYFromLocation(origin_location);
    int x2 = GetCellXFromLocation(target_location.m_x);
    int y2 = GetCellYFromLocation(target_location);

    if (x1 == x2 && y1 == y2) {
        //same location, return 1;
        return true;
    }

    if (x1 != x2) {
        int inc_X = 1;

        if (x1 >= x2)
            inc_X = -1;




        double slope = (y2 - y1) / (double)(x2 - x1);

        //std::cout << "Slope: " << slope << std::endl;
        //std::cout << x1 << "," << x2 << std::endl;

        for (int i = x1; abs(i - x2) > 1; i = i + inc_X) {
            int j = (int)(slope * (i - x1) + y1);
            //std::cout << "Check: " << i << "," << j << std::endl;



            //if (!IsValidCell(GetCellXFromLocation(i), GetCellYFromLocation(i, j))){
            if (!IsValidCell(i, j)) {
                std::cout << "NOT valid" << std::endl;
                return false;
            }
            
        }

        return true;

    }
    else {
        int inc_Y = 1;

        if (y1 >= y2)
            inc_Y = -1;




        double slope = (x2 - x1) / (double)(y2 - y1);

        //std::cout << "Slope: " << slope << std::endl;
        

        for (int j = y1; abs(j - y2) > 1; j = j + inc_Y) {
            int i = (int)(slope * (j - y1) + x1);
            //std::cout << "Check: " << i << "," << j << std::endl;



            //if (!IsValidCell(GetCellXFromLocation(i), GetCellYFromLocation(i, j))){
            if (!IsValidCell(i, j)) {
                std::cout << "NOT Visible" << std::endl;
                return false;
            }

        }

        return true;
    }

}

const int CollisionMap::GetCellXFromLocation(
   const Location& location) const
{
   return GetCellXFromLocation(location.m_x);
}

const int CollisionMap::GetCellXFromLocation(
   const int x) const
{
   const int cell_x = static_cast<int>(std::floor((x - m_start_x) / m_cell_perimeter));
   if (cell_x >= 0 && cell_x < static_cast<int>(m_cell_map.size()))
   {
      return cell_x;
   }

   return -1;
}

const int CollisionMap::GetCellYFromLocation(
   const Location& location) const
{
   return GetCellYFromLocation(location.m_x, location.m_y);
}

const int CollisionMap::GetCellYFromLocation(
   const int x,
   const int y) const
{
   const int cell_x = GetCellXFromLocation(x);
   if (cell_x >= 0)
   {
      const int cell_y = static_cast<int>(std::floor((y - m_start_y) / m_cell_perimeter));
      if (cell_y >= 0 && cell_y < static_cast<int>(m_cell_map[cell_x].size()))
      {
         return cell_y;
      }
   }

   return -1;
}

const int CollisionMap::GetLocationXFromCell(
   const int cell_x) const
{
   int location_x = m_start_x;
   
   if (cell_x >= 0 && cell_x < static_cast<int>(m_cell_map.size()))
   {
      location_x += (cell_x * m_cell_perimeter);
   }

   return location_x;
}

const int CollisionMap::GetLocationYFromCell(
   const int cell_x,
   const int cell_y) const
{
   int location_y = m_start_y;

   if (cell_x >= 0 && cell_x < static_cast<int>(m_cell_map.size()))
   {
      if (cell_y >= 0 && cell_y < static_cast<int>(m_cell_map[cell_x].size()))
      {
         location_y += (cell_y * m_cell_perimeter);
      }
   }

   return location_y;
}

const Location CollisionMap::GetLocationFromCell(
   const int cell_x,
   const int cell_y) const
{
   const Location location(GetLocationXFromCell(cell_x), GetLocationYFromCell(cell_x, cell_y));

   return location;
}