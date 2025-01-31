
#include "clip_las.h"

using namespace mm;

#include "shapefil.h"
#include <iostream>
#include <vector>

class ShapefileReader
{
public:
    ShapefileReader(const std::string &path)
    {
        hSHP = SHPOpen(path.c_str(), "rb");
        hDBF = DBFOpen(path.c_str(), "rb");
        if (hSHP == nullptr || hDBF == nullptr)
        {
            std::cerr << "Failed to open shapefile or associated DBF!" << std::endl;
            exit(1);
        }
    }

    ~ShapefileReader()
    {
        SHPClose(hSHP);
        DBFClose(hDBF);
    }

    void ReadPolygons()
    {
        int nEntities, nShapeType;
        SHPGetInfo(hSHP, &nEntities, &nShapeType, nullptr, nullptr);

        for (int i = 0; i < nEntities; i++)
        {
            SHPObject *psShape = SHPReadObject(hSHP, i);

            if (psShape->nSHPType == SHPT_POLYGON)
            {
                std::cout << "Polygon " << i << " has " << psShape->nVertices << " vertices." << std::endl;

                // Example: Read an attribute named "Name" for each polygon.
                // Adjust based on your actual attribute column names and types.
                int fieldIndex = DBFGetFieldIndex(hDBF, "Name");
                const char *name = DBFReadStringAttribute(hDBF, i, fieldIndex);
                std::cout << "Associated Name: " << name << std::endl;
            }

            SHPDestroyObject(psShape);
        }
    }

    void ReadPolygonProperty()
    {
        int numRecords = DBFGetRecordCount(hDBF);
        int numFields = DBFGetFieldCount(hDBF);

        for (int i = 0; i < numRecords; i++)
        {
            std::cout << "Record " << i + 1 << ":" << std::endl;
            for (int j = 0; j < numFields; j++)
            {
                char fieldName[12];
                DBFFieldType eType = DBFGetFieldInfo(hDBF, j, fieldName, nullptr, nullptr);

                switch (eType)
                {
                case FTString:
                    std::cout << fieldName << ": " << DBFReadStringAttribute(hDBF, i, j) << std::endl;
                    break;
                case FTInteger:
                    std::cout << fieldName << ": " << DBFReadIntegerAttribute(hDBF, i, j) << std::endl;
                    break;
                case FTDouble:
                    std::cout << fieldName << ": " << DBFReadDoubleAttribute(hDBF, i, j) << std::endl;
                    break;
                default:
                    std::cout << fieldName << ": (unsupported type)" << std::endl;
                    break;
                }
            }
            std::cout << std::endl;
        }
    }

private:
    SHPHandle hSHP;
    DBFHandle hDBF;
};

int main12121()
{
    std::string shpFilePath = "/home/jinhu/Work/Data/2023.10.25-AHN4/Kaartbladen_AHN4_poly/Kaartbladen_AHN4_poly.shp";
    ShapefileReader reader(shpFilePath.c_str());
    // reader.ReadPolygons();
    reader.ReadPolygonProperty();
    return 0;
}

