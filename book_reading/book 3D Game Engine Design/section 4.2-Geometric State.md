# Chapter 4 Scene Graphs

## 4.2 Geometric state

基本对象都会有的geometric state有：vertex buffers，index buffers，transformations，和bounding volumes。

### 4.2.1 Vertex Buffers and Index Buffers

顶点位置和顶点对应的属性，如normal vectors，colors，textures coordinate都存储在vertex buffer中。连接拓扑关系存储在index buffer。几何元素表示了点的集合，线段的集合，三角形网格等。

### 4.2.2 Transformations

主要考虑到了计算性能。

TODO

### 4.2.3 Bounding Volumes

最常用的包围体是sphere，或者是axis-aligned bounding box。稍微复杂点的就是oriented bounding box。更加复杂点的是凸包，或凸多面体。所有的包围体都是凸的。一个包围体，也许可以是多个包围体的并集。

#### Culling

包围体的一个作用是用于对象剔除。对于包围体而言，如果对于六个frustum平面，包围体出现在某个frustum平面的负方向（正方向指向view frustum 内部），那么这个包围体就位于frustum外面。伪代码如下：

```c++
bool IsCulled(ViewFrustum frustum, BoudingVolume bound)
{
    for each plane of frustum do
    {
        if bound is on the negative side of plane then
            return true;
    }
    return false; // 会存在位于ViewFrustum外部，而返回false的情况，但不会经常碰到这种情况
}
```

#### Collision Determination

包围体的另一个主要用途是3D picking。如果hit ray没有和包围体相交，那么就不会和包围体内的物体相交。具体会在8.4节介绍。

#### The Abstract bounding volume interface

`BoundingVolume`抽象类如下：

```c++
class BoundingVolume : public Object
{
public:
    virtual ~BoundingVolume();
    virtual void SetCenter(const Vector3f& rkCenter) = 0;
    virtual void SetRadius(float fRadius) = 0;
    virtual Vector3f GetCenter() const = 0;
    virtual float GetRadius() const = 0;
    
    virtual void ComputeFromData(const Vector3fArray* pkVertices) = 0;
    virtual void TransformBy(const Transformation& rkTransform, BoundingVolume* pkResult) = 0;
    virtual int WhichSide(const Plane3f& rkPlane) const = 0;
    virtual bool TestIntersection(const Vector3f& rkOrigin, const Vector3f& rkDirection) const = 0;
    virtual bool TestIntersection(const BoundingVolume* pkInput) const = 0;
    
    virtual void CopyFrom(const BoundingVolume* pkInput) = 0;
    virtual void GrowToContain(const BoundingVolume* pkInput) = 0;
    virtual bool Contains(const Vector3f& rkPoint) const = 0;
    
    static BoundingVolume* Create();
protected:
    BoundingVolume();
};
```

### 4.2.4 Geometric types

支持的基本几何类型有：collections of points，collections of line segments, triangle meshes, and particles.

TODO



