#include "clip_las.h"

#include "lasreader.hpp"
#include "laswriter.hpp"

#include <cstdio>
#include <dirent.h>
#include <filesystem>
#include <iostream>
#include <sys/stat.h>
#include <time.h>
namespace mm
{
    ClipLas::ClipLas()
    {
    }

    ClipLas::~ClipLas()
    {
    }

    void ClipLas::setShpFilePath(std::string shpFilePath)
    {
        this->m_shpFilePath = shpFilePath;
    }

    void ClipLas::setLasFileDirName(std::string lasFileDirName)
    {
        this->m_lasFileDirName = lasFileDirName;
    }

    void ClipLas::setLasFileDirName(std::string lasFileDirName, std::string clippedDir)
    {
        this->m_lasFileDirName = lasFileDirName;
        this->m_clippedLasFileDir = clippedDir;
    }

    bool ClipLas::isPtInPolygon(double x, double y)
    {
        SHPHandle shpHandle = SHPOpen(this->m_shpFilePath.c_str(), "rb");
        if (!shpHandle)
        {
            std::cout << "Failed to open shape file: " << this->m_shpFilePath << std::endl;
            return false;
        }

        int nShapeType, nEntities;
        double adfMinBound[4], adfMaxBound[4];
        SHPGetInfo(shpHandle, &nEntities, &nShapeType, adfMinBound, adfMaxBound);

        for (int i = 0; i < nEntities; ++i)
        {
            SHPObject *shape = SHPReadObject(shpHandle, i);
            double x = 0;
            double y = 0;
            for (int j = 0; j < shape->nParts; ++j)
            {
                Polygon2 currPolygon;
                // int idx = shape->;
            }
        }
        return true;
    }

    bool ClipLas::readPolygonsFromShpFile()
    {
        if (this->m_shpFilePath.empty())
        {
            std::cout << "No shape file is specified." << std::endl;
            return false;
        }

        SHPHandle shpHandle = SHPOpen(this->m_shpFilePath.c_str(), "rb");
        if (shpHandle == nullptr)
        {
            std::cout << "Error in open shape file" << std::endl;
            return false;
        }
        int nShpType, nEntities;
        double adfMinBound[4], adfMaxBound[4];
        SHPGetInfo(shpHandle, &nEntities, &nShpType, adfMinBound, adfMaxBound);

        for (int i = 0; i < nEntities; ++i)
        {
            SHPObject *shape = SHPReadObject(shpHandle, i);
            for (int j = 0; j < shape->nParts; ++j)
            {
                Polygon2 tempPolygon;
                mm::dvec2 vP;
            }
        }

        return true;
    }

    bool ClipLas::readShpFile()
    {
        SHPHandle hSHP = SHPOpen(this->m_shpFilePath.c_str(), "rb");
        if (hSHP == nullptr)
        {
            return false;
        }

        /*
        A pointer to an integer into which the number of entities/structures
        should be placed. May be NULL;
        */
        int pnEntites;

        /*
        A pointer to an integer into which the shapetype of this file should be placed.
        Shapefiles may contain eigher SHPT_POINT, SHPT_ARC, SHPT_POLYGON or
        SHPT_MULTIPOINT entities. This may be NULL;
        */
        int pnShapetype;

        /*
        The x, y, z and m minimum/maximum values will be placed into this four entry array.
        */
        double adfMinBound[4];
        double adfMaxBound[4];

        SHPGetInfo(hSHP, &pnEntites, &pnShapetype, adfMinBound, adfMaxBound);

        for (int i = 0; i < pnEntites; ++i)
        {
            SHPObject *psShape = SHPReadObject(hSHP, i);
            if (psShape == nullptr)
            {
                continue;
            }
            // Read only Polygons;
            if (psShape->nSHPType == 5)
            {
                ShpPolygon2 currPolygon;
                // if (psShape->nParts == 1) // Only contains no holes;
                if (psShape->nParts == 1)
                {
                    mm::dvec2 vertex;
                    for (int j = 0; j < psShape->nVertices; ++j)
                    {
                        vertex.x = psShape->padfX[j];
                        vertex.y = psShape->padfY[j];
                        currPolygon.push_back(vertex);
                    }
                    this->m_polygons.push_back(currPolygon);
                }
                // There are holes in the polygons;
                else if (psShape->nParts > 1)
                {
                    for (int j = 0; j < psShape->nParts; ++j)
                    {
                        mm::dvec2 vertex;
                        int index = psShape->panPartStart[j];
                        while (index < psShape->panPartStart[j + 1] &&
                               index < psShape->nVertices)
                        {
                            vertex.x = psShape->padfX[index];
                            vertex.y = psShape->padfY[index];
                            currPolygon.push_back(vertex);
                            index++;
                        }
                        this->m_polygons.push_back(currPolygon);
                        currPolygon.clear();
                    }
                }
            }
        }
        std::cout << "A total of: [ " << this->m_polygons.size() << " ] polygons were loaded." << std::endl;

        return true;
    }

