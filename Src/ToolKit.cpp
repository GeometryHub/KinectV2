/*==================================================================================================

                                   Copyright (c) 2018 GeometryHub
                                  Unpublished - All rights reserved

====================================================================================================*/
#include "ToolKit.h"
#include <windows.h>

namespace GHB
{
    ToolKit* ToolKit::mpToolKit = NULL;

    ToolKit::ToolKit(void)
    {
    }

    ToolKit* ToolKit::Get()
    {
        if (mpToolKit == NULL)
        {
            mpToolKit = new ToolKit;
        }
        return mpToolKit;
    }

    double ToolKit::GetTime()
    {
        double curTime = 0;
#ifdef WIN32
        static __int64 start = 0;
        static __int64 frequency = 0;

        if (start == 0)
        {
            QueryPerformanceCounter((LARGE_INTEGER*)&start);
            QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
            return 0.0f;
        }

        __int64 counter = 0;
        QueryPerformanceCounter((LARGE_INTEGER*)&counter);
        curTime = (counter - start) / double(frequency);
#else
        curTime = static_cast<double>(time(NULL));
#endif
        return curTime;
    }
    
    ToolKit::~ToolKit(void)
    {
    }
}
