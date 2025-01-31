#ifndef MM_CLIP_LAS_H
#define MM_CLIP_LAS_H

#include <string>
#include <vector>

#include "shapefil.h"
#include "types.h"
namespace mm
{

    class ClipLas
    {
    public:
        ClipLas();
        ~ClipLas();

        /// @brief Set the path names of the shape file path;
        /// @param shpFilePath - the input file path of the input shape file;
        void setShpFilePath(std::string shpFilePath);

        void setLasFileDirName(std::string lasFileDirName);
        void setLasFileDirName(std::string lasFileDirName, std::string clippedFileDir);
        bool readShpFile();
        bool readPolygonsFromShpFile();
        bool getPositivePolygons();
        bool runClipping();

    private:
    public:
        bool isPtInPolygon(double x, double y);

        bool getAllLasFiles();

        bool getAllLasFiles(std::string dirName);

    private:
        /// @brief The folder/directory of the las files;
        std::string m_lasFileDirName;
        /// @brief The folder to place the clipped las files;
        std::string m_clippedLasFileDir;
        /// @brief The container for *las/*laz file names in the directory;
        std::vector<std::string> m_lasFiles;
        /// @brief The bounding box of the las files;
        std::vector<Polygon2> m_lasFilesBBX;

        /// @brief The File path of the shape file;
        std::string m_shpFilePath;

        /// @brief Polygons in the shape file;
        std::vector<ShpPolygon2> m_polygons;
        /// @brief In shape files, clock-wise is positive direction for polygons;
        std::vector<ShpPolygon2> m_positivePolygons;
        std::vector<ShpPolygon2> m_negativePolygons;

        /// @brief Boudning box of the current shape file;
        Box2 m_bbox;

        std::vector<std::string> m_allFiles;
    }; //!- class ClipLas

} // !- namespace mm;

#endif //!- MM_CLIP_LAS_H
