
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

int main(int argc, char **argv)
{

    std::string shpFilePath = "/directory/to/shapefile.shp";
    std::string lasDir = "/directory/to/las/files/";

    mm::ClipLas *clip = new mm::ClipLas;

    clip->getAllLasFiles(lasDir);

    clip->setShpFilePath(shpFilePath);
    clip->readShpFile2();
    clip->getPositivePolygons();
    clip->setLasFileDirName(lasDir);
    clip->runClipping();

    if (clip)
    {
        delete clip;
        clip = nullptr;
    }

    return 0;
}

void usage(char *argv)
{
    std::cout << "Usage:" << std::endl;
    std::cout << argv << " [Path of *.shp file] [Directory of LAS/LAZ files]" << std::endl;
}
