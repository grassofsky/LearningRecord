#pragma once

#include <cmath>
#include <algorithm>
#include <assert.h>

#include "geometry_structure.h"

namespace IntersectionTest
{
#define EPSION 1e-7

    enum IntersectionType
    {
        INTERSECTION,             //< 有相交线段
        DISJOINT,                 //< 不相交
        COPLANE                   //< 共面
    };

    bool IsZero(double value, double epsion = EPSION)
    {
        return std::abs(value) < epsion;
    }

    bool IsEqual(double v1, double v2, double epsion = EPSION)
    {
        return IsZero(v1-v2, epsion);
    }

    bool IsPositive(double value, double epsion = EPSION)
    {
        return value - epsion > 0;
    }

    bool IsNegative(double value, double epsion = EPSION)
    {
        return value + epsion < 0;
    }

    int GetSignType(double value)
    {
        if (IsZero(value)) return 0;
        if (IsPositive(value)) return 1;
        return -1;
    }

    template<typename T>
    void Swap(T &a, T &b)
    {
        auto tmp = a;
        a = b;
        b = tmp;
    }

    void GetVertexNewOrder(const int (&disVTri1SignType)[3], const double (&disVTri1TPlaneTri2)[3], int (&vertexTri1NewOrder)[3])
    {
        // 将顶点划分成两部分，0,2位于另一个三角形同一侧，1位于另一个三角形另一侧
        vertexTri1NewOrder[0] = 0;
        vertexTri1NewOrder[1] = 1;
        vertexTri1NewOrder[2] = 2;

        int prodValue = disVTri1SignType[0] * disVTri1SignType[1] * disVTri1SignType[2];
        // 如果乘积<0，则小于0的为单独的点
        if (prodValue < 0) {
            for (int i = 0; i < 3; ++i) {
                if (disVTri1TPlaneTri2[i] < 0) {
                    Swap(vertexTri1NewOrder[i], vertexTri1NewOrder[1]);
                    break;
                }
            }
        }
        // 如果乘积>0，则大于0的为单独的点
        else if (prodValue > 0) {
            for (int i = 0; i < 3; ++i) {
                if (disVTri1TPlaneTri2[i] > 0) {
                    Swap(vertexTri1NewOrder[i], vertexTri1NewOrder[1]);
                    break;
                }
            }
        }
        // 有点位于平面上
        else {
            int sumValue = disVTri1SignType[0] + disVTri1SignType[1] + disVTri1SignType[2];
            // 如果只有一个点等于0，并且另外两个点同号，那么等于0的点为单独的点
            if (std::abs(sumValue) == 2) {
                for (int i = 0; i < 3; ++i) {
                    if (disVTri1TPlaneTri2[i] == 0) {
                        Swap(vertexTri1NewOrder[i], vertexTri1NewOrder[1]);
                        break;
                    }
                }
            }
            // 如果只有一个点等于0，并且另外两个点异号，那么假定小于0的点为单独的点
            else if (std::abs(sumValue) == 0) {
                for (int i = 0; i < 3; ++i) {
                    if (disVTri1TPlaneTri2[i] < 0) {
                        Swap(vertexTri1NewOrder[i], vertexTri1NewOrder[1]);
                        break;
                    }
                }
            }
            // 如果两个点等于0，那么不等于0的点为单独的点
            else {
                for (int i = 0; i < 3; ++i) {
                    if (disVTri1TPlaneTri2[i] != 0) {
                        Swap(vertexTri1NewOrder[i], vertexTri1NewOrder[1]);
                        break;
                    }
                }
            }
        }
    }

    void CalculateT(
        const Eigen::Vector3d& vecNormalTri1,
        const double(&disVTri1TPlaneTri2)[3], 
        const int (&vertexTri1NewOrder)[3], 
        const Point3d (&verticesTri1)[3], 
        double (&tTri1)[2])
    {

        int maxValueIndex = 0;;
        double maxValue = vecNormalTri1[0];
        for (int i = 1; i < 3; ++i) {
            if (maxValue < vecNormalTri1[i]) {
                maxValue = vecNormalTri1[i];
                maxValueIndex = i;
            }
        }

        double pTri1OnLine[3] = {
            verticesTri1[vertexTri1NewOrder[0]](maxValueIndex),
            verticesTri1[vertexTri1NewOrder[1]](maxValueIndex),
            verticesTri1[vertexTri1NewOrder[2]](maxValueIndex) };

        double tTri1[2];
        tTri1[0] = pTri1OnLine[0] +
            (pTri1OnLine[1] - pTri1OnLine[0]) *
            disVTri1TPlaneTri2[vertexTri1NewOrder[0]] /
            (disVTri1TPlaneTri2[vertexTri1NewOrder[0]] - disVTri1TPlaneTri2[vertexTri1NewOrder[1]]);
        tTri1[1] = pTri1OnLine[2] +
            (pTri1OnLine[1] - pTri1OnLine[2]) *
            disVTri1TPlaneTri2[vertexTri1NewOrder[2]] /
            (disVTri1TPlaneTri2[vertexTri1NewOrder[2]] - disVTri1TPlaneTri2[vertexTri1NewOrder[1]]);
    }

