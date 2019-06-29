/*==================================================================================================

                                  Copyright (c) 2018 GeometryHub
                                 Unpublished - All rights reserved

====================================================================================================*/
// main.cpp : 定义控制台应用程序的入口点。
//
#include "AppKinect2.h"

int main(int argc, char *argv[])
{
    GPPInfo << "argc==" << argc << std::endl;
    GHB::AppKinect2 app;
    if (argc >= 7)
    {
        GPP::Vector3 bboxMin(atof(argv[1]), atof(argv[3]), atof(argv[5]));
        GPP::Vector3 bboxMax(atof(argv[2]), atof(argv[4]), atof(argv[6]));
        app.SetBBox(bboxMin, bboxMax);
        GPPInfo << " set bbox " << bboxMin[0] << " " << bboxMin[1] << " " << bboxMin[2] <<
            " " << bboxMax[0] << " " << bboxMax[1] << " " << bboxMax[2] << std::endl;
    }
    if (argc >= 8)
    {
        app.SetImageInterval(atoi(argv[7]));
    }
    if (argc == 9)
    {
        app.SetImageScale(atof(argv[8]));
    }
    app.Run();
    return 0;
}