    bool ClipLas::getPositivePolygons()
    {
        if (this->m_polygons.empty())
            return false;

        for (int i = 0; i < this->m_polygons.size(); ++i)
        {
            if (!this->m_polygons.at(i).is_clockwise())
            {
                this->m_positivePolygons.push_back(this->m_polygons.at(i));
            }
            else
            {
                this->m_negativePolygons.push_back(this->m_polygons.at(i));
            }
        }

        clock_t start = clock();
        for (int i = 0; i < this->m_positivePolygons.size(); ++i)
        {
            for (int j = 0; j < this->m_negativePolygons.size(); ++j)
            {
                if (this->m_negativePolygons.at(j).parsed)
                    continue;
                if (this->m_positivePolygons.at(i).contains(this->m_negativePolygons.at(j)))
                {
                    this->m_positivePolygons.at(i).indices.push_back(j);
                    this->m_negativePolygons.at(j).parsed = true;
                }
            }
        }
        clock_t total = clock() - start;
        std::cout << "There are: [ " << this->m_positivePolygons.size() << " ] positive polygons" << std::endl;
        std::cout << "There are: [ " << this->m_negativePolygons.size() << " ] negative polygons" << std::endl;
        return true;
    }

    bool ClipLas::getAllLasFiles()
    {
        if (this->m_lasFileDirName.empty())
            return false;

        // std::filesystem::path pathObj(this->m_lasFileDirName);

        for (const auto &entry : std::filesystem::directory_iterator(this->m_lasFileDirName))
        {
            std::filesystem::path pathObj(entry.path());
            std::string fileExt = pathObj.extension();
            if (fileExt != ".las" && fileExt != ".laz" && fileExt != ".LAS" && fileExt != ".LAZ")
            {
                continue;
            }
            this->m_lasFiles.push_back(entry.path());
        }
        if (this->m_lasFiles.empty())
        {
            return false;
        }

        if (!std::filesystem::is_directory(this->m_clippedLasFileDir.c_str()))
        {
            if (!std::filesystem::create_directory(this->m_clippedLasFileDir.c_str()))
            {
                std::cout << "Error in creating the designated output folder" << std::endl;
                return false;
            }
        }

        std::cout << "A total of: [ " << this->m_lasFiles.size() << " ] las/laz files were found under the given directory." << std::endl;
        return true;
    }

