
#include "retile_las.h"
#include <iostream>
#include <string>

void usage(char *argv)
{
    std::cout << "Usage:" << std::endl;
    std::cout << argv << " [InputDirectory] [count_X] [count_Y] " << std::endl;
}



int main()
{
    std::string inLasDir = "/home/jinhu/Work/Data/2023.04.19-MAMBO/NL/LiDAR/retiled";
    const int numX = 40;
    const int numY = 40;
    mm::Retile *retile = new mm::Retile;
    retile->setInputDir(inLasDir);
    retile->setRetileCount(numX, numY);
    retile->retileByCount();

    return 0;
    if (retile)
    {
        delete retile;
        retile = nullptr;
    }
    return 0;
}