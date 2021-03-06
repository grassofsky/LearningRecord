#include <osgViewer/Viewer>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/PolygonOffset>
#include <osgGA/StateSetManipulator>

#include <random>

#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"

#include "../../common/const_var.h"

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;

const int kiWidth = 800, kiHeight = 600;

template<typename Mesh>
class LaplaceSmootherT
{
public:
    typedef typename Mesh::Scalar Scalar;
    typedef typename Mesh::Point  Point;
    typedef typename Mesh::Normal NormalType;
    typedef typename Mesh::VertexHandle VertexHandle;
    typedef typename Mesh::EdgeHandle EdgeHandle;

public:
    LaplaceSmootherT(Mesh& _mesh)
        : mesh_(_mesh)
    {
        // request properties
        mesh_.request_vertex_status();
        mesh_.request_face_normals();
        mesh_.request_vertex_normals();
        
        // custom properties
        mesh_.add_property(original_position_);
        mesh_.add_property(original_normals_);
        mesh_.add_property(new_positions_);
        mesh_.add_property(is_active_);
        mesh_.add_property(vertex_weights_);
        mesh_.add_property(edge_weights_);
    }

    ~LaplaceSmootherT()
    {
        // free properties
        mesh_.release_vertex_status();
        mesh_.release_face_normals();
        mesh_.release_vertex_normals();

        // free custom properties
        mesh_.remove_property(original_position_);
        mesh_.remove_property(original_normals_);
        mesh_.remove_property(new_positions_);
        mesh_.remove_property(is_active_);
        mesh_.remove_property(vertex_weights_);
        mesh_.remove_property(edge_weights_);
    }

    void initialize()
    {
        typename Mesh::VertexIter v_it, v_end(mesh_.vertices_end());

        // update normals
        mesh_.update_face_normals();
        mesh_.update_vertex_normals();

        // store original points & normals
        for (v_it = mesh_.vertices_begin(); v_it != v_end; ++v_it)
        {
            mesh_.property(original_position_, *v_it) = mesh_.point(*v_it);
            mesh_.property(original_normals_, *v_it) = mesh_.normal(*v_it);
        }

        // calculate weights
        compute_weights();
    }

    void smooth(unsigned int _n)
    {
        // mark active vertices
        set_active_vertices();

        // smooth _n iterations
        while (_n--)
        {
            compute_new_positions();
            move_points();
        }
    }

    void set_active_vertices()
    {
        typename Mesh::VertexIter v_it, v_end(mesh_.vertices_end());

        // tag all active vertices
        for (v_it = mesh_.vertices_begin(); v_it != v_end; ++v_it)
        {
            bool active = !mesh_.is_boundary(*v_it) && !mesh_.status(*v_it).locked();
            mesh_.property(is_active_, *v_it) = active;
        }
    }

    void compute_new_positions()
    {
        typename Mesh::VertexIter v_it, v_end(mesh_.vertices_end());
        typename Mesh::ConstVertexOHalfedgeIter voh_it;
        typename Mesh::Normal u, p, zero(0, 0, 0);
        typename Mesh::Scalar w;

        for (v_it = mesh_.vertices_begin(); v_it != v_end; ++v_it)
        {
            if (this->is_active(*v_it))
            {
                // compute umbrella
                u = zero;
                for (voh_it = mesh_.cvoh_iter(*v_it); voh_it.is_valid(); ++voh_it)
                {
                    w = this->weight(mesh_.edge_handle(*voh_it));
                    u += OpenMesh::vector_cast<typename Mesh::Normal>(mesh_.point(mesh_.to_vertex_handle(*voh_it))) * w;
                }
                u *= this->weight(*v_it);
                u -= OpenMesh::vector_cast<typename Mesh::Normal>(mesh_.point(*v_it));

                // damping
                u *= static_cast<typename Mesh::Scalar>(0.5);

                // store new position
                p = OpenMesh::vector_cast<typename Mesh::Normal>(mesh_.point(*v_it));
                p += u;
                this->set_new_position(*v_it, p);
            }
        }
    }

