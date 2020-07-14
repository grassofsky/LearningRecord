#pragma once

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

    void GetTriangleVertices(Point3d (&pt)[3]) const
    {
        pt[0] = m_pt[0];
        pt[1] = m_pt[1];
        pt[2] = m_pt[2];
    }

    void GetNormal(Eigen::Vector3d &vecNormal) const
    {
        vecNormal = m_vecNormal;
    }

private:
    Point3d m_pt[3];
    Eigen::Vector3d m_vecNormal;
};