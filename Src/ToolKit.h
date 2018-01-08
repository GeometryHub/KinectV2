/*==================================================================================================

                               Copyright (c) 2018 GeometryHub
                              Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include <string>
#include <vector>

namespace GHB
{
    class ToolKit
    {
    private:
        static ToolKit*  mpToolKit;
        ToolKit(void);
    public:
        static ToolKit* Get(void);
        static double GetTime(void);
        static std::string GetFilePath(std::string fileName);
#ifdef WIN32
        static bool FileOpenDlg(std::string& selectFileName, char* filterName);
        static bool MultiFileOpenDlg(std::vector<std::string>& selectFileNames, char* filterName);
        static bool FileSaveDlg(std::string& selectFileName, char* filterName);
#endif
        virtual ~ToolKit(void);
    };
}