    void move_points()
    {
        typename Mesh::VertexIter v_it(mesh_.vertices_begin()), v_end(mesh_.vertices_end());

        for (; v_it != v_end; ++v_it)
        {
            if (is_active(*v_it))
            {
                mesh_.set_point(*v_it, new_position(*v_it));
            }
        }
    }

    const Point& new_position(VertexHandle _vh) const
    {
        return mesh_.property(new_positions_, _vh);
    }

    void set_new_position(VertexHandle _vh, const Point& _p)
    {
        mesh_.property(new_positions_, _vh) = _p;
    }

    bool is_active(VertexHandle _vh) const
    {
        return mesh_.property(is_active_, _vh);
    }

    Scalar weight(VertexHandle _vh) const
    {
        return mesh_.property(vertex_weights_, _vh);
    }

    Scalar weight(EdgeHandle _eh) const
    {
        return mesh_.property(edge_weights_, _eh);
    }

    void compute_weights()
    {
        // Cotangent Laplacian
        typename Mesh::VertexIter v_it, v_end(mesh_.vertices_end());
        typename Mesh::EdgeIter e_it, e_end(mesh_.edges_end());
        typename Mesh::HalfedgeHandle heh0, heh1, heh2;
        typename Mesh::VertexHandle v0, v1;
        typename Mesh::Normal d0, d1;
        typename Mesh::Scalar weight, lb(-1.0), ub(1.0);

        // init vertex weights
        for (v_it = mesh_.vertices_begin(); v_it != v_end; ++v_it)
        {
            mesh_.property(vertex_weights_, *v_it) = 0.0;
        }

        for (e_it = mesh_.edges_begin(); e_it != e_end; ++e_it)
        {
            const typename Mesh::Point *p0, *p1, *p2;

            weight = 0.0;

            heh0 = mesh_.halfedge_handle(*e_it, 0);
            v0 = mesh_.to_vertex_handle(heh0);
            p0 = &mesh_.point(v0);

            heh1 = mesh_.halfedge_handle(*e_it, 1);
            v1 = mesh_.to_vertex_handle(heh1);
            p1 = &mesh_.point(v1);

            heh2 = mesh_.next_halfedge_handle(heh0);
            p2 = &mesh_.point(mesh_.to_vertex_handle(heh2));
            d0 = (*p0 - *p2); d0.normalize();
            d1 = (*p1 - *p2); d1.normalize();
            weight += static_cast<typename Mesh::Scalar>(1.0) / tan(acos(std::max(lb, std::min(ub, dot(d0, d1)))));

            heh2 = mesh_.next_halfedge_handle(heh1);
            p2 = &mesh_.point(mesh_.to_vertex_handle(heh2));
            d0 = (*p0 - *p2); d0.normalize();
            d1 = (*p1 - *p2); d1.normalize();
            weight += static_cast<typename Mesh::Scalar>(1.0) / tan(acos(std::max(lb, std::min(ub, dot(d0, d1)))));

            mesh_.property(edge_weights_, *e_it) = weight;
            mesh_.property(vertex_weights_, v0) += weight;
            mesh_.property(vertex_weights_, v1) += weight;
        }

        // invert vertex weights:
        // before: sum of edge weights
        // after: one over sum of edge weights 
        for (v_it = mesh_.vertices_begin(); v_it != v_end; ++v_it)
        {
            weight = mesh_.property(vertex_weights_, *v_it);
            if (weight)
            {
                mesh_.property(vertex_weights_, *v_it) = static_cast<typename Mesh::Scalar>(1.0) / weight;
            }
        }
    }

private:
    Mesh& mesh_;

    OpenMesh::VPropHandleT<Point> original_position_;
    OpenMesh::VPropHandleT<NormalType> original_normals_;
    OpenMesh::VPropHandleT<Point> new_positions_;
    OpenMesh::VPropHandleT<bool> is_active_;
    OpenMesh::VPropHandleT<Scalar> vertex_weights_;
    OpenMesh::EPropHandleT<Scalar> edge_weights_;
};

