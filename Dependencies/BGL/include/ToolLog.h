/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "BglDefines.h"
#include <fstream>
#include <iostream>
#ifdef LOG_STRING
#include <sstream>
#include <vector>
#include <string>
#endif 

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
    else GPP::LogSystem::Get()->GetLogStream() 
#define GPPDebug GPPLog(GPP::LOGLEVEL_DEBUG)
#define GPPInfo GPPLog(GPP::LOGLEVEL_INFO)
#define GPPError GPPLog(GPP::LOGLEVEL_ERROR)

    extern BGL_EXPORT const bool gLogStat;

#define GPPSTAT \
    if (!gLogStat) ;\
    else GPP::LogSystem::Get()->GetLogStream() 

#ifdef LOG_STRING
    class BGL_EXPORT LogSystem
    {
    private:
        static LogSystem* mpLogSystem;
        LogSystem();
    public:
        static LogSystem* Get(void);
        ~LogSystem(void);
        std::stringstream& GetLogStream();
        std::vector<std::string> GetLogString();
        void AddLogCount();
        void Clear(void);
    public:
        std::stringstream mSStream;
        std::vector<std::string> mStringList;
        Int mCurLogCount;
        static Int mMaxLogCount;
    };
#else
    class BGL_EXPORT LogSystem
    {
    private:
        static LogSystem* mpLogSystem;
        LogSystem(std::string logFileName);
    public:
        static void SetLogFileName(std::string logFileName);
        static LogSystem* Get(void);
        ~LogSystem(void);
        std::ofstream& GetLogStream();
    public:
        std::ofstream mOFStream;
    };
#endif
}
