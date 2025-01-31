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
|
├── clipping/      # Holds the clipping scripts.
|      ├── src/    # Source codes.
|      |
|      └── CMakeLists.txt   # Making configuration using CMake.
|
|
└── retiling/                                 # Directory holds the source codes
    ├── retile_by_count/                      # C++ scripts of classify trees from reedbeds vegetation
    |        ├── src/                         # Source codes
    |        └── CMakeLists.txt               # Configuration using CMake tool
    └── retile_by_size/                       # C++ scripts of individualize single trees
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
    $ cd path-to-root-dir-of-Trees
    $ mkdir release  && cd release
    $ cmake -DCMAKE_BUILD_TYPE=release ..
    $ make
    ```

- Option 2: Use any IDE that can directly handle CMakeLists files to open the `CMakeLists.txt` in the **root** directory of Trees3D.
  Then you should have obtained a usable project and just build it. I recommend using
  [CLion](https://www.jetbrains.com/clion/) or [QtCreator](https://www.qt.io/product). For Windows users: your IDE must be set for `x64`.

### Module 1: [clipping]

This module clips the LiDAR point clouds in LAS/LAS format w.r.t. the polygons (with \*.shp format).

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

### Module 2: [TreeIndividualization]

This module performs the individual tree delineation from the classified **[trees&shrubs]** points.
There are three parameters:

```
    radius: The neighbourhood size used for neighbourhood searching;
    verticalResolution: The vertical slicing resolution;
    miniPtsPerCluster: The minimum number of points that defines a cluster.
```

Below shows an example on the usage of Module [TreeIndividualization]:

```javascript {.line-numbers}
void main()
{
    /// The directory of the input directory;
    std::string las_dir = "path\to\input\las\files";

    /// The output directory of the output tree individualization;
    std::string out_dir = "path\to\output\las\files";

    /// Get all the las files in the given directory;
    std::vector<std::string> las_files;
    ListFilesInDirectory(las_dir, las_files);

    /// Parameters;

    // The radius for neighbourhood searching.
    const double radius = 2.0;
    // The vertical resolution;
    const double verticalResolution = 0.5;
    // The minimum number of points for a cluster;
    const int miniPtsPerCluster = 10;

    /// Individualize the trees from the point cloud data;
    for (size_t i = 0; i < las_files.size(); ++i)
    {
        std::vector<Point3D> treePts;

        std::string las_file_path = las_dir + las_files[i];
        treePts = readLasFile(las_file_path.c_str());

        FoxTree *foxTree = new FoxTree(treePts, radius, verticalResolution, miniPtsPerCluster);

        foxTree->separateTrees(1, 1);
        std::string outputTree = out_dir + las_files[i] + ".xyz";
        foxTree->outputTrees(outputTree.c_str(), foxTree->m_nTrees);
        std::cout << "File: " << las_files[i] << " finished." << std::endl;

        if (foxTree)
        {
            delete foxTree;
            foxTree = nullptr;
        }
    }

    return;
}

```

## License

MIT License

## Contact

For any suggestions and bug reports, please contact:

Jinhu Wang

jinhu.wang (at) hotmail (dot) com
