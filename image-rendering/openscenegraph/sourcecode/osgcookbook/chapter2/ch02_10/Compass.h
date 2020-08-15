#pragma once

#include <osg/MatrixTransform>
#include <osg/Camera>

class Compass : public osg::Camera
{
public:
    Compass();
    Compass(const Compass& copy, osg::CopyOp copyop=osg::CopyOp::SHALLOW_COPY);
    META_Node(osg, Compass);

    void setPlate(osg::MatrixTransform* plate) { _plateTransform = plate; }
    osg::MatrixTransform* getPlate() { return _plateTransform; }
    const osg::MatrixTransform* getPlate() const { return _plateTransform; }

    void setNeedle(osg::MatrixTransform* needle) { _needleTransform = needle; }
    osg::MatrixTransform* getNeedle() { return _needleTransform; }
    const osg::MatrixTransform* getNeedle() const { return _needleTransform; }

    void setMainCamera( osg::Camera* camera ) { _mainCamera = camera; }
    osg::Camera* getMainCamera() { return _mainCamera.get(); }
    const osg::Camera* getMainCamera() const { return _mainCamera.get(); }
    
    virtual void traverse( osg::NodeVisitor& nv );

protected:
    virtual ~Compass();
    
    osg::ref_ptr<osg::MatrixTransform> _plateTransform;
    osg::ref_ptr<osg::MatrixTransform> _needleTransform;
    osg::observer_ptr<osg::Camera> _mainCamera;
};