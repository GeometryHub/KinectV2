/*==================================================================================================

                                  Copyright (c) 2018 GeometryHub
                                 Unpublished - All rights reserved

====================================================================================================*/
#include "AppKinect2.h"
#include <process.h>
#include "ToolKit.h"
#include <iostream>

namespace GHB
{
    static unsigned __stdcall RunThread(void *arg)
    {
        AppKinect2* app = (AppKinect2*)arg;
        if (app == NULL)
        {
            return 0;
        }
        app->ExportDepthData();
        return 1;
    }

    AppKinect2::AppKinect2() :
        mAppRunning(true),
        mpRoot(NULL),
        mpMainCamera(NULL),
        mpRenderWindow(NULL),
        mpSceneManager(NULL),
        mpViewport(NULL),
        mpLight(NULL),
        mpInputManager(NULL),
        mpKeyboard(NULL),
        mTimeAccumulate(0),
        mRenderDeltaTime(0.05),
        mpKinectSensor(NULL),
        mpMultiSourceFrameReader(NULL),
        mpCoordinateMapper(NULL),
        mMSEvent(NULL),
        mFrameWidth(-1),
        mFrameHeight(-1),
        mExportCountAcc(0),
        mDepthCountAcc(0),
        mDepthTimeAcc(0),
        mIsScanDepth(false),
        mScanedDepthList(),
        mMutex()
    {
    }

    void AppKinect2::Run()
    {
        Init();
        double timeLastFrame = GetTime();
        while (mAppRunning)
        {
            double timeCurrentFrame = GetTime();
            double timeSinceLastFrame = timeCurrentFrame - timeLastFrame;
            timeLastFrame = timeCurrentFrame;
            Update(timeSinceLastFrame);
        }
    }

    bool AppKinect2::keyPressed(const OIS::KeyEvent &arg)
    {
        if (arg.key == OIS::KC_S)
        {
            mIsScanDepth = true;
            if (mpLight)
            {
                mpLight->setDiffuseColour(0.753, 0.5, 0.5);
            }
        }
        else if (arg.key == OIS::KC_V)
        {
            mIsScanDepth = false;
            if (mpLight)
            {
                mpLight->setDiffuseColour(0.09, 0.48627, 0.69);
            }
        }
        return true;
    }

    bool AppKinect2::keyReleased(const OIS::KeyEvent &arg)
    {
        return true;
    }

    void AppKinect2::windowResized(Ogre::RenderWindow* rw)
    {
        mpMainCamera->setAspectRatio((Ogre::Real)rw->getWidth() / (Ogre::Real)rw->getHeight());
    }

    bool AppKinect2::windowClosing(Ogre::RenderWindow* rw)
    {
        mAppRunning = false;
        return true;
    }

    void AppKinect2::ExportDepthData()
    {
        while (mAppRunning)
        {
            if (mScanedDepthList.empty())
            {
                continue;
            }
            std::string fileName;
            GPP::PointCloud* curDepth = NULL;
            {
                GPP::ScopedLock lock(mMutex);
                if (mScanedDepthList.empty())
                {
                    continue;
                }
                fileName = mScanedDepthList.at(0).first;
                curDepth = mScanedDepthList.at(0).second;
                mScanedDepthList.pop_front();
                mExportCountAcc++;
            }
            GPP::Parser::ExportGridPointCloud(fileName, curDepth);
            GPPFREEPOINTER(curDepth);
        }
    }

