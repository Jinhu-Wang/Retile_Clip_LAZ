#include "retile_las.h"

#include "lasreader.hpp"
#include "laswriter.hpp"

#include <chrono>
#include <dirent.h>
#include <filesystem>
#include <iostream>
#include <math.h>
#include <time.h>
namespace mm
{

    Retile::Retile()
        : m_sizeX(0), m_sizeY(0)
    {
    }

    Retile::~Retile()
    {
    }

    void Retile::setInputDir(std::string inDirName)
    {
        this->m_inFolder = inDirName;
    }

    void Retile::setRetileSize(double sizeX, double sizeY)
    {
        this->m_sizeX = sizeX;
        this->m_sizeY = sizeY;
    }

    void Retile::setRetileCount(int numX, int numY)
    {
        this->m_numCols = numX;
        this->m_numRows = numY;
    }

    bool Retile::retileOneFileBySize(std::string &fileName)
    {
        LASreadOpener readOpener;
        readOpener.set_file_name(fileName.c_str(), true);

        LASreader *reader = readOpener.open();
        if (!reader || reader->npoints == 0)
        {
            reader->close();
            delete reader;
            reader = nullptr;
            return false;
        }

        double deltaX = reader->header.max_x - reader->header.min_x;
        double deltaY = reader->header.max_y - reader->header.min_y;

        this->m_numCols = std::ceil(deltaX / this->m_sizeX);
        this->m_numRows = std::ceil(deltaY / this->m_sizeY);

        std::filesystem::path pathObj(fileName);
        std::string fileExt = pathObj.extension();
        std::string fileDir = pathObj.parent_path();
        std::string retileFolder = fileDir + "/" + pathObj.stem().string() + "_Retiled/";
        std::filesystem::create_directory(retileFolder.c_str());

        this->m_outFolders.push_back(retileFolder);

        std::cout << "There will be [ " << this->m_numCols * this->m_numRows << " ] number of new tiles generated." << std::endl;
        pathObj.replace_extension("");
        std::vector<std::string> outFileNames(this->m_numCols * this->m_numRows);
        std::vector<int> numPts(this->m_numCols * this->m_numRows);
        std::vector<LASwriter *> writers(this->m_numCols * this->m_numRows);
        for (int i = 0; i < this->m_numRows; ++i)
        {
            for (int j = 0; j < this->m_numCols; ++j)
            {
                int index = i * this->m_numCols + j;
                std::string outPath = retileFolder + pathObj.stem().c_str();
                outPath += "_" + std::to_string(i) + "_" + std::to_string(j) + fileExt;
                outFileNames[index] = outPath;

                LASwriteOpener writeOpener;
                writeOpener.set_file_name(outFileNames[index].c_str());
                writers[index] = writeOpener.open(&reader->header);
            }
        }
        // int total = reader->npoints;
        // int currId = 0.0;
        // clock_t startTime = clock();
        auto start = std::chrono::steady_clock::now();
        while (reader->read_point())
        {
            const int col = std::floor((reader->point.get_x() - reader->header.min_x) / this->m_sizeX);
            const int row = std::floor((reader->point.get_y() - reader->header.min_y) / this->m_sizeY);
            const int index = row * this->m_numCols + col;

            numPts[index]++;

            LASpoint pt;
            pt.init(&reader->header,
                    reader->header.point_data_format,
                    reader->header.point_data_record_length,
                    nullptr);
            pt = reader->point;
            writers[index]->write_point(&pt);
            writers[index]->update_inventory(&pt);
            // currId++;
            // double percentage = currId * 1.0 / total;
            // std::cout << "The progress of this file: " << (int)(percentage * 100) << " %\r";
            // std::cout.flush();
        }

        for (int i = 0; i < this->m_numCols * this->m_numRows; ++i)
        {
            if (numPts[i] == 0)
            {
                std::filesystem::remove(outFileNames[i].c_str());
                continue;
            }
            writers[i]->update_header(&reader->header, true);
            writers[i]->close();
            delete writers[i];
            writers[i] = nullptr;
        }

        reader->close();
        delete reader;
        reader = nullptr;
        auto end = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        // auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        // auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "File " << fileName << " is retiled in [ " << elapsed.count() / 1000 << " ] seconds." << std::endl;
        std::cout << "The tiles were output to folder [ " << retileFolder << " ]." << std::endl;
        return true;
    }

