
#include "retile_las.h"
#include <iostream>
#include <string>

void usage(char *argv)
{
    std::cout << "Usage:" << std::endl;
    std::cout << argv << " [InputDirectory] [sizeX] [sizeY] " << std::endl;
}

int main()
{
    mm::Retile *retile = new mm::Retile;

    retile->setInputDir("/home/jinhu/Work/Data/2023.06.27-AHN4/raw1_1");
    const float sizeX = 500.0f;
    const float sizeY = 500.0f;

    retile->setRetileSize(sizeX, sizeY);

    retile->retileBySize();

    if (retile)
    {
        delete retile;
        retile = nullptr;
    }
    std::cout << "The retiling is done." << std::endl;

    return 0;
}