    void AppKinect2::Init()
    {
        // Init Render
        mpRoot = new Ogre::Root();
        bool hasConfig = false;
        if (mpRoot->restoreConfig())
        {
            hasConfig = true;
        }
        else if (mpRoot->showConfigDialog())
        {
            hasConfig = true;
        }

        if (hasConfig)
        {
            mpRenderWindow = mpRoot->initialise(true, "VisionPark");
            mpSceneManager = mpRoot->createSceneManager(Ogre::ST_GENERIC, "MainSceneManager");
            mpMainCamera = mpSceneManager->createCamera("MainCamera");
            mpMainCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
            mpMainCamera->setPosition(0, 0, -2);
            mpMainCamera->lookAt(0, 0, 0);
            mpMainCamera->setNearClipDistance(0.05);
            mpMainCamera->setFarClipDistance(0);
            mpMainCamera->setAspectRatio((Ogre::Real)mpRenderWindow->getWidth() / (Ogre::Real)mpRenderWindow->getHeight());
            mpViewport = mpRenderWindow->addViewport(mpMainCamera);
            mpViewport->setBackgroundColour(Ogre::ColourValue(0.86, 0.86, 0.86));
            mpMainCamera->setAspectRatio(Ogre::Real(mpViewport->getActualWidth()) / Ogre::Real(mpViewport->getActualHeight()));
            Ogre::WindowEventUtilities::addWindowEventListener(mpRenderWindow, this);
        }

        // Init Resource
        LoadResource("./media", "FileSystem", "CommonResource", false);

        // Init Input
        OIS::ParamList paramList;
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        mpRenderWindow->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << (unsigned int)windowHnd;
        paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
        paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
        paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
        paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
        mpInputManager = OIS::InputManager::createInputSystem(paramList);
        mpKeyboard = static_cast<OIS::Keyboard*>(mpInputManager->createInputObject(OIS::OISKeyboard, true));
        mpKeyboard->setEventCallback(this);
        mpKeyboard->setBuffered(true);
        unsigned int width, height, depth;
        int left, top;
        mpRenderWindow->getMetrics(width, height, depth, left, top);

        // Set up scene
        mpSceneManager->setAmbientLight(Ogre::ColourValue(0.15, 0.15, 0.15));
        mpLight = mpSceneManager->createLight("SimpleLight");
        mpLight->setPosition(0, 0, -20);
        mpLight->setDiffuseColour(0.09, 0.48627, 0.69);
        mpLight->setSpecularColour(0.3, 0.3, 0.3);

        // Init Kinect2
        if (!SUCCEEDED(GetDefaultKinectSensor(&mpKinectSensor)))
        {
            GPPInfo << "Kinect: Cannot get kinnect sensor." << std::endl;
            return;
        }
        if (!SUCCEEDED(mpKinectSensor->get_CoordinateMapper(&mpCoordinateMapper)))
        {
            GPPInfo << "Kinect: get_CoordinateMapper failed" << std::endl;
            return;
        }
        if (!SUCCEEDED(mpKinectSensor->Open()))
        {
            GPPInfo << "Kinect: kinect open failed" << std::endl;
            return;
        }
        if (!SUCCEEDED(mpKinectSensor->OpenMultiSourceFrameReader(
            FrameSourceTypes::FrameSourceTypes_Depth,
            &mpMultiSourceFrameReader)))
        {
            GPPInfo << "Kinect: kinect OpenMultiSourceFrameReader failed" << std::endl;
            return;
        }
        if (!SUCCEEDED(mpMultiSourceFrameReader->SubscribeMultiSourceFrameArrived(&mMSEvent)))
        {
            GPPInfo << "Kinect: kinect SubscribeMultiSourceFrameArrived failed" << std::endl;
            return;
        }
        _beginthreadex(NULL, 0, RunThread, (void *)this, 0, NULL);
        _beginthreadex(NULL, 0, RunThread, (void *)this, 0, NULL);
    }

    void AppKinect2::Update(double timeElapsed)
    {
        UpdateInputSystem();
        UpdateKinectData();
        mTimeAccumulate += timeElapsed;
        mDepthTimeAcc += timeElapsed;
        if (mTimeAccumulate > mRenderDeltaTime)
        {
            mTimeAccumulate = mTimeAccumulate - mRenderDeltaTime;
            mpRoot->renderOneFrame();
        }
        if (mDepthTimeAcc > 1.0)
        {
            std::cout << "\r capture fps=" << int(double(mDepthCountAcc) / mDepthTimeAcc) << 
                " , export fps=" << int(double(mExportCountAcc) / mDepthTimeAcc);
            mDepthTimeAcc = 0;
            mDepthCountAcc = 0;
            mExportCountAcc = 0;
        }
    }

    void AppKinect2::UpdateInputSystem()
    {
        mpKeyboard->capture();
        Ogre::WindowEventUtilities::messagePump();
    }

    void AppKinect2::UpdateKinectData()
    {
        GPP::PointCloud* pointCloud = CaptureOneDepth();
        if (pointCloud == NULL)
        {
            return;
        }
        if (pointCloud->GetValidGridCount() < 10)
        {
            GPPFREEPOINTER(pointCloud);
            return;
        }
        if (mDepthCountAcc % 6 == 0)
        {
            //GPP::ConsolidateGrid::CalculateGridNormal(pointCloud);
            RenderPointCloud(pointCloud);
        }
        mDepthCountAcc++;
        if (mIsScanDepth)
        {
            __int64 counter = 0;
            QueryPerformanceCounter((LARGE_INTEGER*)&counter);
            std::stringstream ss;
            ss << "./scan/depth_" << counter << ".gbg";
            std::string fileName;
            ss >> fileName;
            GPP::ScopedLock lock(mMutex);
            mScanedDepthList.push_back(std::pair<std::string, GPP::PointCloud*>(fileName, pointCloud));
        }
        else
        {
            GPPFREEPOINTER(pointCloud);
        }
    }

