/*==================================================================================================

                       Copyright (c) 2017 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "Vector3.h"
#include <vector>

namespace GPP
{
    class BGL_EXPORT FitPointCloud
    {
    public:
        FitPointCloud();
        ~FitPointCloud();
        
        static bool FitPlane(const std::vector<Vector3>& supportCoords, const std::vector<Vector3>* supportNormals,
            Vector3& planeCenter, Vector3& planeNormal);

        static bool FitSphere(const std::vector<Vector3>& supportCoords, const std::vector<Vector3>& supportNormals,
            Vector3& center, Real& radius);

        static bool FitCylinder(const std::vector<Vector3>& supportCoords, const std::vector<Vector3>& supportNormals, const Vector3* initDir,
            Vector3& center, Vector3& dir, Real& radius);

        static bool FitCone(const std::vector<Vector3>& supportCoords, const std::vector<Vector3>& supportNormals,
            Vector3& apex, Vector3& dir, Real& angle);
        
    };
}
