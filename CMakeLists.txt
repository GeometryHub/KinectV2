cmake_minimum_required(VERSION 2.4)
project(KinectV2)
option(DEBUG_mode "ON for debug or OFF for release" OFF)
include_directories(${PROJECT_SOURCE_DIR}/Dependencies/OGRE/include/OGRE
                    ${PROJECT_SOURCE_DIR}/Dependencies/OGRE/include/OIS
                    ${PROJECT_SOURCE_DIR}/Dependencies/Kinect2/include
                    ${PROJECT_SOURCE_DIR}/Dependencies/BasicGeometryLib/include)
link_directories(${PROJECT_SOURCE_DIR}/Dependencies/OGRE/lib
                 ${PROJECT_SOURCE_DIR}/Dependencies/Kinect2/lib
                 ${PROJECT_SOURCE_DIR}/Dependencies/BasicGeometryLib/lib)
if(DEBUG_mode)
    link_libraries(OgreMain_d.lib RenderSystem_GL_d.lib OIS_d.lib Kinect20.lib basicgeometrylib_d.lib)
else()
    link_libraries(OgreMain.lib RenderSystem_GL.lib OIS.lib Kinect20.lib basicgeometrylib.lib)
endif()
set(APP_SRC 
    Src/AppKinect2.cpp
    Src/main.cpp)
add_definitions("-D BGL_DLL_EXPORT")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(KinectV2 ${APP_SRC})
