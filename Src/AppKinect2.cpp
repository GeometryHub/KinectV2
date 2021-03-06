/*==================================================================================================

                                  Copyright (c) 2018 GeometryHub
                                 Unpublished - All rights reserved

====================================================================================================*/
#include "AppKinect2.h"
#include <process.h>
#include <iostream>

namespace GHB
{
    template <class Interface>
    static void SafeRelease(Interface *& pInterfaceToRelease)
    {
        if (pInterfaceToRelease)
        {
            pInterfaceToRelease->Release();
            pInterfaceToRelease = NULL;
        }
    }

    static unsigned __stdcall RunDepthThread(void *arg)
    {
        AppKinect2* app = (AppKinect2*)arg;
        if (app == NULL)
        {
            return 0;
        }
        app->ExportDepthData();
        return 1;
    }
    
    static unsigned __stdcall RunImageThread(void *arg)
    {
        AppKinect2* app = (AppKinect2*)arg;
        if (app == NULL)
        {
            return 0;
        }
        app->ExportImageData();
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
        mDepthWidth(512),
        mDepthHeight(424),
        mColorWidth(1920),
        mColorHeight(1080),
        mExportDepthAcc(0),
        mExportImageAcc(0),
        mDepthCountAcc(0),
        mDepthTimeAcc(0),
        mIsScanDepth(false),
        mScanedDepthList(),
        mImageList(),
        mImageDataList(),
        mMapList(),
        mDepthMutex(),
        mImageMutex(),
        mBBoxMin(-5, -5, 0.5),
        mBBoxMax(5, 5, 5),
        mpColorBuffer(NULL),
        mImageInterval(0),
        mDepthId(0),
        mImageScale(1)
    {
        mpColorBuffer = new RGBQUAD[mColorWidth * mColorHeight];
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
            MessageBox(NULL, "Scan Mode", "Tip", MB_OK);
            mIsScanDepth = true;
        }
        else if (arg.key == OIS::KC_V)
        {
            MessageBox(NULL, "View Mode", "Tip", MB_OK);
            mIsScanDepth = false;
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

    void AppKinect2::SetBBox(const GPP::Vector3& bboxMin, const GPP::Vector3& bboxMax)
    {
        mBBoxMin = bboxMin;
        mBBoxMax = bboxMax;
    }

    void AppKinect2::SetImageInterval(int imageInterval)
    {
        mImageInterval = imageInterval;
    }

    void AppKinect2::SetImageScale(double scale)
    {
        mImageScale = scale;
        GPPInfo << "mImageScale = " << mImageScale << std::endl;
    }

    void AppKinect2::ExportDepthData()
    {
        while (mAppRunning)
        {
            std::string fileName;
            GPP::PointCloud* curDepth = NULL;
            {
                GPP::ScopedLock lock(mDepthMutex);
                if (!mScanedDepthList.empty())
                {
                    fileName = mScanedDepthList.at(0).first;
                    curDepth = mScanedDepthList.at(0).second;
                    mScanedDepthList.pop_front();
                    mExportDepthAcc++;
                }
            }
            if (curDepth == NULL)
            {
                GPP::SleepThread(0.03);
                continue;
            }
            double startTime = GPP::Profiler::GetTime();
            //curDepth->SetHasColor(false);
            curDepth->RemoveOuterBlankGrids();
            GPP::Parser::ExportGridPointCloud(fileName, curDepth);
            GPPFREEPOINTER(curDepth);
            double exportDepthTime = GPP::Profiler::GetTime() - startTime;
            if (exportDepthTime > 0.2)
            {
                GPPInfo << "    exportDepthTime=" << exportDepthTime << std::endl;
            }
        }
    }

    void AppKinect2::ExportImageData()
    {
        while (mAppRunning)
        {
            cv::Mat* image = NULL;
            RGBQUAD* imageData = NULL;
            std::vector<std::pair<short, short> > map;
            std::string imageName, mapName;
            {
                GPP::ScopedLock lock(mImageMutex);
                if (!mImageList.empty())
                {
                    image = mImageList.at(0).second;
                    imageName = mImageList.at(0).first;
                    mImageList.pop_front();
                    imageData = mImageDataList.at(0);
                    mImageDataList.pop_front();
                    map = mMapList.at(0).second;
                    mapName = mMapList.at(0).first;
                    mMapList.pop_front();
                    mExportImageAcc++;;
                }
            }
            if (image == NULL)
            {
                GPP::SleepThread(0.03);
                continue;
            }
            double startTime = GPP::Profiler::GetTime();
            if (mImageScale < 1.0 - GPP::REAL_TOL)
            {
                cv::Mat resizeImg(image->rows * mImageScale, image->cols * mImageScale, CV_8UC4);
                cv::resize(*image, resizeImg, cv::Size(image->cols * mImageScale, image->rows * mImageScale));
                cv::imwrite(imageName, resizeImg);
            }
            else
            {
                cv::imwrite(imageName, *image);
            }
            GPPFREEPOINTER(image);
            GPPFREEARRAY(imageData);

            FILE *fp = fopen(mapName.c_str(), "wb");
            int mapSize = map.size();
            fwrite(&mapSize, sizeof(int), 1, fp);
            for (std::vector<std::pair<short, short> >::iterator iitr = map.begin(); iitr != map.end(); ++iitr)
            {
                iitr->first *= mImageScale;
                iitr->second *= mImageScale;
                fwrite(&(iitr->first), sizeof(short), 1, fp);
                fwrite(&(iitr->second ), sizeof(short), 1, fp);
            }
            fclose(fp);
            double exportImageTime = GPP::Profiler::GetTime() - startTime;
            if (exportImageTime > 0.2)
            {
                GPPInfo << "        exportImageTime=" << exportImageTime << std::endl;
            }
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
            mpRenderWindow = mpRoot->initialise(true, "KinectV2");
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
        mpLight->setDiffuseColour(0.8, 0.8, 0.8);
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
            FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color,
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
        _beginthreadex(NULL, 0, RunDepthThread, (void *)this, 0, NULL);
        _beginthreadex(NULL, 0, RunDepthThread, (void *)this, 0, NULL);
        _beginthreadex(NULL, 0, RunImageThread, (void *)this, 0, NULL);
        if (mImageInterval > 0 && mImageInterval < 5)
        {
            _beginthreadex(NULL, 0, RunImageThread, (void *)this, 0, NULL);
        }
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
            int exportDepthAcc = 0;
            {
                GPP::ScopedLock lock(mDepthMutex);
                exportDepthAcc = mExportDepthAcc;
                mExportDepthAcc = 0;
            }
            int exportImageAcc = 0;
            {
                GPP::ScopedLock lock(mImageMutex);
                exportImageAcc = mExportImageAcc;
                mExportImageAcc = 0;
            }
            std::cout << "\r capture fps=" << int(double(mDepthCountAcc) / mDepthTimeAcc) <<
                " , depth fps=" << int(double(exportDepthAcc) / mDepthTimeAcc) <<
                ", image fps=" << int(double(exportImageAcc) / mDepthTimeAcc) << "    ";
            mDepthTimeAcc = 0;
            mDepthCountAcc = 0;
        }
    }

    void AppKinect2::UpdateInputSystem()
    {
        mpKeyboard->capture();
        Ogre::WindowEventUtilities::messagePump();
    }

    void AppKinect2::UpdateKinectData()
    {
        if (mpMultiSourceFrameReader == NULL)
        {
            GPPInfo << "CaptureOneDepth: mpMultiSourceFrameReader == NULL" << std::endl;
            return;
        }
        IMultiSourceFrameArrivedEventArgs* pMultiArgs = NULL;
        if (!SUCCEEDED(mpMultiSourceFrameReader->GetMultiSourceFrameArrivedEventData(mMSEvent, &pMultiArgs)))
        {
            //GPPInfo << "CaptureOneDepth: GetMultiSourceFrameArrivedEventData failed" << std::endl;
            return;
        }
        IMultiSourceFrameReference* pFrameRef = NULL;
        if (!SUCCEEDED(pMultiArgs->get_FrameReference(&pFrameRef)))
        {
            GPPInfo << "CaptureOneDepth: get_FrameReference failed" << std::endl;
            return;
        }
        IMultiSourceFrame* pMultiSourceFrame = NULL;
        if (!SUCCEEDED(pFrameRef->AcquireFrame(&pMultiSourceFrame)))
        {
            GPPInfo << "CaptureOneDepth: pFrameRef->AcquireFrame failed" << std::endl;
            return;
        }

        IDepthFrameReference* pDepthRef = NULL;
        if (!SUCCEEDED(pMultiSourceFrame->get_DepthFrameReference(&pDepthRef)))
        {
            GPPInfo << "CaptureOneDepth: pMultiSourceFrame->get_DepthFrameReference failed" << std::endl;
            return;
        }
        IDepthFrame* pDepthFrame = NULL;
        if (!SUCCEEDED(pDepthRef->AcquireFrame(&pDepthFrame)))
        {
            GPPInfo << "CaptureOneDepth: pDepthRef->AcquireFrame failed" << std::endl;
            return;
        }
        UINT nDepthSize = 0;
        UINT16* pDepthBuffer = NULL;
        if (!SUCCEEDED(pDepthFrame->AccessUnderlyingBuffer(&nDepthSize, &pDepthBuffer)))
        {
            GPPInfo << "CaptureOneDepth: pDepthFrame->AccessUnderlyingBuffer failed" << std::endl;
            return;
        }
        std::vector<CameraSpacePoint> depthPoints(nDepthSize);
        if (!SUCCEEDED(mpCoordinateMapper->MapDepthFrameToCameraSpace(nDepthSize, pDepthBuffer,
            nDepthSize, &(depthPoints.at(0)))))
        {
            GPPInfo << "CaptureOneDepth: mpCoordinateMapper->MapDepthFrameToCameraSpace failed" << std::endl;
            return;
        }
        std::vector<ColorSpacePoint> depth2ColorMaps(nDepthSize);
        if (!SUCCEEDED(mpCoordinateMapper->MapDepthFrameToColorSpace(nDepthSize, pDepthBuffer,
            nDepthSize, &(depth2ColorMaps.at(0)))))
        {
            GPPInfo << "CaptureOneDepth: mpCoordinateMapper->MapDepthFrameToColorSpace failed" << std::endl;
            return;
        }
        SafeRelease(pDepthRef);
        SafeRelease(pDepthFrame);

        IColorFrameReference* pColorRef = NULL;
        if (!SUCCEEDED(pMultiSourceFrame->get_ColorFrameReference(&pColorRef)))
        {
            GPPInfo << "CaptureOneDepth: pMultiSourceFrame->get_DepthFrameReference failed" << std::endl;
            return;
        }
        IColorFrame* pColorFrame = NULL;
        if (!SUCCEEDED(pColorRef->AcquireFrame(&pColorFrame)))
        {
            GPPInfo << "CaptureOneDepth: pColorRef->AcquireFrame failed" << std::endl;
            return;
        }
        ColorImageFormat imageFormat = ColorImageFormat_None;
        if (!SUCCEEDED(pColorFrame->get_RawColorImageFormat(&imageFormat)))
        {
            GPPInfo << "CaptureOneDepth: get_RawColorImageFormat failed" << std::endl;
            return;
        }
        UINT nColorSize = 0;
        RGBQUAD *pColorBuffer = NULL;
        if (imageFormat == ColorImageFormat_Bgra)
        {
            if (!SUCCEEDED(pColorFrame->AccessRawUnderlyingBuffer(&nColorSize, reinterpret_cast<BYTE**>(&pColorBuffer))))
            {
                GPPInfo << "CaptureOneDepth: AccessRawUnderlyingBuffer failed" << std::endl;
                return;
            }
        }
        else if (mpColorBuffer)
        {
            pColorBuffer = mpColorBuffer;
            nColorSize = mColorWidth * mColorHeight * sizeof(RGBQUAD);
            if (!SUCCEEDED(pColorFrame->CopyConvertedFrameDataToArray(nColorSize, reinterpret_cast<BYTE*>(pColorBuffer), ColorImageFormat_Bgra)))
            {
                GPPInfo << "CaptureOneDepth: CopyConvertedFrameDataToArray failed" << std::endl;
                return;
            }
        }
        else
        {
            GPPInfo << "CaptureOneDepth: imageFormat != ColorImageFormat_Bgra" << std::endl;
            return;
        }
        SafeRelease(pColorRef);
        SafeRelease(pColorFrame);
        SafeRelease(pMultiArgs);

        bool captureImg = mIsScanDepth && (mImageInterval > 0) && (mDepthId % mImageInterval == 0);
        GPP::PointCloud* grid = new GPP::PointCloud;
        grid->InitGrid(mDepthWidth, mDepthHeight);
        grid->SetHasColor(true);
        std::vector<std::pair<short, short> > depthMap;
        for (int wid = 0; wid < mDepthWidth; wid++)
        {
            for (int hid = 0; hid < mDepthHeight; hid++)
            {
                const CameraSpacePoint& point = depthPoints.at(hid * mDepthWidth + wid);
                if (point.Z > mBBoxMin[2] && point.Z < mBBoxMax[2] &&
                    point.X > mBBoxMin[0] && point.X < mBBoxMax[0] &&
                    point.Y > mBBoxMin[1] && point.Y < mBBoxMax[1])
                {
                    const ColorSpacePoint& colorCoord = depth2ColorMaps.at(hid * mDepthWidth + wid);
                    if (colorCoord.X < 0 || colorCoord.X >= mColorWidth || colorCoord.Y < 0 || colorCoord.Y >= mColorHeight)
                    {
                        continue;
                    }
                    grid->SetGridCoord(wid, hid, GPP::Vector3(point.X, point.Y, point.Z));
                    RGBQUAD* pixel = pColorBuffer + (int(colorCoord.Y) * mColorWidth + int(colorCoord.X));
                    GPP::Color4 color(pixel->rgbRed, pixel->rgbGreen, pixel->rgbBlue);
                    grid->SetGridColor(wid, hid, GPP::Color4::Color4ToVector3(color));
                    if (captureImg)
                    {
                        depthMap.push_back(std::pair<short, short>(colorCoord.X, mColorHeight - colorCoord.Y - 1));
                    }
                }
            }
        }
        if (grid->GetPointCount() < 10)
        {
            GPPFREEPOINTER(grid);
            return;
        }
        if (mDepthCountAcc % 6 == 0)
        {
            RenderPointCloud(grid);
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
            {
                GPP::ScopedLock lock(mDepthMutex);
                mScanedDepthList.push_back(std::pair<std::string, GPP::PointCloud*>(fileName, grid));
            }
            GPP::SleepThread(0.001);
            if (captureImg)
            {
                ss.clear();
                ss << "./scan/depth_" << counter << ".png";
                std::string imgName;
                ss >> imgName;
                ss.clear();
                ss << "./scan/depth_" << counter << ".bmap";
                std::string mapName;
                ss >> mapName;
                RGBQUAD* colorBufferCopy = new RGBQUAD[mColorWidth * mColorHeight];
                memcpy(colorBufferCopy, pColorBuffer, sizeof(RGBQUAD) * mColorWidth * mColorHeight);
                cv::Mat* img = new cv::Mat(mColorHeight, mColorWidth, CV_8UC4, reinterpret_cast<uchar*>(colorBufferCopy));
                {
                    GPP::ScopedLock lock(mImageMutex);
                    mImageList.push_back(std::pair<std::string, cv::Mat*>(imgName, img));
                    mImageDataList.push_back(colorBufferCopy);
                    mMapList.push_back(std::pair<std::string, std::vector<std::pair<short, short> > >(mapName, depthMap));
                }
                GPP::SleepThread(0.001);
            }
            mDepthId++;
        }
        else
        {
            GPPFREEPOINTER(grid);
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
        for (int pid = 0; pid < pointCount; pid += 2)
        {
            GPP::Vector3 coord = pointCloud->GetPointCoord(pid);
            GPP::Vector3 color = pointCloud->GetPointColor(pid);
            manualObj->position(coord[0], coord[1], coord[2]);
            manualObj->colour(color[0], color[1], color[2]);
        }
        manualObj->end();
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
        GPPFREEARRAY(mpColorBuffer);
    }
}
