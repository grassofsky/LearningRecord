#define CGAL_NO_GMP 1

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/box_intersection_d.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>

#include <vector>
#include <fstream>
#include <chrono>

typedef CGAL::Exact_predicates_inexact_constructions_kernel   Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>                   Mesh;
typedef CGAL::property_map_selector<Mesh, CGAL::vertex_point_t>::type Vpm;

const double inf = -std::numeric_limits<double>::max();
const double up = std::numeric_limits<double>::max();

struct MyBox 
{
    void UpdateBox()
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (low[i] > pt[j][i]) low[i] = pt[j][i];
                if (high[i] < pt[j][i]) high[i] = pt[j][i];
            }
        }
    }

    double low[3] = { up, up, up };
    double high[3] = { inf, inf, inf };
    Kernel::Point_3 pt[3];
};

struct Traits 
{
    typedef double NT;
    typedef MyBox& Box_parameter;
    typedef std::ptrdiff_t ID;

    static int dimension() { return 3; }
    static double min_coord(Box_parameter b, int d)
    {
        return b.low[d];
    }
    static double max_coord(Box_parameter b, int d)
    {
        return b.high[d];
    }
    static std::ptrdiff_t id(Box_parameter b) { return (std::ptrdiff_t)(&b); }
};

void ReadMesh(Mesh& mesh, std::string offFilename)
{
    std::ifstream input(offFilename);
    if (!input || !(input >> mesh) || !CGAL::is_triangle_mesh(mesh)) {
        std::cerr << "Not a valid input file" << std::endl;
        return;
    }
}

std::map<std::string, const MyBox*> mapIterBox;

std::string GenerateUID(const MyBox* a)
{
    std::stringstream ss;
    ss << a;
    return ss.str();
}

void callback(const MyBox& a, const MyBox& b)
{
    std::string uid = GenerateUID(&a);
    if (mapIterBox.count(uid) == 0)
    {
        mapIterBox[uid] = &a;
    }
    uid = GenerateUID(&b);
    if (mapIterBox.count(uid) == 0)
    {
        mapIterBox[uid] = &b;
    }
}

void WriteResult(const std::string& fileout)
{
    std::ofstream of;
    of.open(fileout, std::ios::out);
    of << "STLFILE\n";
    for (auto iter = mapIterBox.begin(); iter != mapIterBox.end(); ++iter)
    {
        auto pBox = iter->second;
        Kernel::Vector_3 normal = CGAL::cross_product(pBox->pt[0] - pBox->pt[1], pBox->pt[0] - pBox->pt[2]);
        double tmp = std::sqrtf(normal.x() * normal.x() + normal.y() * normal.y() + normal.z() * normal.z());
        of << "facet normal " << normal.x() / tmp << " " << normal[1] / tmp << " " << normal[2] / tmp << "\n";
        of << "outer loop\n";
        for (int j = 0; j < 3; ++j)
        {
            auto pt = pBox->pt[j];
            of << "vertex " << pt[0] << " " << pt[1] << " " << pt[2] << "\n";
        }
        of << "endloop\n";
        of << "endfacet\n";
    }
    of << "endsolidfilenamestl";
    of.close();
}

int main(int argc, char* argv[])
{
    Mesh mesh1, mesh2;
    std::string file1 = "E:/stl/Reamers52.off";
    std::string file2 = "E:/stl/semi-sphere.off";

    ReadMesh(mesh1, file1);
    ReadMesh(mesh2, file2);

    Vpm vpm1 = get_property_map(boost::vertex_point, mesh1);
    Vpm vpm2 = get_property_map(boost::vertex_point, mesh2);

    std::vector<MyBox> boxes1, boxes2;
    std::vector<MyBox*> ptr1, ptr2;
    auto f_range = mesh1.faces();
    for (auto f_iter = f_range.begin(); f_iter != f_range.end(); ++f_iter)
    {
        auto h = mesh1.halfedge(*f_iter);
        MyBox box;
        box.pt[0] = get(vpm1, source(h, mesh1));
        box.pt[1] = get(vpm1, target(h, mesh1));
        box.pt[2] = get(vpm1, target(next(h, mesh1), mesh1));
        box.UpdateBox();
        boxes1.push_back(box);
    }

    ptr1.resize(boxes1.size());
    for (int i = 0; i < boxes1.size(); ++i)
    {
        ptr1[i] = &boxes1[i];
    }

    f_range = mesh2.faces();
    for (auto f_iter = f_range.begin(); f_iter != f_range.end(); ++f_iter)
    {
        auto h = mesh2.halfedge(*f_iter);
        MyBox box;
        box.pt[0] = get(vpm2, source(h, mesh2));
        box.pt[1] = get(vpm2, target(h, mesh2));
        box.pt[2] = get(vpm2, target(next(h, mesh2), mesh2));
        box.UpdateBox();
        boxes2.push_back(box);
    }

    ptr2.resize(boxes2.size());
    for (int i = 0; i < boxes2.size(); ++i)
    {
        ptr2[i] = &boxes2[i];
    }

    // Run the self intersection algorithm with all defaults on the
    // indirect pointers to bounding boxes. Avoids copying the boxes.
    auto t1 = std::chrono::steady_clock::now();
    CGAL::box_intersection_d(boxes1.begin(), boxes1.end(), boxes2.begin(), boxes2.end(), callback, Traits());
    auto t2 = std::chrono::steady_clock::now();
    std::cout << "Intersection test cost: " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms" << std::endl;
    WriteResult("E:/stl/intersect_result_box_cgal.stl");
    return 0;
}


