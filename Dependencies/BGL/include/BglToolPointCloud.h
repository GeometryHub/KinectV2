/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include "PointCloud.h"
#include "IPointList.h"
#include "Matrix4x4.h"
#include "PointCloudInfo.h"
#include <vector>

namespace GPP
{
    extern BGL_EXPORT PointCloud* CopyPointCloud(const PointCloud* pointCloud);


    extern BGL_EXPORT ErrorCode CopyPointCloud(const IPointCloud* pointCloud, IPointCloud* copyCloud);

    
    // deleteIndex could have duplicate index
    // originPointIds->at(pid_AfterDelete) = pid_OriginId
    extern BGL_EXPORT ErrorCode DeletePointCloudElements(IPointCloud* pointCloud, const std::vector<Int>& deleteIndex,
        std::vector<Int>* originPointIds = NULL);


    // keepIndex could have duplicate index
    // originPointIds->at(pid_AfterDelete) = pid_OriginId
    extern BGL_EXPORT ErrorCode KeeyPointCloudElements(IPointCloud* pointCloud, const std::vector<Int>& keepIndex, 
        std::vector<Int>* originPointIds = NULL);
    

    extern BGL_EXPORT ErrorCode CalculatePointListBoundingBox(const IPointList* pointlist, const Matrix4x4* transform, 
        Vector3& bboxMin, Vector3& bboxMax);

    
    // transformList == NULL means identity transform
    extern BGL_EXPORT ErrorCode CalculatePointCloudListBoundingBox(const std::vector<IPointCloud*> pointCloudList, 
        const std::vector<Matrix4x4>* transformList, Vector3& bboxMin, Vector3& bboxMax);


    extern BGL_EXPORT ErrorCode CalculatePointCloudDensity(const IPointCloud* pointCloud, PointCloudInfo* pointCloudInfo,
        Int neighborCount, Real& density, const std::vector<Int>* selectIndex = NULL);


    extern BGL_EXPORT ErrorCode CalculatePointListDensity(const IPointList* pointList, Int neighborCount, Real& density);


    extern BGL_EXPORT ErrorCode CalculateGridDensity(const IGridPointCloud* pointCloud, Real& density);

    
    // pointCloud's normal will be rotated if it has normal
    extern BGL_EXPORT void TransformPointCloud(IPointCloud* pointCloud, const Matrix4x4& transform);


    // User can set specific appendColor if refCloud has color infomation
    extern BGL_EXPORT ErrorCode AppendPointCloud(IPointCloud* refCloud, const IPointCloud* appendCloud, 
        const Matrix4x4* appendTransform, const Vector3* appendColor);


    // depthData should be depth point cloud and has normal
    // if sampleList == NULL, it means all points are sampled
    // NOTE: cameraDir is opposite to point cloud normal
    extern BGL_EXPORT ErrorCode EstimateCameraDirection(const IPointCloud* depthData, const std::vector<Int>* sampleList, Vector3& cameraDir);


    extern BGL_EXPORT ErrorCode DetectGridBoundary(const IGridPointCloud* pointCloud, Int boundarySize, std::vector<Int>& boundaryIds);


    // pointCloud should have normal
    // neighborSize >= 5
    extern BGL_EXPORT ErrorCode DetectPointCloudBoundary(const IPointCloud* pointCloud, PointCloudInfo* cloudInfo, Int neighborSize,
        Int boundarySize, std::vector<Int>& boundaryIds);


    // pointCloud should have normal
    extern BGL_EXPORT ErrorCode ComputePointCloudFieldGradient(const IPointCloud* pointCloud, PointCloudInfo* info,
        const std::vector<Real>& fields, Int neighborCount, std::vector<Vector3>* gradients);


    extern BGL_EXPORT void UnifyPointList(std::vector<Vector3>& pointList, Real bboxSize, Real* scaleValue = NULL, Vector3* objCenterCoord = NULL);


    extern BGL_EXPORT void UnifyPointList(std::vector<Vector3>& pointList, Real scaleValue, const Vector3& objCenterCoord);

}
