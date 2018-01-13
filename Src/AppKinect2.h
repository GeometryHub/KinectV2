/*==================================================================================================

                                   Copyright (c) 2018 GeometryHub
                                  Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Ogre.h"
#include "OIS.h"
#include "Kinect.h"
#include "BGL.h"
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
        void ExportDepthData(void);

    private:
        void Init(void);
        void LoadResource(std::string path, std::string type, std::string groupName, bool recursive);
        void Update(double timeElapsed);
        void UpdateInputSystem(void);
        void UpdateKinectData(void);
        void RenderPointCloud(const GPP::PointCloud* pointCloud);
        GPP::PointCloud* CaptureOneDepth(void);
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
        int mFrameWidth;
        int mFrameHeight;
        int mExportCountAcc;
        int mDepthCountAcc;
        double mDepthTimeAcc;
        bool mIsScanDepth;
        std::deque<std::pair<std::string, GPP::PointCloud*> > mScanedDepthList;
        GPP::Mutex mMutex;
        GPP::Vector3 mBBoxMin;
        GPP::Vector3 mBBoxMax;
    };
}
