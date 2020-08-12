#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Geometry>

#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/IO/MeshIO.hh"

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

int main()
{
    MyMesh mesh;
    std::string filename = "E:/stl/pelvis_mesh_model.stl";
    OpenMesh::IO::read_mesh(mesh, filename);
    std::vector<MyMesh::Point> vecVertices;
    std::vector<MyMesh::Point> vecNormals;
    mesh.request_vertex_normals();
    mesh.update_normals();

    for (auto face : mesh.faces())
    {
        for (auto vertex : mesh.fv_range(face))
        {
            auto point = mesh.point(vertex);
            auto normal = mesh.normal(vertex);

            vecVertices.push_back(point);
            vecNormals.push_back(normal);
        }
    }

    mesh.release_vertex_normals();

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> verticeArray = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> normalArray = new osg::Vec3Array();
    for (auto& vertex : vecVertices)
    {
        verticeArray->push_back(osg::Vec3(vertex[0], vertex[1], vertex[2]));
    }

    for (auto& normal : vecNormals)
    {
        normalArray->push_back(osg::Vec3(normal[0], normal[1], normal[2]));
    }
    
    geom->setVertexArray(verticeArray.get());
    geom->setNormalArray(normalArray.get());
    geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    geom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::TRIANGLES, 0, vecVertices.size()));

    osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
    color->push_back(osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
    geom->setColorArray(color.get());
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);

    geode->addDrawable(geom);

    osgViewer::Viewer viewer;
    viewer.setSceneData(geode.get());
    viewer.setUpViewInWindow(100, 100, 800, 600);

    return viewer.run();
}