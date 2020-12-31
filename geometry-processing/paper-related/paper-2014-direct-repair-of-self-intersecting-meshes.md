# PaperRead - Direct repair of self-intersecting meshes

>Marco Attene,
>Direct repair of self-intersecting meshes,
>Graphical Models,
>Volume 76, Issue 6,
>2014,
>Pages 658-668,
>ISSN 1524-0703,
>https://doi.org/10.1016/j.gmod.2014.09.002.
>(http://www.sciencedirect.com/science/article/pii/S1524070314000496)

每种修复算法对输入都有自己的要求，而这些要求在实际应用中并不总是满足的。(M. Attene, M. Campen, L. Kobbelt, Polygon mesh repairing: an application perspective, ACM Comput. Surv. 45 (2) (2013).)

作为支持输入限制的一个例子，可以考虑许多算法假设输入具有流形连通性并且没有表面孔。在大多数情况下，这些相同的算法只能产生流形结果，这意味着不能构造两个流形实体的非流形并集。 **本文提出了一种快速、精确的从任意三角形网格中消除交集的算法，对输入没有任何严格的要求**。 

## 2. State of the art

 自相交修复技术的研究现状可分为fragile algorithms, approximate methods,  Robust and exact methods。 

**2.1 Fragile algorithms**

处理每种情况，为了获取精确解，容易出现无法处理的情况，需要undo，然后重新尝试，e.g. Autodesk Maya

**2.2 Approximate methods**

无法生成确切的结果。

**2.3 Robust and exact methods**

GNU Multi Precision arithmetic library用于浮点数准确判断，CGAL中使用了该库进行exact计算。

## 3. Algorithm

### 3.2 Overview

整个过程可以分为：预处理和相交去除。输入使用的是非流形网格数据结构。

```
Require: A pure 2D abstract simplicial complex M embeded in R^3
Ensure: The outer hull H of |M|
  1: Encode M into a nonmanifold structure H
  2: Remove degenerate triangles from H
  3: Compute H's intersection segments and points
  4: Subdivide each triangle in H along its intersection elements (if any)
  5: Unify coincident edges and vertices into nonmanifold elements
  6: Mark triangles in H belong to the most external surface
  7: Remove unmarked triangles
```

### 3.3. Computing the intersection elements

使用了 an axis-aligned BSP tree。当节点中的三角形相交数量大于一个阈值的时候进行划分（如：50）。在每个叶子节点中的三角形，进行all-with-all相交测试。

基于：*P. Volino, N. Magnenat-Thalmann, Effificient self-collision detection on smoothly discretized surface animations using geometrical shape regularity, Comput. Graph. Forum 13 (3) (1994) 155–166.*，如果满足以下条件，那么cell没有相交元素：

- the union of all the triangles in T forms a single combinatorial disk D;
- the dot-product of all the normals of triangles in T with one reference vector N is positive;
- the projection of D on a plane whose directional vector is N has a non self-intersecting boundary.

其中T是cell中的三角形集合。N是从与axis平行的六个向量中选择一个与T的平均法向量夹角最大的一个。

任何相交的顶点，属于以下三种情况中的一种：

1. p属于顶点位置（`L(p) = 0`)
   - 边的内部，或三角面的内部，碰到了一个顶点（如，金字塔的顶点，顶着一个三角面）；
2. p属于open edge（`L(p) = 1`）
   - 共面的两条边相交，并且交点不是任一条边的端点；
   - 边穿过三角面的交点，并且交点不是三角形的顶点，也不是边的端点；
3. p属于open triangle（`L(p) = 2`）
   -  三个非共面开放三角形共用一个不在任何三角形边界上的点 ；？？？

- 



