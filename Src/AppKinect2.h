/*==================================================================================================

                                   Copyright (c) 2018 GeometryHub
                                  Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Ogre.h"
#include "OIS.h"
#include "Kinect.h"
#include "opencv2/opencv.hpp"
#include "BGL.h"
#include "ThreadPool.h"
#include <deque>

namespace GHB
{
    class AppKinect2 : 
        public OIS::KeyListener,
        public Ogre::WindowEventListener
    {
    public:
        AppKinect2();
        void Run(void);
        virtual ~AppKinect2();
        virtual bool keyPressed(const OIS::KeyEvent &arg);
        virtual bool keyReleased(const OIS::KeyEvent &arg);
        virtual void windowResized(Ogre::RenderWindow* rw);
        virtual bool windowClosing(Ogre::RenderWindow* rw);

        void SetBBox(const GPP::Vector3& bboxMin, const GPP::Vector3& bboxMax);
        void SetImageInterval(int imageInterval);
        void ExportDepthData(void);
        void ExportImageData(void);

    private:
        void Init(void);
        void LoadResource(std::string path, std::string type, std::string groupName, bool recursive);
        void Update(double timeElapsed);
        void UpdateInputSystem(void);
        void UpdateKinectData(void);
        void RenderPointCloud(const GPP::PointCloud* pointCloud);
        double GetTime(void);

    private:
        bool mAppRunning;
        Ogre::Root*    mpRoot;
        Ogre::Camera*  mpMainCamera;
        Ogre::RenderWindow* mpRenderWindow;
        Ogre::SceneManager* mpSceneManager;
        Ogre::Viewport* mpViewport;
        Ogre::Light* mpLight;
        OIS::InputManager* mpInputManager;
        OIS::Keyboard*     mpKeyboard;
        double mTimeAccumulate;
        double mRenderDeltaTime;
        IKinectSensor*           mpKinectSensor;
        IMultiSourceFrameReader* mpMultiSourceFrameReader;
        ICoordinateMapper*       mpCoordinateMapper;
        WAITABLE_HANDLE mMSEvent;
        int mDepthWidth;
        int mDepthHeight;
        int mColorWidth;
        int mColorHeight;
        int mExportDepthAcc;
        int mExportImageAcc;
        int mDepthCountAcc;
        double mDepthTimeAcc;
        bool mIsScanDepth;
        std::deque<std::pair<std::string, GPP::PointCloud*> > mScanedDepthList;
        std::deque<std::pair<std::string, cv::Mat*> > mImageList;
        std::deque<std::pair<std::string, std::vector<std::pair<short, short> > > > mMapList;
        GPP::Mutex mDepthMutex;
        GPP::Mutex mImageMutex;
        GPP::Vector3 mBBoxMin;
        GPP::Vector3 mBBoxMax;
        RGBQUAD* mpColorBuffer;
        int mImageInterval;
        int mDepthId;
    };
}
