/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"

namespace GPP
{
    extern BGL_EXPORT void DumpOnce(void);

    extern BGL_EXPORT bool GetDumpOnce(void);

    extern BGL_EXPORT void SetDumpOnce(bool dump);

    extern BGL_EXPORT void SetDumpDirectory(const std::string& dumpDir);

    extern BGL_EXPORT std::string GetDumpDirectory(void);
}