    bool ClipLas::getAllLasFiles(std::string pathName)
    {
        DIR *dir = nullptr;
        struct dirent *entry;

        dir = opendir(pathName.c_str());
        if (dir == nullptr)
        {
            closedir(dir);
            return false;
        }

        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_REG)
            {
                std::string fileName = entry->d_name;

                size_t pos = fileName.find_last_of(".");
                if (pos != std::string::npos)
                {
                    std::string extension = fileName.substr(pos + 1);
                    if (extension == "las" || extension == "LAS" ||
                        extension == "laz" || extension == "LAZ")
                    {
                        //
                        this->m_allFiles.push_back(entry->d_name);
                        //
                        this->m_lasFiles.push_back(entry->d_name);
                    }
                }
            }
        }
        closedir(dir);

        if (mkdir(this->m_clippedLasFileDir.c_str(), 0770) != 0)
        {
            std::cout << "Error in creating the designiated output folder" << std::endl;
            return false;
        }

        std::cout << "A total of: [ " << this->m_lasFiles.size() << " ] las/laz files were found under the given directory." << std::endl;
        return true;
    }

    bool ClipLas::runClipping()
    {
        if (this->m_lasFiles.empty())
            return false;
        if (this->m_polygons.empty())
            return false;
        if (this->m_positivePolygons.empty())
            return false;

        int fileCount = 1;
        for (auto &file : this->m_lasFiles)
        {
            // std::filesystem::path pathObj(file);
            // std::string fileName = pathObj.stem();
            // std::string fileOutputPath = this->m_clippedLasFileDir + fileName + ".laz";

            clock_t startTime = clock();

            std::string fileOutputPath = this->m_clippedLasFileDir + file;
            std::string fileName = this->m_lasFileDirName + "/" + file;
            LASreadOpener readOpener;
            readOpener.set_file_name(fileName.c_str(), true);
            std::cout << "output file directory: " << fileOutputPath << std::endl;
            std::cout << "---------------------------------------------------------------------------" << std::endl;
            std::cout << fileName << " is being checked. [ " << fileCount++ << "/" << m_lasFiles.size() << "]" << std::endl;

            LASreader *reader = readOpener.open();
            if (!reader || reader->npoints == 0)
            {
                reader->close();
                delete reader;
                reader = nullptr;
                std::cout << fileName << " can not be loaded. Thus, it will be skipped." << std::endl;
                continue;
            }

            std::cout << "There are: [ " << reader->header.number_of_point_records << " ] points." << std::endl;

            ShpPolygon2 bbxLasFile;
            dvec2 vertex;
            vertex.x = reader->header.min_x;
            vertex.y = reader->header.min_y;
            bbxLasFile.push_back(vertex);
            vertex.x = reader->header.min_x;
            vertex.y = reader->header.max_y;
            bbxLasFile.push_back(vertex);
            vertex.x = reader->header.max_x;
            vertex.y = reader->header.max_y;
            bbxLasFile.push_back(vertex);
            vertex.x = reader->header.max_x;
            vertex.y = reader->header.min_y;
            bbxLasFile.push_back(vertex);

            LASwriteOpener writeOpener;
            writeOpener.set_file_name(fileOutputPath.c_str());
            LASwriter *writer = writeOpener.open(&reader->header);
            int numPtInPolygon = 0;

            std::vector<int> idxPositivePlg;
            for (int i = 0; i < this->m_positivePolygons.size(); ++i)
            {
                if (this->m_positivePolygons.at(i).contains(bbxLasFile) &&
                    !bbxLasFile.intersects(this->m_positivePolygons.at(i)))
                {
                    bool IsIntersect = false;
                    for (int j = 0; j < this->m_positivePolygons.at(i).indices.size(); ++j)
                    {
                        int index = this->m_positivePolygons.at(i).indices.at(j);
                        if (this->m_negativePolygons.at(index).intersects(bbxLasFile))
                        {
                            IsIntersect = true;
                            break;
                        }
                    }
                    if (!IsIntersect)
                    {
                        idxPositivePlg.push_back(i);
                        // std::filesystem::copy_file(file, fileOutputPath);
                        // std::cout << file << " is fully contained in the loaded polygons." << std::endl;
                        while (reader->read_point())
                        {
                            LASpoint lasPt;
                            lasPt.init(&reader->header,
                                       reader->header.point_data_format,
                                       reader->header.point_data_record_length,
                                       nullptr);
                            lasPt = reader->point;
                            writer->write_point(&lasPt);
                            writer->update_inventory(&lasPt);
                            numPtInPolygon++;
                        }

                        break;
                    }
                    else
                    {
                        if (!this->m_positivePolygons.at(i).empty())
                        {
                            idxPositivePlg.push_back(i);
                        }
                    }
                }
                else if (this->m_positivePolygons.at(i).intersects(bbxLasFile) ||
                         bbxLasFile.intersects(this->m_positivePolygons.at(i)) ||
                         bbxLasFile.contains(this->m_positivePolygons.at(i)))
                {
                    if (!this->m_positivePolygons.at(i).empty())
                    {
                        idxPositivePlg.push_back(i);
                    }
                }
            }
            if (idxPositivePlg.empty())
            {
                // std::filesystem::remove(fileOutputPath.c_str());
                std::cout << fileName << " does not intersect with any polygons." << std::endl;
                std::remove(fileOutputPath.c_str());
                continue;
            }

            dvec2 pt;
            std::cout << "Start parsing each points for clipping." << std::endl;
            while (reader->read_point())
            {
                pt.x = reader->point.get_x();
                pt.y = reader->point.get_y();

                bool IsInPlg = false;
                bool IsInNegativePlg = false;

                for (int i = 0; i < idxPositivePlg.size(); ++i)
                {
                    int idPlg = idxPositivePlg.at(i);
                    if (!this->m_positivePolygons.at(idPlg).contains(pt))
                        continue;
                    IsInPlg = true;
                    IsInNegativePlg = false;
                    for (int k = 0; k < this->m_positivePolygons.at(idPlg).indices.size(); ++k)
                    {
                        int id = this->m_positivePolygons.at(idPlg).indices.at(k);
                        if (this->m_negativePolygons.at(id).contains(pt))
                        {
                            IsInNegativePlg = true;
                            break;
                        }
                    }
                }
                if (IsInPlg && !IsInNegativePlg)
                {
                    LASpoint lasPt;
                    lasPt.init(&reader->header,
                               reader->header.point_data_format,
                               reader->header.point_data_record_length,
                               nullptr);
                    lasPt = reader->point;
                    writer->write_point(&lasPt);
                    writer->update_inventory(&lasPt);
                    numPtInPolygon++;
                }
            }

            idxPositivePlg.clear();
            writer->update_header(&reader->header, true);
            writer->close();
            delete writer;
            writer = nullptr;

            std::cout << reader->header.number_of_point_records << " points were written out." << std::endl;

            reader->close();
            delete reader;
            reader = nullptr;

            if (numPtInPolygon == 0)
            {
                // std::filesystem::remove(fileOutputPath.c_str());
                std::remove(fileOutputPath.c_str());
            }
            std::cout << "[ " << file << " ] has been clipped." << std::endl;

            std::cout << "---------------------------------------------------------------------------" << std::endl;
        }
        return true;
    }

} // !-namespace mm
