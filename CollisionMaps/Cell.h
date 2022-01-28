#pragma once

struct Cell
{
public:
    Cell()
    {
        x = 0;
        y = 0;
    }

    Cell(
        const int _x,
        const int _y)
    {
        x = _x;
        y = _y;
    }

    int x;
    int y;
    

};
