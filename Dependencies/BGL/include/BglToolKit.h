/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Vector3.h"

namespace GPP
{
    // count >= 0: 0 - it will auto-allocate thread according CPU processor count.
    extern BGL_EXPORT void SetThreadCount(Int count);


    extern BGL_EXPORT Vector3 ColorCoding(Real scaleValue);


    extern BGL_EXPORT std::string GetTimeString(void);


    extern BGL_EXPORT void SleepThread(Real second);


/////////////////////////////////////// Internal Use ///////////////////////////////////////
    
    class ThreadManager
    {
        static ThreadManager* mpThreadManager;
        explicit ThreadManager();
    public:
        static ThreadManager* Get();
        void SetThreadCount(Int count);
        Int GetThreadCount(void) const;
        ~ThreadManager();

    private:
        Int mThreadCount;
    };
}
