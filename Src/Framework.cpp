/*==================================================================================================

                                    Copyright (c) 2018 VisionPark
                                  Unpublished - All rights reserved

====================================================================================================*/
#include "Framework.h"
#include <iostream>
#include "AppFuseDepthVideo.h"
#include "AppTest.h"
#include "AppKinect2.h"


namespace VPK
{
    Framework::Framework()
    {
    }

    Framework::~Framework()
    {
    }

    void Framework::Init()
    {
        RegisterTestClass("kinect2", CreateAppKinect2);
        std::cout << "Framework::Init...Success" << std::endl;
    }

    void Framework::Run(int argc, char *argv[])
    {
        std::cout << "Framework::Run..." << std::endl;
        if (argc < 1)
        {
            std::cout << "Framework::Run: invalid argc < 1: " << argc << std::endl;
            return;
        }
        std::string appName = std::string(argv[0]);
        std::map<std::string, CreateApp>::iterator findItem = mAppMap.find(appName);
        if (findItem == mAppMap.end())
        {
            std::cout << "Framework::Run: could not find app: " << appName.c_str() << std::endl;
            return;
        }
        std::cout << appName.c_str() << std::endl;
        argc--;
        argv++;
        std::vector<std::string> parms;
        for (int pid = 0; pid < argc; pid++)
        {
            parms.push_back(std::string(argv[pid]));
        }
        AppBase* app = findItem->second();
        if (app == NULL)
        {
            std::cout << "Framework::Run: create app failed" << std::endl;
            return;
        }
        if (!app->Run(parms))
        {
            std::cout << "Framework::Run: " << appName.c_str() << " failed" << std::endl;
        }
        else
        {
            std::cout << "Framework::Run...Success" << std::endl;
        }
        delete app;
    }

    bool Framework::RegisterTestClass(std::string appName, CreateApp method)
    {
        std::map<std::string, CreateApp>::iterator findItem = mAppMap.find(appName);
        if (findItem != mAppMap.end())
        {
            return false;
        }
        mAppMap[appName] = method;
        return true;
    }
}
