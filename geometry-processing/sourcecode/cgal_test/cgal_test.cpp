#define CGAL_NO_GMP 1

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/box_intersection_d.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Surface_mesh.h>

#include <vector>
#include <fstream>
#include <chrono>

typedef CGAL::Exact_predicates_inexact_constructions_kernel   Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>                   Mesh;
typedef CGAL::Box_intersection_d::Box_with_info_d<double, 3, Mesh::Halfedge_index> Box;
typedef CGAL::property_map_selector<Mesh, CGAL::vertex_point_t>::type Vpm;

void ReadMesh(Mesh& mesh, std::string offFilename)
{
    std::ifstream input(offFilename);
    if (!input || !(input >> mesh) || !CGAL::is_triangle_mesh(mesh)) {
        std::cerr << "Not a valid input file" << std::endl;
        return;
    }
}

void callback(const Box* a, const Box* b) {
       // EMPTY
}

int main(int argc, char* argv[])
{
    Mesh mesh1, mesh2;
    std::string file1 = "E:/stl/rabit.off";
    std::string file2 = "E:/stl/sphere.off";

    ReadMesh(mesh1, file1);
    ReadMesh(mesh2, file2);

    Vpm vpm1 = get_property_map(boost::vertex_point, mesh1);
    Vpm vpm2 = get_property_map(boost::vertex_point, mesh2);

    std::vector<Box> boxes1, boxes2;
    std::vector<Box*> ptr1, ptr2;
    auto f_range = mesh1.faces();
    for (auto f_iter = f_range.begin(); f_iter != f_range.end(); ++f_iter)
    {
        auto h = mesh1.halfedge(*f_iter);
        boxes1.push_back(Box(
            get(vpm1, source(h, mesh1)).bbox() +
            get(vpm1, target(h, mesh1)).bbox() +
            get(vpm1, target(next(h, mesh1), mesh1)).bbox(),
            h));
        ptr1.push_back(&boxes1.back());
    }

    f_range = mesh2.faces();
    for (auto f_iter = f_range.begin(); f_iter != f_range.end(); ++f_iter)
    {
        auto h = mesh2.halfedge(*f_iter);
        boxes2.push_back(Box(
            get(vpm2, source(h, mesh2)).bbox() +
            get(vpm2, target(h, mesh2)).bbox() +
            get(vpm2, target(next(h, mesh2), mesh2)).bbox(),
            h));
        ptr2.push_back(&boxes2.back());
    }

    // Run the self intersection algorithm with all defaults on the
    // indirect pointers to bounding boxes. Avoids copying the boxes.
    auto t1 = std::chrono::steady_clock::now();
    CGAL::box_intersection_d(ptr1.begin(), ptr1.end(), ptr2.begin(), ptr2.end(), callback);
    auto t2 = std::chrono::steady_clock::now();
    std::cout << "Intersection test cost: " << std::chrono::duration<double, std::milli>(t2 - t1).count() << " ms" << std::endl;
    return 0;
}