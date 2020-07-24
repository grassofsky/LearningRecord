# GCompute  - 计算三角形对的交点（2）

上一篇文章中介绍了基于论文[PaperRead - A fast triangle-triangle intersection test](https://www.cnblogs.com/grass-and-moon/p/13297665.html)实现了[GCompute  - 计算三角形对的交点（1）](https://www.cnblogs.com/grass-and-moon/p/13366496.html)。这篇文章将从边和三角形相交的角度出发，计算交点。

[RTCD - 3.1 A math and geometry primer - matrix摘录](https://www.cnblogs.com/grass-and-moon/p/13277367.html)我们已经介绍了，如何判断一个点和一个三角形的关系。此处借用这个关系来计算三角形对之间的交点，大致思路如下：

- 首先计算三角形A的三个顶点和三角形B的位置关系；
- 根据位置关系，计算可能会有交点的边和三角形面的关系（直线和平面求交）；
- 计算交点的重心坐标，判断点是不是位于三角形内。

具体实现如下：

```c++
#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>
#include <assert.h>
#include <vector>
#include <iterator>

#include <Eigen/Dense>

typedef Eigen::Vector3d Point3d;

class Triangle 
{
public:
    Triangle(Point3d pt0, Point3d pt1, Point3d pt2) : m_pt {pt0, pt1, pt2} 
    {
        auto vecPt0TPt1 = pt1 - pt0;
        auto vecPt0TPt2 = pt2 - pt0;
        m_vecNormal = vecPt0TPt1.cross(vecPt0TPt2);
        m_vecNormal.normalize();
    }

    double GetDistanceFromPointToTrianglePlane(Point3d pt) const
    {
        auto vecPtTPt0 = m_pt[0] - pt;
        
        return m_vecNormal.dot(vecPtTPt0);
    }

    Point3d m_pt[3];
    Eigen::Vector3d m_vecNormal;
};


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

enum PositionType
{
    IN,
    OUT,
    ON
};

PositionType GetPositionType(const Triangle& tri, const Point3d& pt)
{
    Eigen::Matrix3d oriented;
    oriented << tri.m_pt[0].x() - pt.x(), tri.m_pt[0].y() - pt.y(), tri.m_pt[0].z() - pt.z(),
                tri.m_pt[1].x() - pt.x(), tri.m_pt[1].y() - pt.y(), tri.m_pt[1].z() - pt.z(),
                tri.m_pt[2].x() - pt.x(), tri.m_pt[2].y() - pt.y(), tri.m_pt[2].z() - pt.z();

    double value = oriented.determinant();
    if (IsNegative(value))
    {
        return OUT;
    } 
    else if (IsPositive(value))
    {
        return IN;
    }

    return ON;
}

Point3d CalInterPoint(const Eigen::Vector3d& planeNormal, const Point3d& ptOnPlane, const Eigen::Vector3d& lineDir, const Point3d& ptOnLine)
{
    double t = planeNormal.dot(ptOnPlane - ptOnLine) / planeNormal.dot(lineDir);
    return ptOnLine + lineDir * t;
}

bool CheckPtOnTriangle(const Triangle& tri, const Point3d& pt)
{
    Eigen::Vector3d v0 = tri.m_pt[1] - tri.m_pt[0], v1 = tri.m_pt[2] - tri.m_pt[0], v2 = pt - tri.m_pt[0];
    double d00 = v0.dot(v0);
    double d01 = v0.dot(v1);
    double d11 = v1.dot(v1);
    double d20 = v2.dot(v1);
    double d21 = v2.dot(v1);
    double denom = d00 * d11 - d01 * d01;
    double v = (d11 * d20 - d01 * d21) / denom;
    double w = (d00 * d21 - d01 * d20) / denom;
    if (v >= 0 && v <= 1 && w >= 0 && w <= 1)
    {
        return true;
    }
    return false;
}

IntersectionType GetIntersectionPoints(const Triangle& triPlane, const Triangle& triPoints, std::vector<Point3d>& pts)
{
    std::vector<Point3d> ptResult;

    PositionType relateToTriPlane[3] = {
        GetPositionType(triPlane, triPoints.m_pt[0]),
        GetPositionType(triPlane, triPoints.m_pt[1]),
        GetPositionType(triPlane, triPoints.m_pt[2])
    };

    // 平面和直线相交计算
    // 直线 P = V0 + dir*t
    // 平面 Normal \cdot (P - POn) = 0
    // =>
    // t = N \cdot (POn - V0) / N \cdot dir
    if (relateToTriPlane[0] == relateToTriPlane[1] && relateToTriPlane[1] == relateToTriPlane[2])
    {
        if (relateToTriPlane[0] == ON)
        {
            return COPLANE;
        }
        else
        {
            return DISJOINT;
        }
    }
    else if (relateToTriPlane[0] == relateToTriPlane[1])
    {
        Point3d inter1 = CalInterPoint(triPlane.m_vecNormal, triPlane.m_pt[0], triPoints.m_pt[2] - triPoints.m_pt[0], triPoints.m_pt[2]);
        Point3d inter2 = CalInterPoint(triPlane.m_vecNormal, triPlane.m_pt[0], triPoints.m_pt[2] - triPoints.m_pt[1], triPoints.m_pt[2]);

        if (CheckPtOnTriangle(triPlane, inter1)) ptResult.push_back(inter1);
        if (CheckPtOnTriangle(triPlane, inter2)) ptResult.push_back(inter2);
    }
    else if (relateToTriPlane[0] == relateToTriPlane[2])
    {
        Point3d inter1 = CalInterPoint(triPlane.m_vecNormal, triPlane.m_pt[0], triPoints.m_pt[1] - triPoints.m_pt[0], triPoints.m_pt[1]);
        Point3d inter2 = CalInterPoint(triPlane.m_vecNormal, triPlane.m_pt[0], triPoints.m_pt[1] - triPoints.m_pt[2], triPoints.m_pt[1]);

        if (CheckPtOnTriangle(triPlane, inter1)) ptResult.push_back(inter1);
        if (CheckPtOnTriangle(triPlane, inter2)) ptResult.push_back(inter2);
    }
    else if (relateToTriPlane[1] == relateToTriPlane[2])
    {
        Point3d inter1 = CalInterPoint(triPlane.m_vecNormal, triPlane.m_pt[0], triPoints.m_pt[0] - triPoints.m_pt[1], triPoints.m_pt[0]);
        Point3d inter2 = CalInterPoint(triPlane.m_vecNormal, triPlane.m_pt[0], triPoints.m_pt[0] - triPoints.m_pt[2], triPoints.m_pt[0]);

        if (CheckPtOnTriangle(triPlane, inter1)) ptResult.push_back(inter1);
        if (CheckPtOnTriangle(triPlane, inter2)) ptResult.push_back(inter2);
    }
    else // 有一个点位于三角平面上，另外两个点分别位于两边
    {
        if (relateToTriPlane[0] == ON && CheckPtOnTriangle(triPlane, triPoints.m_pt[0])) ptResult.push_back(triPoints.m_pt[0]);
        else if (relateToTriPlane[1] == ON && CheckPtOnTriangle(triPlane, triPoints.m_pt[1])) ptResult.push_back(triPoints.m_pt[0]);
        else if (relateToTriPlane[2] == ON && CheckPtOnTriangle(triPlane, triPoints.m_pt[2])) ptResult.push_back(triPoints.m_pt[0]);
    }

    if (ptResult.empty())
    {
        return DISJOINT;
    }

    std::move(begin(ptResult), end(ptResult), back_inserter(pts));

    return INTERSECTION;
}

void TriIntersectTestCase()
{
    {
        Triangle tr1(Point3d(0, 0, 0), Point3d(1, 0, 1), Point3d(0, 1, 1));
        Triangle tr2(Point3d(1, 1, 0), Point3d(1, 1, 1), Point3d(0, 0, 1));

        std::vector<Point3d> pts;
        auto type = GetIntersectionPoints(tr1, tr2, pts);

        assert(type == INTERSECTION);
        std::cout << "Intersection points: \n";
        for (int i = 0; i < pts.size(); ++i)
        {
            std::cout << "=====" << "\n";
            std::cout << pts[i] << "\n";
        }
    }

    {
        Triangle tr1(Point3d(0, 0, 0), Point3d(0, 0, 1), Point3d(1, 1, 0));
        Triangle tr2(Point3d(1, 1, 0), Point3d(1, 1, 1), Point3d(0, 0, 1));

        std::vector<Point3d> pts;
        auto type = GetIntersectionPoints(tr1, tr2, pts);

        assert(type == COPLANE);
        assert(pts.size() == 0);
    }

    {
        Triangle tr1(Point3d(0, 0, 0), Point3d(0, 0, 1), Point3d(1, 1, 0));
        Triangle tr2(Point3d(1, 0, 1), Point3d(0, 1, 1), Point3d(1, 1, 1));

        std::vector<Point3d> pts;
        auto type = GetIntersectionPoints(tr1, tr2, pts);

        assert(type == DISJOINT);
        assert(pts.size() == 0);
    }
}

int main()
{
    TriIntersectTestCase();
    return 0;
}
```

