
#ifndef MM_RETILE_LAS_H
#define MM_RETILE_LAS_H

#include <string>
#include <vector>

namespace mm
{

    class Retile
    {
    public:
        Retile();
        ~Retile();

        void setInputDir(std::string inDirName);

        void setRetileSize(double sizeX, double sizeY);
        void setRetileCount(int numX, int numY);

        bool retileBySize();
        bool retileByCount();
        bool getFilesInFolder();

    private:
        bool retileOneFileBySize(std::string &fileName);
        bool retileOneFileByCount(std::string &fileName);

    private:
        ///
        double m_sizeX;
        double m_sizeY;

        /// @brief Number of columns and rows;
        int m_numCols;
        int m_numRows;

        /// @brief Input directory ;
        std::string m_inFolder;

        /// @brief Input file name;
        std::vector<std::string> m_inFileNames;
        std::vector<std::string> m_outFolders;

    }; // !- class Retile;

} // !- namespace mm

#endif // !- MM_RETILE_LAS_H