    bool Retile::retileOneFileByCount(std::string &fileName)
    {
        LASreadOpener readOpener;
        readOpener.set_file_name(fileName.c_str(), true);

        LASreader *reader = readOpener.open();
        if (!reader || reader->npoints == 0)
        {
            reader->close();
            delete reader;
            reader = nullptr;
            return false;
        }

        double deltaX = (reader->header.max_x - reader->header.min_x) / this->m_numCols;
        double deltaY = (reader->header.max_y - reader->header.min_y) / this->m_numRows;

        std::filesystem::path pathObj(fileName);
        std::string fileExt = pathObj.extension();
        std::string fileDir = pathObj.parent_path();
        std::string retileFolder = fileDir + "/" + pathObj.stem().string() + "_Retiled/";
        std::filesystem::create_directory(retileFolder.c_str());

        pathObj.replace_extension("");
        std::vector<std::string> outFileName(this->m_numCols * this->m_numRows);
        std::vector<int> numPts(this->m_numCols * this->m_numRows);
        std::vector<LASwriter *> writers(this->m_numCols * this->m_numRows);
        for (int i = 0; i < this->m_numRows; ++i)
        {
            for (int j = 0; j < this->m_numCols; ++j)
            {
                int index = i * this->m_numCols + j;
                std::string outPath = retileFolder + pathObj.stem().c_str();
                outPath += std::to_string(i) + "_" + std::to_string(j) + fileExt;
                outFileName[index] = outPath;

                LASwriteOpener writeOpener;
                writeOpener.set_file_name(outFileName[index].c_str());
                writers[index] = writeOpener.open(&reader->header);
            }
        }

        while (reader->read_point())
        {
            const int col = std::floor((reader->point.get_x() - reader->header.min_x) / deltaX);
            const int row = std::floor((reader->point.get_y() - reader->header.min_y) / deltaY);
            const int index = row * this->m_numCols + col;

            numPts[index]++;

            LASpoint pt;
            pt.init(&reader->header,
                    reader->header.point_data_format,
                    reader->header.point_data_record_length,
                    nullptr);
            pt = reader->point;
            writers[index]->write_point(&pt);
            writers[index]->update_inventory(&pt);
        }

        for (int i = 0; i < this->m_numCols * this->m_numRows; ++i)
        {
            if (numPts[i] == 0)
            {
                std::filesystem::remove(outFileName[i].c_str());
                continue;
            }
            writers[i]->update_header(&reader->header, true);
            writers[i]->close();
            delete writers[i];
            writers[i] = nullptr;
        }

        reader->close();
        delete reader;
        reader = nullptr;

        return true;
    }

    /// @note this verison does not compile on spider server.
    /*
        bool Retile::getFilesInFolder()
        {
            for (const auto &entry : std::filesystem::directory_iterator(this->m_inFolder))
            {
                std::filesystem::path pathObj(entry.path());
                std::string fileExt = pathObj.extension();
                if (fileExt != ".las" && fileExt != ".LAS" && fileExt != ".laz" && fileExt != ".LAZ")
                {
                    continue;
                }
                this->m_inFileNames.push_back(entry.path());
            }
            if (this->m_inFileNames.empty())
            {
                return false;
            }
            return true;
        }
    */

    /// @brief This version compiles on spider server.
    /// @return
    bool Retile::getFilesInFolder()
    {
        DIR *dir = nullptr;
        struct dirent *entry;

        dir = opendir(this->m_inFolder.c_str());
        if (dir == nullptr)
        {
            closedir(dir);
            std::cout << "directory: " << this->m_inFolder.c_str() << std::endl;
            std::cout << "The provided [las/laz] directory can not be accessed." << std::endl;
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
                        std::string fullFileName = this->m_inFolder + "/" + entry->d_name;
                        // this->m_inFileNames.push_back(entry->d_name);
                        this->m_inFileNames.push_back(fullFileName);
                    }
                }
            }
        }
        closedir(dir);
        return true;
    }

    bool Retile::retileByCount()
    {
        if (this->m_inFileNames.empty())
        {
            this->getFilesInFolder();
        }
        if (this->m_inFileNames.empty())
        {
            return false;
        }

        for (int i = 0; i < this->m_inFileNames.size(); ++i)
        {
            this->retileOneFileByCount(this->m_inFileNames.at(i));
        }

        return true;
    }

    bool Retile::retileBySize()
    {
        if (this->m_inFileNames.empty())
        {
            this->getFilesInFolder();
            std::cout << "There are " << this->m_inFileNames.size() << " [las/laz] files found for re-tiling." << std::endl;
        }
        if (this->m_inFileNames.empty())
        {
            std::cout << "There are no [las/las] files found under the given directory" << std::endl;
            return false;
        }

        std::cout << "Start re-tiling by the specified size..." << std::endl;
        for (int i = 0; i < this->m_inFileNames.size(); ++i)
        {
            std::cout << this->m_inFileNames[i] << " is being re-tiled..." << std::endl;
            this->retileOneFileBySize(this->m_inFileNames.at(i));
        }
        return true;
    }

} // !- namespace