osg::ref_ptr<osg::Group> generateGroupNode(LaplaceSmootherT<MyMesh>& smoother, MyMesh& mesh);
void addnoise(MyMesh& mesh);
std::vector<osg::Camera*> getCameras(int nCamera);

int main()
{
    MyMesh mesh;
    std::string filename = data_path + "sphere.stl";
    OpenMesh::IO::read_mesh(mesh, filename);
    addnoise(mesh);

    LaplaceSmootherT<MyMesh> smoother(mesh);
    smoother.initialize();

    std::vector<osg::Camera*> cameras = getCameras(2);
    if (cameras.empty())
    {
        return -1;
    }
    osgViewer::Viewer viewer;
    osg::ref_ptr<osg::Group> group1 = generateGroupNode(smoother, mesh);
    cameras[0]->addChild(group1);
    viewer.addSlave(cameras[0], osg::Matrixd(), osg::Matrixd::scale(1.0, 1.0, 1.0), false);

    // smoothing
    smoother.smooth(3);
    osg::ref_ptr<osg::Group> group2 = generateGroupNode(smoother, mesh);
    cameras[1]->addChild(group2);
    viewer.addSlave(cameras[1], osg::Matrixd(), osg::Matrixd::scale(1.0, 1.0, 1.0), false);

    viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));

    viewer.setSceneData(group1);
    return viewer.run();
}

std::vector<osg::Camera*> getCameras(int nCamera)
{
    std::vector<osg::Camera*> vecCameras;

    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = 0;
    traits->y = 0;
    traits->width = kiWidth;
    traits->height = kiHeight;
    traits->windowDecoration = true;
    traits->doubleBuffer = true;
    traits->sharedContext = 0;

    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
    if (gc.valid())
    {
        gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.2f, 1.0f));
        gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
    }

    for (int i = 0; i < nCamera; ++i)
    {
        osg::ref_ptr<osg::Camera> camera = new osg::Camera;
        camera->setGraphicsContext(gc.get());
        camera->setViewport(new osg::Viewport((i * kiWidth) / nCamera, 0, kiWidth / nCamera, kiHeight));
        camera->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(kiWidth) / static_cast<double>(kiHeight), 1.0f, 10000.0f);
        GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
        camera->setDrawBuffer(buffer);
        camera->setReadBuffer(buffer);
        vecCameras.push_back(camera.release());
    }

    return vecCameras;
}

void addnoise(MyMesh& mesh)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> u(0, 2);

    mesh.request_face_normals();
    mesh.request_vertex_normals();
    mesh.update_face_normals();
    mesh.update_normals();

    MyMesh::VertexIter v_it, v_end(mesh.vertices_end());
    for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
    {
        auto normal = mesh.normal(*v_it);
        double noise = u(mt);
        auto position = mesh.point(*v_it);
        auto new_position = position + noise * normal;
        mesh.set_point(*v_it, new_position);
    }
}

// calculate mean curvature https://blog.csdn.net/qq_38517015/article/details/105185241
double getBarycentricNeighborArea(MyMesh& mesh, MyMesh::VertexIter v_it)
{
    double area = 0.0;
    for (auto vf_it = mesh.vf_iter(*v_it); vf_it.is_valid(); ++vf_it)
    {
        std::vector<MyMesh::Point> vecPts;;
        for (auto fv_it = mesh.fv_iter(*vf_it); fv_it.is_valid(); ++fv_it)
        {
            vecPts.push_back(mesh.point(*fv_it));
        }

        assert(vecPts.size() == 3);
        auto a = OpenMesh::vector_cast<OpenMesh::Vec3d>(vecPts[0] - vecPts[1]).length();
        auto b = OpenMesh::vector_cast<OpenMesh::Vec3d>(vecPts[2] - vecPts[1]).length();
        auto c = OpenMesh::vector_cast<OpenMesh::Vec3d>(vecPts[0] - vecPts[2]).length();
        auto p = (a + b + c) / 2.0;
        area += std::sqrt(p * (p - a) * (p - b) * (p - c));
    }

    return area / 3.0;
}

