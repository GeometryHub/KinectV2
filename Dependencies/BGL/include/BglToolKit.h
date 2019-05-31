/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Vector3.h"

namespace GPP
{
    // Reset progress value before calling api
    extern BGL_EXPORT void ResetApiProgress(void);


    // Get progress value when running api
    // progress value range: [0, 1]
    extern BGL_EXPORT Real GetApiProgress(void);


    extern BGL_EXPORT void EnableApiProgress(void);


    extern BGL_EXPORT void DisableApiProgress(void);


    // count >= 0: 0 - it will auto-allocate thread according CPU processor count.
    extern BGL_EXPORT void SetThreadCount(Int count);


    extern BGL_EXPORT Int GetThreadCount();


    // scaleValue range: [0, 1.2]
    extern BGL_EXPORT Vector3 ColorCoding(Real scaleValue);


    extern BGL_EXPORT std::string GetTimeString(void);


    extern BGL_EXPORT void SleepThread(Real second);


    // cross platform rand number
    class BGL_EXPORT GppRand
    {
    public:
        GppRand();
        GppRand(int seed);
        ~GppRand();

        void SetSeed(int seed);
        int GetRand(void); // rand value >= 0

    private:
        int mState;
    };
    
}
