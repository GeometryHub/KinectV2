/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/

#pragma once
#include "IPointCloud.h"

namespace GPP
{
    class Vector3;
    class BGL_EXPORT Color4
    {
    public:
        unsigned char mRed;
        unsigned char mGreen;
        unsigned char mBlue;
        unsigned char mAlpha;

    public:
        Color4();
        Color4(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

        unsigned char& operator[](int index);
        unsigned char operator[] (int index)const;

        static Int Color4ToInt(const Color4& color);
        static Color4 IntToColor4(Int c);

        // this function will first trim the component of input v3 into [0.0, 1.0], and then convert them to [0, 255]
        static Color4 Vector3ToColor4(const Vector3& v3);
        // this functino will convert Color4 [0, 255] to Vector3 [0, 1]
        static Vector3 Color4ToVector3(const Color4& color);
    };

    
    // rgb range [0, 1]
    // hsv range: h-[0, 360], s-[0, 1], v-[0, 1]
    // H(hue), S(saturation), V(value)
    extern BGL_EXPORT Vector3 ConvertRGB2HSV(const Vector3& rgb);


    extern BGL_EXPORT Vector3 ConvertHSV2RGB(const Vector3& hsv);


    extern BGL_EXPORT ErrorCode UnifyPointCloudColor(IPointCloud* pointCloud, Real* h, Real* s, Real* v);
}
