/*==================================================================================================

                       Copyright (c) 2019 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "ITriMesh.h"
#include "IPointCloud.h"
#include <vector>
#include <sstream>

namespace GPP
{
    class BGL_EXPORT TestResult
    {
    public:
        TestResult();
        ~TestResult();

        void AddTriMesh(const ITriMesh* triMesh);
        void AddPointCloud(const IPointCloud* pointCloud);

        bool Compare2MasterRes(const TestResult& res) const;
        bool operator == (const TestResult& res) const;

        bool Import(const std::string& fileName);
        void Import(std::stringstream& ss);
        void Export(const std::string& fileName) const;

        void Clear(void);

    public:
        std::vector<Real> mScaleList;
        std::vector<Int> mIntList;
        std::vector<Vector3> mVec3List;
        static Real mScaleEpsilon;
    };

    extern BGL_EXPORT void DebugOnce(void);

    extern BGL_EXPORT bool GetDebugOnce(void);

    extern BGL_EXPORT void SetDebugOnce(bool debug);

    extern BGL_EXPORT TestResult* GetMasterTestResult(void);

    extern BGL_EXPORT void SetMasterTestResult(TestResult* result);

    extern BGL_EXPORT void ClearMasterResult(void);
}
