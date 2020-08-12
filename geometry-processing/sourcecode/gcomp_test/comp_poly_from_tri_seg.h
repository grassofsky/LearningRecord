#pragma once

#include <Eigen/Dense>
#include <vector>
#include <set>
#include <map>

typedef Eigen::Vector2d Point2d;
typedef int EdgeHandle;
typedef std::map<EdgeHandle, std::vector<int>> MapOrderPtOnEdge;

enum PositionType
{
    OnEdge,
    OnFace,
    OnVertex,
};

struct NodeGraph
{
    std::set<int> nodes;
}

struct Node
{
    Node() = default;
    Node(const Point2d& pt, PositionType ptType, const std::string& name) :
     pt(pt), ptType(ptType), name(name) {}
    Node(const Point2d& pt, PositionType ptType, const std::string& name, EdgeHandle handle) :
     pt(pt), ptType(ptType), name(name), edgeHandle(handle) {}
    Point2d pt;
    PositionType ptType;
    std::string name;
    EdgeHandle edgeHandle = -1;
};

struct Triangle
{
    Point2d vertex[3];
};

struct Segment
{
    Segment() = default;
    Segment(int a, int b) : nodeId{a, b} {}
    int nodeId[2];
};

MapOrderPtOnEdge FilterPtOnEdgeAndSort(const std::vector<Node>& nodes, const Triangle& tri)
{
    MapOrderPtOnEdge mapOrderPtOnEdge;

    for (int i=0, size=nodes.size(); i < size; ++i)
    {
        if (nodes[i].ptType == OnEdge)
        {
            mapOrderPtOnEdge.insert_or_assign(nodes[i].edgeHandle, i);
        }
    }

    for (auto iter = mapOrderPtOnEdge.begin(); iter != mapOrderPtOnEdge.end(); ++iter)
    {
        if (iter->second.size() >= 2)
        {
            auto edgeHandle = iter->first;
            std::sort(iter->second.begin(), iter->second.end(), [&nodes, &tri, &edgeHandle](int a, int b) {
                return (nodes[a].pt - tri.vertex[edgeHandle]).norm() < (nodes[b].pt - tri.vertex[edgeHandle]).norm();
            });
        }
    }

    return mapOrderPtOnEdge;
}

std::vector<Point2d> GetStartPts(const MapOrderPtOnEdge& mapOrderPtOnEdge, const Triangle& tri)
{
    std::vector<Point2d> vecPts;
    vecPts.emplace_back(tri.vertex[0]);
    EdgeHandle preEdgeHandle = 0;
    for (auto iter=mapOrderPtOnEdge.begin(); iter!=mapOrderPtOnEdge.end(); ++iter)
    {
        while (preEdgeHandle != iter->first)
        {
            preEdgeHandle++;
            vecPts.emplace_back(tri.vertex[preEdgeHandle]);
        }

        vecPts.insert(vecPts.end(), iter->second.begin(), iter->second.end());
    }

    if (preEdgeHandle == 0)
    {
        vecPts.emplace_back(tri.vertex[1]);
        vecPts.emplace_back(tri.vertex[2]);
    }
    else if (preEdgeHandle == 1)
    {
        vecPts.emplace_back(tri.vertex[2]);
    }

    return vecPts;
}

int main()
{
    Triangle tri;
    tri.vertex[0] = Point2d(5,4);        // A
    tri.vertex[1] = Point2d(-1.5,-5);    // B
    tri.vertex[2] = Point2d(15.5, -7);   // C

    std::vector<Node> nodes;
    nodes.emplace_back(Point2d(2.88, 1.06), OnEdge, "K", 0);        // 0
    nodes.emplace_back(Point2d(3.5, 0.5), OnFace, "J");          // 1
    nodes.emplace_back(Point2d(6.5, 1), OnFace, "I");            // 2
    nodes.emplace_back(Point2d(6.6, 2.32), OnEdge, "H", 2);         // 3
    nodes.emplace_back(Point2d(4.7, -1.8), OnFace, "L");         // 4
    nodes.emplace_back(Point2d(2.6, -3), OnFace, "M");           // 5
    nodes.emplace_back(Point2d(6, -5), OnFace, "N");             // 6
    nodes.emplace_back(Point2d(9.26, -6.27), OnEdge, "G", 1);       // 7
    nodes.emplace_back(Point2d(8, -4), OnFace, "F");             // 8
    nodes.emplace_back(Point2d(9, -3), OnFace, "E");             // 9
    nodes.emplace_back(Point2d(11.78, -3.1), OnEdge, "D", 2);       // 10

    std::vector<Segment> segments;
    segments.emplace_back(0, 1);
    segments.emplace_back(1, 2);
    segments.emplace_back(2, 3);
    segments.emplace_back(4, 5);
    segments.emplace_back(5, 6);
    segments.emplace_back(6, 4);
    segments.emplace_back(7, 8);
    segments.emplace_back(8, 9);
    segments.emplace_back(9, 10);


    std::vector<NodeGraph> vecNodeGraph;
    vecNodeGraph.resize(nodes.size());
    for (int i=0; i<segments.size(); ++i)
    {
        vecNodeGraph[segments[i].nodeId[0]].nodes.insert(segments[i].nodeId[1]);
        vecNodeGraph[segments[i].nodeId[1]].nodes.insert(segments[i].nodeId[0]);
    }

    auto mapOrderPtOnEdge = FilterPtOnEdgeAndSort(nodes, tri);
    std::vector<Point2d> vecStartPts = GetStartPts(mapOrderPtOnEdge, tri);

    std::vector<std::vector<Point2d>> vecPolygons;
    std::vector<Point2d> vecPolygon;
    while (FindNextStartPt(pt))
    {

    }
}