# Retile and Clip large LAZ files

**Retile** large LAZ files to smaller tiles and **Clip** 3D LiDAR point clouds of LAS/LAZ format with polygons of ESRI _shapefiles_.

## Table of Contents

- [Overview](#overview)
- [File Structure](#file-structure)
- [Requirements](#requirements)
- [Usage Instructions](#usage-instructions)
- [License](#license)
- [Contact](#contact)

## Overview

This repository holds the C++ scripts of re-tiling large LiDAR LAS/LAZ files and clipping of those LAS/LAZ files w.r.t. the polygons in ESRI shapefiles format.

---

## File Structure

```plaintext
project_directory/
│
├── 3rd_party/      # The third party software, i.e. LAStools, shapelib.
|
├── clipping/      # Holds the clipping scripts.
|      ├── src/    # Source codes.
|      |
|      └── CMakeLists.txt   # Making configuration using CMake.
|
└── retiling/                                 # Directory holds the source codes
    ├── retile_by_count/                      # C++ scripts of retile by number of tiles
    |        ├── src/                         # Source codes
    |        └── CMakeLists.txt               # Configuration using CMake tool
    └── retile_by_size/                       # C++ scripts of retile by size in two directions
             ├── src/                         # Source codes
             └── CMakeLists.txt               # Configuration using CMake tool
```

## Requirements

The `C++` scripts in this repository depends on the **[LAStools](https://lastools.github.io/)** to read point cloud data in **LAS/LAZ** formats and the **[shapelib](http://shapelib.maptools.org/)** for reading **ESRI** shapefiles.

To use the scripts, a `C++` compiler, i.e. `g++`,`gcc`, `mscv`, `clang++`, etc., should be installed.

## Usage Instructions

### There are two ways to build:

- Option 1: Using CMake to generate makefiles and then 'make' (on Linux/macOS).

  - On Linux or maxOS, simply:
    ```
    $ cd path-to-dir
    $ mkdir release  && cd release
    $ cmake -DCMAKE_BUILD_TYPE=release ..
    $ make
    ```

- Option 2: Use any IDE that can directly handle CMakeLists files to open the `CMakeLists.txt` in the **root** directory of Trees3D.
  Then you should have obtained a usable project and just build it. I recommend using
  [CLion](https://www.jetbrains.com/clion/) or [QtCreator](https://www.qt.io/product). For Windows users: your IDE must be set for `x64`.

### Module 1: [clipping]

This module clips the LiDAR point clouds in LAS/LAS format w.r.t. the polygons (with *.shp format).

There are two parameters to specify:

```
    std::string shp_file_path:  The full path to the shapefile;
    std::string las_dir:        The full path to the directory of LAS/LAZ files;
```

```javascript {.line-numbers}
int main(int argc, char **argv)
{
    // Specify the directory to the shapefile [*.shp] file;
    std::string shpFilePath = "/directory/to/shapefile.shp";
    // Specify the directory to the LAS/LAZ files;
    std::string lasDir = "/directory/to/las/files/";

    // Instantiate the clipping object;
    mm::ClipLas *clip = new mm::ClipLas;

    clip->getAllLasFiles(lasDir);

    clip->setShpFilePath(shpFilePath);
    clip->readShpFile();
    clip->getPositivePolygons();
    clip->setLasFileDirName(lasDir);
    clip->runClipping();

    // Clean the memory;
    if (clip)
    {
        delete clip;
        clip = nullptr;
    }

    return 0;
}

```

### Module 2: [retiling]

This module performs the re-tiling of LARGE LAS/LAZ files, which provides two sub-modules: 
1. [retile_by_count] - This module re-tiles the original LAS/LAZ file with the given number of new tiles in **X** and **Y** directions. 
2. [retile_by_size] - This module re-tiels the original LAS/LAZ file with the given size of new tiles in **X** and **Y** directions.

There are three parameters for each of the two sub-modules.

In case of [retile_by_count]:

```
    std::string in_las_dir: The full path to the directory where the LARGE LAS/LAZ files are.
    const int num_x:        The specified number of new tiles in **X** direction.
    const int num_y:        The specified number of new tiles in **Y** direction.
```

and in case of [retile_by_size]:

```
    std::string in_las_dir: The full path to the directory where the LARGE LAS/LAZ files are.
    const float sizeX:        The specified length of new tiles in **X** direction.
    const float sizeY:        The specified length of new tiles in **Y** direction.
```

Below shows two examples on the usage of Module [retiling] using [retile_by_count] and [retile_by_size] respectively:

```javascript {.line-numbers}

[retile_by_count]


int main()
{
    // Specify the directory to the LARGE LAS/LAZ files;
    std::string inLasDir = "/dir/to/las/files/";

    // Specify the number of new tiles in the two axes directions;
    const int numX = 40;
    const int numY = 40;


    mm::Retile *retile = new mm::Retile;
    retile->setInputDir(inLasDir);
    retile->setRetileCount(numX, numY);
    retile->retileByCount();

    // Clean memory;
    if (retile)
    {
        delete retile;
        retile = nullptr;
    }
    return 0;
}


[retile_by_size]

int main()
{
    mm::Retile *retile = new mm::Retile;

    // Specify the directory to the LARGE LAS/LAZ files;
    retile->setInputDir("/dir/to/las/files/");

    // Specify the size of new tiles in the two axes directions;
    const float sizeX = 500.0f;
    const float sizeY = 500.0f;

    retile->setRetileSize(sizeX, sizeY);
    retile->retileBySize();

    // Clean memory;
    if (retile)
    {
        delete retile;
        retile = nullptr;
    }

    std::cout << "The retiling is done." << std::endl;

    return 0;
}
```

## License

MIT License

## Contact

For any suggestions and bug reports, please contact:

Jinhu Wang

jinhu.wang (at) hotmail (dot) com