int main(int argc, char **argv)
{
    // French Data;
    // std::string lasDir = "/home/jinhu/Work/Data/2023.04.19-MAMBO/FR/LIDARHD_1-0_LAZ_MQ-0740_6249-2021/";
    // std::string shpFilePath = "/home/jinhu/Work/Data/2023.04.19-MAMBO/FR/FR_Bagnas_fusionne/perimetre_bagnas_fusionne_20221123.shp";

    // std::string shpFilePath = "/home/jinhu/Work/Data/2023.04.19-MAMBO/FR/FR_Bagnas_fusionne/perimetre_bagnas_fusionne_20221123.shp";
    // std::string lasDir = "/home/jinhu/Work/Data/2023.04.19-MAMBO/NL/LiDAR/";
    // std::string shpFilePath = "/home/jinhu/Work/Data/2023.04.28-Oost_8cm_images/polygons/marsh_excluding_water_infrastruct_pioneer_final/marsh_excluding_water_infrastruct_pioneer_final.shp";
    // std::string shpFilePath = "/home/jinhu/Work/Data/2023.04.30-MAMBO/DK/MolsBjerge/Polygons/molslab/molslab.shp";
    // std::string shpFilePath = "/home/jinhu/Work/Data/2023.04.30-MAMBO/FR/FR_Bagnas_fusionne/perimetre_bagnas_fusionne_20221123.shp";
    // std::string shpFilePath = "/home/jinhu/Work/Data/2023.04.19-MAMBO/FR/FR_Bagnas_fusionne/perimetre_bagnas_fusionne_20221123.shp";
    // std::string shpFilePath = "/home/jinhu/shapefile_RDnew/translated/RDNew.shp";
    // std::string shpFilePath = "/home/jinhu/Work/Data/2023.10.25-AHN4/Kaartbladen_AHN4_poly/Kaartbladen_AHN4_poly.shp";
    std::string shpFilePath = "/home/jinhu/Downloads/fenced_bound_proj_AHN4/fenced_bound_proj_AHN4.shp";
    //  std::string shpFilePath = "/home/jinhu/Work/Data/2023.04.28-Oost_8cm_images/polygons/debug/Oost2.shp";
    //  std::string lasDir = "/home/jinhu/Work/Data/2023.04.19-MAMBO/NL/LiDAR/retiled/C_26AN2_Retiled";
    //  std::string lasDir = "/home/jinhu/Work/Data/2023.04.30-MAMBO/DK/MolsBjerge/PointClouds";
    // std::string lasDir = "/home/jinhu/Work/Data/2023.04.30-MAMBO/FR/PointClouds";
    std::string lasDir = "/home/jinhu/Downloads/26BZ1";
    // std::string lasDir = "/home/jinhu/Work/Data/2023.04.30-MAMBO/FR/PointClouds/LIDARHD_1-0_LAZ_MQ-0742_6247-2021/LIDARHD_1-0_LAZ_MQ-0742_6247-2021";
    // std::string lasDir = "/home/jinhu/Work/Data/2023.04.19-MAMBO/FR/PointClouds";

    mm::ClipLas *clip = new mm::ClipLas;

    clip->getAllLasFiles(lasDir);

    clip->setShpFilePath(shpFilePath);
    clip->readShpFile2();
    clip->getPositivePolygons();
    clip->setLasFileDirName(lasDir);
    // clip->getAllLasFiles();
    clip->runClipping3();

    return 0;
}

void usage(char *argv)
{
    std::cout << "Usage:" << std::endl;
    std::cout << argv << " [Path of *.shp file] [Directory of LAS/LAZ files]" << std::endl;
}

int main_works(int argc, char **argv)
{
    if (argc <= 1 || argc != 4)
    {
        usage(argv[0]);
        return 1;
    }

    std::string shpFilePath = argv[1];
    std::string lasDir = argv[2];
    std::string outDir = argv[3];

    if (shpFilePath.empty())
    {
        std::cout << "Shape file is not found." << std::endl;
        return 1;
    }
    if (lasDir.empty())
    {
        std::cout << "LAS/LAZ file direcotry is not found." << std::endl;
        return 1;
    }
    std::cout << "===========================================================================" << std::endl;
    std::cout << "The input shapefile is: [ " << shpFilePath << " ]" << std::endl;
    std::cout << "The input directory of [las/laz] file is: [ " << lasDir << " ]" << std::endl;
    std::cout << "---------------------------------------------------------------------------" << std::endl;
    mm::ClipLas *clip = new mm::ClipLas;
    clip->setShpFilePath(shpFilePath);
    clip->readShpFile2();
    clip->getPositivePolygons();
    clip->setLasFileDirName(lasDir, outDir);
    clip->getAllLasFiles();
    std::cout << "The clipping is in progress...." << std::endl;

    clip->runClipping2();
    std::cout << "The clipping is done." << std::endl;
    std::cout << "===========================================================================" << std::endl;

    return 0;
}