    void AppKinect2::LoadResource(std::string path, std::string type, std::string groupName, bool recursive)
    {
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path, type, groupName, recursive);
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(groupName);
    }

    void AppKinect2::RenderPointCloud(const GPP::PointCloud* pointCloud)
    {
        if (mpSceneManager == NULL || pointCloud == NULL)
        {
            return;
        }
        std::string pointCloudName = "ScenePointCloud";
        Ogre::ManualObject* manualObj = NULL;
        if (mpSceneManager->hasManualObject(pointCloudName))
        {
            manualObj = mpSceneManager->getManualObject(pointCloudName);
            manualObj->clear();
        }
        else
        {
            manualObj = mpSceneManager->createManualObject(pointCloudName);
            if (mpSceneManager->hasSceneNode("ModelNode"))
            {
                mpSceneManager->getSceneNode("ModelNode")->attachObject(manualObj);
            }
            else
            {
                mpSceneManager->getRootSceneNode()->createChildSceneNode("ModelNode")->attachObject(manualObj);
            }
        }
        int pointCount = pointCloud->GetPointCount();
        manualObj->begin("SimplePoint", Ogre::RenderOperation::OT_POINT_LIST);
        for (int pid = 0; pid < pointCount; pid += 3)
        {
            GPP::Vector3 coord = pointCloud->GetPointCoord(pid);
            GPP::Vector3 normal = pointCloud->GetPointNormal(pid);
            manualObj->position(coord[0], coord[1], coord[2]);
            manualObj->normal(normal[0], normal[1], normal[2]);
        }
        manualObj->end();
    }

    template <class Interface>
    static void SafeRelease(Interface *& pInterfaceToRelease)
    {
        if (pInterfaceToRelease)
        {
            pInterfaceToRelease->Release();
            pInterfaceToRelease = NULL;
        }
    }

    GPP::PointCloud* AppKinect2::CaptureOneDepth()
    {
        if (mpMultiSourceFrameReader == NULL)
        {
            GPPInfo << "CaptureOneDepth: mpMultiSourceFrameReader == NULL" << std::endl;
            return NULL;
        }
        IMultiSourceFrameArrivedEventArgs* pMultiArgs = NULL;
        if (!SUCCEEDED(mpMultiSourceFrameReader->GetMultiSourceFrameArrivedEventData(mMSEvent, &pMultiArgs)))
        {
            return NULL;
        }
        IMultiSourceFrameReference* pFrameRef = NULL;
        if (!SUCCEEDED(pMultiArgs->get_FrameReference(&pFrameRef)))
        {
            GPPInfo << "CaptureOneDepth: get_FrameReference failed" << std::endl;
            return NULL;
        }
        IMultiSourceFrame* pMultiSourceFrame = NULL;
        if (!SUCCEEDED(pFrameRef->AcquireFrame(&pMultiSourceFrame)))
        {
            GPPInfo << "CaptureOneDepth: pFrameRef->AcquireFrame failed" << std::endl;
            return NULL;
        }
        IDepthFrameReference* pDepthRef = NULL;
        if (!SUCCEEDED(pMultiSourceFrame->get_DepthFrameReference(&pDepthRef)))
        {
            GPPInfo << "CaptureOneDepth: pMultiSourceFrame->get_DepthFrameReference failed" << std::endl;
            return NULL;
        }
        IDepthFrame* pDepthFrame = NULL;
        if (!SUCCEEDED(pDepthRef->AcquireFrame(&pDepthFrame)))
        {
            return NULL;
        }
        UINT nDepthSize = 0;
        UINT16* pDepthBuffer = NULL;
        if (!SUCCEEDED(pDepthFrame->AccessUnderlyingBuffer(&nDepthSize, &pDepthBuffer)))
        {
            GPPInfo << "CaptureOneDepth: pDepthFrame->AccessUnderlyingBuffer failed" << std::endl;
            return NULL;
        }
        if (mFrameWidth < 0)
        {
            IFrameDescription* frameDescription = NULL;
            if (!SUCCEEDED(pDepthFrame->get_FrameDescription(&frameDescription)))
            {
                return NULL;
            }
            if (!SUCCEEDED(frameDescription->get_Width(&mFrameWidth)))
            {
                return NULL;
            }
            if (!SUCCEEDED(frameDescription->get_Height(&mFrameHeight)))
            {
                return NULL;
            }
            GPPInfo << "Frame size: " << mFrameWidth << " " << mFrameHeight << std::endl;
        }
        std::vector<CameraSpacePoint> depthPoints(nDepthSize);
        if (!SUCCEEDED(mpCoordinateMapper->MapDepthFrameToCameraSpace(nDepthSize, pDepthBuffer, 
            nDepthSize, &(depthPoints.at(0)))))
        {
            GPPInfo << "CaptureOneDepth: mpCoordinateMapper->MapDepthFrameToCameraSpace failed" << std::endl;
            return NULL;
        }
        SafeRelease(pDepthRef);
        SafeRelease(pDepthFrame);
        SafeRelease(pMultiArgs);
        GPP::PointCloud* grid = new GPP::PointCloud;
        grid->InitGrid(mFrameWidth, mFrameHeight);
        for (int hid = 0; hid < mFrameHeight; hid++)
        {
            for (int wid = 0; wid < mFrameWidth; wid++)
            {
                const CameraSpacePoint& point = depthPoints.at(hid * mFrameWidth + wid);
                if (point.Z > 0.1 && point.Z < 5 && point.X > -5 && point.X < 5 && point.Y > -5 && point.Y < 5)
                {
                    grid->SetGridCoord(wid, hid, GPP::Vector3(point.X, point.Y, point.Z));
                }
            }
        }
        return grid;
    }

    double AppKinect2::GetTime()
    {
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
        return ((counter - start) / double(frequency));
    }

    AppKinect2::~AppKinect2()
    {
        if (mpKinectSensor)
        {
            mpKinectSensor->Close();
        }
    }
}
