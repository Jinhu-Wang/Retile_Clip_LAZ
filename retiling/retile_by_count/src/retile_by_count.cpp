
#include "retile_las.h"
#include <iostream>
#include <string>

void usage(char *argv)
{
    std::cout << "Usage:" << std::endl;
    std::cout << argv << " [InputDirectory] [count_X] [count_Y] " << std::endl;
}

int main11(int argc, char **argv)
{
    if (argc <= 1 || argc != 4)
    {
        usage(argv[0]);
        return 1;
    }

    std::string inputDir = argv[1];
    const int numX = std::stoi(argv[2]);
    const int numY = std::stoi(argv[3]);

    std::cout << "The input directory containing las/laz files is:" << argv[1] << std::endl;
    std::cout << "The las/laz files will be retiled by count: " << argv[2] << " " << argv[3] << std::endl;

    mm::Retile *retile = new mm::Retile;

    retile->setInputDir(argv[1]);
    retile->setRetileCount(numX, numY);
    retile->retileByCount();
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