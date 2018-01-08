/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include <fstream>
#include <iostream>

namespace GPP
{
    enum BGL_EXPORT LogLevel
    {
        LOGLEVEL_DEBUG = 0,
        LOGLEVEL_INFO,
        LOGLEVEL_ERROR,
        LOGLEVEL_OFF
    };

    extern BGL_EXPORT const LogLevel gSystemLogLevel;

#define GPPLog(level) \
    if (level < GPP::gSystemLogLevel) ;\
    else GPP::LogSystem::Get()->GetOFStream() 
#define GPPDebug GPPLog(GPP::LOGLEVEL_DEBUG)
#define GPPInfo GPPLog(GPP::LOGLEVEL_INFO)
#define GPPError GPPLog(GPP::LOGLEVEL_ERROR)

    class BGL_EXPORT LogSystem
    {
    private:
        static LogSystem* mpLogSystem;
        LogSystem(std::string logFileName);
    public:
        static void SetLogFileName(std::string logFileName);
        static LogSystem* Get(void);
        ~LogSystem(void);
        std::ofstream& GetOFStream();
    public:
        std::ofstream mOFStream;
    };
}
