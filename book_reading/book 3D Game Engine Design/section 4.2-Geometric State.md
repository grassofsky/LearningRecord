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

包围体的一个作用是用于对象剔除。

#### Collision Determination

#### The Abstract bounding volume interface



### 4.2.4 Geometric types



