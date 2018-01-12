/*==================================================================================================

                       Copyright (c) 2016 GeometryPlusPlus, ThreePark
                             Unpublished - All rights reserved

====================================================================================================*/
#pragma once
#include <string>
#include <vector>
#include <Vector3.h>

namespace GPP
{
    class PointCloud;
    class IPointCloud;
    class IGridPointCloud;
    class TriMesh;
    class ITriMesh;
    class BGL_EXPORT Parser
    {
    public:
        Parser();
        ~Parser();

        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: asc, obj
        static PointCloud* ImportPointCloud(std::string fileName);


        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: obj, stl, off, ply
        static TriMesh* ImportTriMesh(std::string fileName);


        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: obj, asc, ply
        static ErrorCode ExportPointCloud(std::string fileName, const IPointCloud* pointCloud);


        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: gtg, gbg
        static ErrorCode ExportGridPointCloud(std::string fileName, const IGridPointCloud* pointCloud);


        // fileName should contains the path name and fileName, like ../Models/PointCloud.asc or C:/Models/PointCloud.asc
        // Support format: obj, stl, ply
        static ErrorCode ExportTriMesh(std::string fileName, const ITriMesh* triMesh);


        /////////////////////////////////////////////// Internal Use ///////////////////////////////////////////////

        // meshColor == NULL: use vertex color
        static ErrorCode ExportTriMeshToPovray(std::string fileName, const TriMesh* triMesh, Vector3* meshColor);

        // Not test yet
        static ErrorCode ExportTextureTriMeshToPovray(std::string fileName, const TriMesh* triMesh);

        // line segment is a cylinder
        // radius is cylinder radius
        static ErrorCode ExportLineSegmentToPovray(std::string fileName, const std::vector<Vector3>& lineSegment, Real radius, Vector3 color);

    };
}