    IntersectionType TriangleIntersectionTest(const Triangle& tri1, const Triangle& tri2) 
    {
        Point3d verticesTri1[3], verticesTri2[3];
        tri1.GetTriangleVertices(verticesTri1);
        tri2.GetTriangleVertices(verticesTri2);

        double disVTri2TPlaneTri1[3];
        int disVTri2SignType[3];
        double disVTri1TPlaneTri2[3];
        int disVTri1SignType[3];

        for (int i = 0; i < 3; ++i) {
            disVTri2TPlaneTri1[i] = tri1.GetDistanceFromPointToTrianglePlane(verticesTri2[i]);
            disVTri2SignType[i] = GetSignType(disVTri2TPlaneTri1[i]);
        }

        // 如果三角形Tri2的三个顶点在三角形Tri1的同一侧，则不相交
        if ((disVTri2SignType[0] > 0 && disVTri2SignType[1] > 0 && disVTri2SignType[2] > 0) ||
            (disVTri2SignType[0] < 0 && disVTri2SignType[1] < 0 && disVTri2SignType[2] < 0)) {
            return DISJOINT;
        }
        
        // 都为0，则共面
        if ((disVTri2SignType[0] | disVTri2SignType[1] | disVTri2SignType[2]) == 0) {
            return COPLANE;
        }
        
        for (int i = 0; i < 3; ++i) {
            disVTri1TPlaneTri2[i] = tri2.GetDistanceFromPointToTrianglePlane(verticesTri1[i]);
            disVTri1SignType[i] = GetSignType(disVTri1TPlaneTri2[i]);
        }

        // 如果三角形Tri1的三个顶点在三角形Tri2的同一侧，则不相交
        if ((disVTri1SignType[0] > 0 && disVTri1SignType[1] > 0 && disVTri1SignType[2] > 0) ||
            (disVTri1SignType[0] < 0 && disVTri1SignType[1] < 0 && disVTri1SignType[2] < 0)) {
            return DISJOINT;
        }

        // 都为0，则共面
        if ((disVTri1SignType[0] | disVTri1SignType[1] | disVTri1SignType[2]) == 0) {
            return COPLANE;
        }

        // 对顶点顺序进行调整，如何论文中的描述
        int vertexTri1NewOrder[3] = { 0, 1, 2 };
        int vertexTri2NewOrder[3] = { 0, 1, 2 };
        GetVertexNewOrder(disVTri1SignType, disVTri1TPlaneTri2, vertexTri1NewOrder);
        GetVertexNewOrder(disVTri2SignType, disVTri2TPlaneTri1, vertexTri2NewOrder);

        Eigen::Vector3d vecNormalTri1, vecNormalTri2;
        tri1.GetNormal(vecNormalTri1);
        tri2.GetNormal(vecNormalTri2);

        double tTri1[2], tTri2[2];
        // 计算相交线和每个三角形的相交线段
        CalculateT(vecNormalTri1, disVTri1TPlaneTri2, vertexTri1NewOrder, verticesTri1, tTri1);
        CalculateT(vecNormalTri2, disVTri2TPlaneTri1, vertexTri2NewOrder, verticesTri2, tTri2);

        if (tTri1[0] > tTri1[1]) Swap(tTri1[0], tTri1[1]);

        // 比较是否overlap
        if ((tTri2[0] > tTri1[0] && tTri2[0] < tTri1[1]) ||
            (tTri2[1] > tTri1[0] && tTri2[1] < tTri1[1])) {
            return INTERSECTION;
        }

        return DISJOINT;
    }

    void TestCase()
    {
        {
            Triangle tr1(Point3d(0, 0, 0), Point3d(1, 0, 1), Point3d(0, 1, 1));
            Triangle tr2(Point3d(1, 1, 0), Point3d(1, 1, 1), Point3d(0, 0, 1));

            auto type = IntersectionTest::TriangleIntersectionTest(tr1, tr2);

            assert(type == IntersectionTest::INTERSECTION);
        }

        {

            Triangle tr1(Point3d(0, 0, 0), Point3d(0, 0, 1), Point3d(1, 1, 0));
            Triangle tr2(Point3d(1, 1, 0), Point3d(1, 1, 1), Point3d(0, 0, 1));

            auto type = IntersectionTest::TriangleIntersectionTest(tr1, tr2);

            assert(type == IntersectionTest::COPLANE);
        }

        {
            Triangle tr1(Point3d(0, 0, 0), Point3d(0, 0, 1), Point3d(1, 1, 0));
            Triangle tr2(Point3d(1, 0, 1), Point3d(0, 1, 1), Point3d(1, 1, 1));

            auto type = IntersectionTest::TriangleIntersectionTest(tr1, tr2);

            assert(type == IntersectionTest::DISJOINT);
        }
    }
};