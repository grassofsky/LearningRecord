#include <osg/AnimationPath>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/LineWidth>
#include <osgUtil/Tessellator>

#include <osg/Vec3>
#include <osg/Quat>
#include <osgViewer/Viewer>
#include <cmath>

osg::AnimationPathCallback* createAnimationPathCallback(float radius, float time)
{
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
    path->setLoopMode(osg::AnimationPath::LOOP);

    unsigned int numSamples = 32;
    float delta_yaw = 2.0f * osg::PI / ((float)numSamples - 1.0f);
    float delta_time = time / (float)numSamples;
    for (unsigned int i = 0; i < numSamples; ++i)
    {
        float yaw = delta_yaw * i;
        osg::Vec3 pos(sinf(yaw) * radius, cosf(yaw) * radius, 0.0f);
        osg::Quat rot(-yaw, osg::Z_AXIS);
        path->insert(delta_time * (float)i, osg::AnimationPath::ControlPoint(pos, rot));
    }

    osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
    apcb->setAnimationPath(path.get());
    return apcb.release();
}

int main()
{
    // concave polygon
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(8);
    (*vertices)[0].set(0.0f, 0.0f, 0.0f);
    (*vertices)[1].set(3.0f, 0.0f, 0.0f);
    (*vertices)[2].set(3.0f, 0.0f, 3.0f);
    (*vertices)[3].set(0.0f, 0.0f, 3.0f);
    (*vertices)[4].set(1.0f, 0.0f, 1.0f);
    (*vertices)[5].set(2.0f, 0.0f, 1.0f);
    (*vertices)[6].set(2.0f, 0.0f, 2.0f);
    (*vertices)[7].set(1.0f, 0.0f, 2.0f);

    // normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
    (*normals)[0].set(0.0f, -1.0f, 0.0f);

    osg::ref_ptr<osg::Geometry> polygon = new osg::Geometry;
    polygon->setVertexArray(vertices.get());
    polygon->setNormalArray(normals.get());
    polygon->setNormalBinding(osg::Geometry::BIND_OVERALL);
    polygon->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));
    polygon->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 4, 4));

    osgUtil::Tessellator tessellator;
    tessellator.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
    tessellator.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
    tessellator.retessellatePolygons(*polygon);

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
    (*colors)[0].set(1.0f, 1.0f, 0.0f, 1.0f);

    osg::ref_ptr<osg::Geometry> border = new osg::Geometry;
    border->setVertexArray(vertices.get());
    border->setColorArray(colors.get());
    border->setColorBinding(osg::Geometry::BIND_OVERALL);
    border->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 0, 4));
    border->addPrimitiveSet(new osg::DrawArrays(GL_LINE_LOOP, 4, 4));
    border->getOrCreateStateSet()->setAttribute(new osg::LineWidth(5.0f));

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(polygon.get());
    geode->addDrawable(border.get());

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(geode.get());

    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());
    viewer.setUpViewInWindow(0, 0, 800, 600);
    return viewer.run();
}