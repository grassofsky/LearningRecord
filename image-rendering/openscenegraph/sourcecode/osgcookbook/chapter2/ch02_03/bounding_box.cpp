#include <osg/ComputeBoundsVisitor>
#include <osg/ShapeDrawable>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osg/PolygonMode>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

#include "../../common/CommonFunctions.h"
#include "../../common/const_val.h"

class BoundingBoxCallback : public osg::NodeCallback
{
public:
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::BoundingBox bb;
        for (unsigned int i=0; i<_nodesToCompute.size(); ++i)
        {
            osg::Node* tmpNode = _nodesToCompute[i];
            osg::ComputeBoundsVisitor cbbv;
            tmpNode->accept(cbbv);

            osg::BoundingBox localBB = cbbv.getBoundingBox();
            osg::Matrix localToWorld = osg::computeLocalToWorld(tmpNode->getParent(0)->getParentalNodePaths()[0]);
            for (unsigned int i=0; i<8; ++i)
            {
                bb.expandBy(localBB.corner(i) * localToWorld);
            }
        }

        osg::MatrixTransform* trans = static_cast<osg::MatrixTransform*>(node);
        trans->setMatrix(
            osg::Matrix::scale(bb.xMax() - bb.xMin(), bb.yMax() - bb.yMin(), bb.zMax() - bb.zMin()) * 
            osg::Matrix::translate(bb.center())
        );

        traverse(node, nv);
    }

    osg::NodePath _nodesToCompute;
};

int main(int argc, char** argv)
{
    osg::ref_ptr<osg::MatrixTransform> cessna = new osg::MatrixTransform();
    cessna->addChild(osgDB::readNodeFile(datapath + "cessna.osg"));

    osg::ref_ptr<osg::AnimationPathCallback> apcb = osgCookBook::createAnimationPathCallback(50.0f, 6.0f);
    cessna->addUpdateCallback(apcb);

    osg::ref_ptr<osg::MatrixTransform> dumptruck = new osg::MatrixTransform();
    dumptruck->addChild(osgDB::readNodeFile(datapath + "dumptruck.osg"));
    dumptruck->setMatrix(osg::Matrix::translate(0.0f, 0.0f, -100.0f));

    osg::ref_ptr<osg::MatrixTransform> models = new osg::MatrixTransform();
    models->addChild(cessna.get());
    models->addChild(dumptruck.get());
    models->setMatrix(osg::Matrix::translate(0.0f, 0.0f, 200.0f));

    osg::ref_ptr<BoundingBoxCallback> bbcb = new BoundingBoxCallback();
    bbcb->_nodesToCompute.push_back(cessna.get());
    bbcb->_nodesToCompute.push_back(dumptruck.get());

    osg::ref_ptr<osg::Geode> geode = new osg::Geode();
    geode->addDrawable(new osg::ShapeDrawable(new osg::Box));

    osg::ref_ptr<osg::MatrixTransform> boundingBoxNode = new osg::MatrixTransform();
    boundingBoxNode->addChild(geode.get());
    boundingBoxNode->setUpdateCallback(bbcb.get());
    boundingBoxNode->getOrCreateStateSet()->setAttributeAndModes(
        new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
    boundingBoxNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(models.get());
    root->addChild(osgDB::readNodeFile(datapath + "lz.osg"));
    root->addChild(boundingBoxNode.get());

    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());
    viewer.setUpViewInWindow(0, 0, 800, 600);
    return viewer.run();
}