#include <osg/ClipNode>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

#include "../../common/const_val.h"

int main()
{
    osg::ref_ptr<osg::Node> scene = osgDB::readNodeFile(datapath + "cessna.osg");

    float z = -10.0f;
    osg::ref_ptr<osg::MatrixTransform> reverse = new osg::MatrixTransform;
    reverse->preMult(osg::Matrix::translate(0.0f,0.0f,-z) * 
                     osg::Matrix::scale(1.0f,1.0f,-1.0f) * 
                     osg::Matrix::translate(0.0f,0.0f,z));
    reverse->addChild(scene.get());

    osg::ref_ptr<osg::ClipPlane> clipPlane = new osg::ClipPlane;
    clipPlane->setClipPlane(0.0, 0.0, -1.0, z);
    clipPlane->setClipPlaneNum(0);

    osg::ref_ptr<osg::ClipNode> clipNode = new osg::ClipNode;
    clipNode->addClipPlane(clipPlane.get());
    clipNode->addChild(reverse.get());

    // Why need clip node here??? this will be helping when dealing with water sim
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(scene.get());
    root->addChild(clipNode.get());

    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());
    viewer.setUpViewInWindow(0, 0, 800, 600);
    return viewer.run();
}