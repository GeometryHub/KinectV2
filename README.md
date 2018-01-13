# KinectV2
A scanner about Kinect V2

Usage:
0. Before running KinectV2 program, please install driver for Kinect V2: https://www.microsoft.com/en-us/download/details.aspx?id=44561
1. Click setbin.bat.
2. Run ./Build/bin/Release/scan.bat.
3. GUI hotkeys: ‘v’-view mode, point cloud is in blue color; ‘s’-scan mode, point cloud is in red color
4. Scanned point clouds will be save in directory ./Build/bin/Release/scan in gbg format

Build:
0. Creating a build directory Build in project root, which is the directory where CMake will create the
build system for your chosen platform and compiler.
1. The pre-build sdk of BGL and OGRE is based on Visual Studio 2015.
2. Compile the project and run setbin.bat to copy resources to target directories. 
