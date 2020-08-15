#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgUtil/CullVisitor>
#include <osgViewer/Viewer>

#include "../../common/const_val.h"

class BillboardCallback : public osg::NodeCallback
{
public:
    BillboardCallback(osg::MatrixTransform* billboard)
    : _billboardNode(billboard) {}

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(nv);
        if (_billboardNode.valid() && cv)
        {
            osg::Vec3d translation, scale;
            osg::Quat rotation, so;
            cv->getModelViewMatrix()->decompose(translation, rotation, scale, so);

            osg::Matrixd matrix(rotation.inverse());
            _billboardNode->setMatrix(matrix);
        }
        traverse(node, nv);
    }

protected:
    osg::observer_ptr<osg::MatrixTransform> _billboardNode;
};

int main()
{
    osg::ref_ptr<osg::MatrixTransform> billboardNode = new osg::MatrixTransform;
    billboardNode->addChild(osgDB::readNodeFile(datapath + "cessna.osg"));

    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(billboardNode.get());
    root->addChild(osgDB::readNodeFile(datapath+"lz.osg"));
    root->addCullCallback(new BillboardCallback(billboardNode.get()));

    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());
    viewer.setUpViewInWindow(0, 0, 800, 600);
    return viewer.run();
}