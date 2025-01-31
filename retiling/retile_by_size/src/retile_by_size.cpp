
#include "retile_las.h"
#include <iostream>
#include <string>

void usage(char *argv)
{
    std::cout << "Usage:" << std::endl;
    std::cout << argv << " [InputDirectory] [sizeX] [sizeY] " << std::endl;
}

int main111(int argc, char **argv)
{
    if (argc <= 1 || argc != 4)
    {
        usage(argv[0]);
        return 1;
    }

    std::string inputDir = argv[1];
    const double sizeX = std::stod(argv[2]);
    const double sizeY = std::stod(argv[3]);

    std::cout << "The input directory containing las/laz files is:" << argv[1] << std::endl;
    std::cout << "The las/laz files will be retiled by size: " << argv[2] << " " << argv[3] << std::endl;

    mm::Retile *retile = new mm::Retile;

    retile->setInputDir(argv[1]);
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

int main()
{
    mm::Retile *retile = new mm::Retile;

    retile->setInputDir("/home/jinhu/Work/Data/2023.06.27-AHN4/raw1_1");
    retile->setRetileSize(500, 500);
    retile->retileBySize();

    if (retile)
    {
        delete retile;
        retile = nullptr;
    }
    std::cout << "The retiling is done." << std::endl;

    return 0;
}