void colorMap(double curvature, osg::Vec4& color)
{
    color[3] = 1.0;

    const double rone = 0.8;
    const double gone = 1.0;
    const double bone = 1.0;

    double x = curvature;
    x = (x < 0 ? 0 : (x > 1 ? 1 : x));
    if (x < 1. / 8.)
    {
        color[0] = 0;
        color[1] = 0;
        color[2] = bone * (0.5 + x / (1. / 8.) * 0.5); 
    }
    else if (x < 3. / 8.)
    {
        color[0] = 0;
        color[1] = gone * (x - 1. / 8.) / (3. / 8. - 1. / 8.);
        color[2] = bone;
    }
    else if (x < 5. / 8.)
    {
        color[0] = rone * (x - 3. / 8.) / (5. / 8. - 3. / 8.);
        color[1] = gone;
        color[2] = bone - (x - 3. / 8.) / (5. / 8. - 3. / 8.);
    }
    else if (x < 7. / 8.)
    {
        color[0] = rone;
        color[1] = gone - (x - 5. / 8.) / (7. / 8. - 5. / 8.);
        color[2] = 0;
    }
    else
    {
        color[0] = rone - (x - 7. / 8.) / (1. - 7. / 8.) * 0.5;
        color[1] = 0;
        color[2] = 0;
    }
}

osg::ref_ptr<osg::Group> generateGroupNode(LaplaceSmootherT<MyMesh>& smoother, MyMesh& mesh)
{
    std::vector<MyMesh::Point> vecVertices;
    std::vector<MyMesh::Point> vecNormals;
    std::vector<double> vecCurvature;
    mesh.request_face_normals();
    mesh.request_vertex_normals();
    mesh.update_face_normals();
    mesh.update_normals();

    OpenMesh::VPropHandleT<double> mean_curvature;
    mesh.add_property(mean_curvature);
    MyMesh::VertexIter v_it, v_end(mesh.vertices_end());
    MyMesh::ConstVertexOHalfedgeIter voh_it;
    MyMesh::Scalar w;
    for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it)
    {
        double localArea = getBarycentricNeighborArea(mesh, v_it);

        MyMesh::Scalar k = 0;
        for (voh_it = mesh.cvoh_iter(*v_it); voh_it.is_valid(); ++voh_it)
        {
            w = smoother.weight(mesh.edge_handle(*voh_it));
            k += w * OpenMesh::vector_cast<OpenMesh::Vec3d>(mesh.point(*v_it) - mesh.point(mesh.to_vertex_handle(*voh_it))).length();
        }
        k = 0.5 * k / localArea;
        mesh.property(mean_curvature, *v_it) = k;
    }

    for (auto face : mesh.faces())
    {
        for (auto vertex : mesh.fv_range(face))
        {
            auto point = mesh.point(vertex);
            auto normal = mesh.normal(vertex);
            auto curvature = mesh.property(mean_curvature, vertex);

            vecVertices.push_back(point);
            vecNormals.push_back(normal);
            vecCurvature.push_back(curvature);
        }
    }

    mesh.remove_property(mean_curvature);

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

    osg::ref_ptr<osg::Vec3Array> verticeArray = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec3Array> normalArray = new osg::Vec3Array();
    osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array();
    for (auto& vertex : vecVertices)
    {
        verticeArray->push_back(osg::Vec3(vertex[0], vertex[1], vertex[2]));
    }

    for (auto& normal : vecNormals)
    {
        normalArray->push_back(osg::Vec3(normal[0], normal[1], normal[2]));
    }
    // normalize curvature
    double minCurvature(1), maxCurvature(6);
    for (auto& curvature : vecCurvature)
    {
        osg::Vec4 color;
        colorMap((curvature - minCurvature)/(maxCurvature - minCurvature), color);
        colorArray->push_back(color);
    }

    geom->setVertexArray(verticeArray.get());
    geom->setNormalArray(normalArray.get());
    geom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    geom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::TRIANGLES, 0, vecVertices.size()));

    geom->setColorArray(colorArray.get());
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    geode->addDrawable(geom);

    osg::ref_ptr<osg::Group> group = new osg::Group;
    group->addChild(geode);

    return group;
}

