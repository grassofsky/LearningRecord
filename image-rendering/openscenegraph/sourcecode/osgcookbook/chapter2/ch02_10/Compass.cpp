#include "Compass.h"

Compass::Compass()
{

}

Compass::Compass(const Compass& copy, osg::CopyOp copyop)
: osg::Camera(copy, copyop)
, _plateTransform(copy._plateTransform)
, _needleTransform(copy._needleTransform)
, _mainCamera(copy._mainCamera)
{

}

Compass::~Compass()
{

}

void Compass::traverse(osg::NodeVisitor& nv)
{
    if (_mainCamera.valid() && nv.getVisitorType() == osg::NodeVisitor::CULL_VISITOR)
    {
        osg::Matrix matrix = _mainCamera->getViewMatrix();
        matrix.setTrans(osg::Vec3());

        osg::Vec3 northVec = osg::Z_AXIS * matrix;
        northVec.z() = 0.0f;
        northVec.normalize();
        
                
    }
}