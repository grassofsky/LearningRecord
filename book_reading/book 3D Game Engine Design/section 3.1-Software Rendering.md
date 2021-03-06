# Chapter 3 Renderers

## 3.1 Software Rendering

场景图管理系统的任务是在进行绘制时识别每个场景图中可能可见的对象集。接着就是将需要渲染的对象发送到renderer进行绘制。

**TODO：关于software rendering部分，需要结合代码进一步学习**

**TODO：整理rendering流程中计算误差引起的一些问题**

### 3.1.1  Vertex Shaders

三角形有三个顶点，每个顶点有位置和可选的顶点属性。绘制的第一个步骤，是对每个顶点，应用vertex shader。 这相当于调用一个顶点程序（vertex programming），其输入是顶点位置和其他属性。 顶点位置通常传进来的是模型坐标系，但是这个并不是必须的。随着vertex program传递进来的还有矩阵，用来对顶点位置，法向量等进行变换。该程序至少需要计算顶点在clip-space下的坐标，并作为结果输入到clipper，和rasterizer。示例程序如下：

```glsl
// GeometricTools\WildMagic4\Data\ShaderPrograms\Cg
void VertexColor3VProgram
(
    in float4 kModelPosition : POSITION,
    in float3 kModelColor : COLOR,
    out float4 kClipPosition : POSITION,
    out float4 kVertexColor : COLOR,
    uniform float4x4 WVPMatrix)
{
    // Transform the position from model space to clip space.
    kClipPosition = mul(kModelPosition, WVPMatrix);
    
    // Pass through the vertex color
    kVertexColor = kModelColor;
}
```

这个vertex program被`ApplyVertexShader`调用。在调用的时候，renderer已经知道了要处理的顶点，和每个vertex的大小

软件渲染器为输出提供存储，因此ApplyVertexShader增加当前输出数组的大小（如果需要）以存储从vertex程序传回的结果。调用vertex程序的实际代码行是：

`m_oVProgram(afRegister,afInVertex,afOutVertex);`

第一个参数是常量数组。在前面的示例中，这个数组存储矩阵Hwvp。第二个参数是顶点的输入数组（模型空间位置和属性）。第三个参数是顶点的输出数组（clip space位置和属性）。

接来下需要根据具体的绘制类型调用具体的绘制函数。对于三角形的绘制函数为`DrawTriMesh`。

### 3.1.2  Back-Face Culling

通常情况下选择背面剔除，可以跳过这个步骤，也可以采用正面剔除。

在三维坐标系（如，相机坐标系，或世界坐标系中）很容易判断三角形是不是可见的。假设在三维坐标系中，$\bold P_i, 0 \le i \le 2$，（此处假设顶点是逆时针排序的）眼睛的位置为$\bold E$，如果下面的公式的值大于0，那么三角形是可见的：
$$
\delta = (\bold E - \bold P_0)\cdot(\bold P_1 - \bold P_0)\times(\bold P_2 - \bold P_0) > 0
$$

在view coordinate中，眼睛的位置为原点。该点在clip space中，的位置为，$(0, 0, -d_{max}d_{min}/(d_{max} - d_{min})$，0)。clip space中，三角形的三个顶点为：$(r_i',u_i',d_i',w_i'),0\le i \le2$。clip space中的d值对于朝向没有什么作用（因为眼睛位置的近或远对于朝向确认没有作用），在软渲染的代码里面将d省略了，如下[**TODO,进一步理解**]：
$$
\bold E_1 = (r_1'-r_0', u_1'-u_0', w_1' - w_0'), \\
\bold E_2 = (r_2'-r_0', u_2'-u_0', w_2'-w_0') \\
\bold N = \bold E_1 \times \bold E_2, \\
\delta_1 = \sigma(\bold N \cdot (r_0', u_0', w_0'))
$$

$\sigma$基于cull state或（相机坐标系的handedness）确定的，用来确定符号的函数。如果$\delta_1 \le 0$，三角形是背面不可见的。用矩阵形式表示如下：[**TODO,进一步理解**]
$$
\begin{align}
\delta_1 &= \sigma \text{det}\begin{bmatrix}
r_0' & u_0' & w_0' \\
r_1' & u_1' & w_1' \\
r_2' & u_2' & w_2'
\end{bmatrix} \\
&=\sigma(w_0'(r_1'u_2' - r_2'u_1') - w_1'(r_0'u_2'-r_2'u_0') + w_2'(r_0'u_1' - r_1'u_0'))
\end{align}
$$
**[TODO, 另一种实现方式]**

### 3.1.3  Clipping

在Section 2.4.3中介绍了两种clipping算法，plane-at-a-time-clipping，和polygon-of-intersection。具体实现的函数如下：

```c++
void ClipPolygon(int& riQuantity, int aiIndex[SOFT_MAX_CLIP_INDICES], int aiEdge[SOFT_MAX_CLIP_INDICES]);
```

作为input参数，riQuantity为初始的多边形顶点数目，为3；aiIndex存储了vertex programs输出的顶点的indices。

作为output参数，riQuantity为被frustum planes裁切后的多边形的顶点数目，如果有新的顶点生成，那么它会被存储在output vertex array中。aiIndex表示多边形的顶点，aiEdge用来存储仅用于绘制wireframe的边。

上面的ClipPolygon函数是对下面的函数的wrapper：

```c++
void ClipPolygon(const Vector4& rkPlane, int& riQuantity, 
                 int aiIndex[SOFT_MAX_CLIP_INDICES],
                 int aiEdge[SOFT_MAX_CLIP_INDICES]);
```

rkPlane的参数$(n_0,n_1,n_2,-d)$，对应的平面的法向量是$\bold N = (n_0, n_1,n_2)$，对于clip space中的点$(r',u',d',w')$，如果满足如下条件的点位于frustum内部，（N方向是由frustum平面指向中心方向），
$$
(n_0, n_1,n_2,-d)\cdot(r',u',d',w') \ge 0
$$
该函数计算了，frustum内部的顶点数$p$，frustum外部的顶点数$n$，$k$为多边形的顶点数，$p + n \le k$，位于面上的点没有考虑。

如果p = 0，那么修整个多边形位于frustum外部，该polygon is culled，此时riQuantity返回的为0；

如果p>0, n=0，那么多边形位于frustum内部，不需要进行clipping；

如果p>0,n>0，多边形部分位于内部，部分位于外部，那么需要进行裁切，需要进一步被传递给函数`ClipPolygonAgainstPlane`。

### 3.1.4  Rasterizing

在绘制凸多边形的三角形扇形时，`DrawTriMesh`中部分代码如下：

```c++
int iNumTriangles = iQuantity - 2;
for (int j=1; j<=iNumTriangles; ++j)
{
    RasterizeTriangle(aiIndex[0],aiIndex[j],aiIndex[j+1]);
}
```

rasterizeTriangle函数接收，三个clip space坐标系的点，然后把他们映射到window space中，通过ClipToWindow函数。返回fX和fY，fDepth，depth的范围为[0,1]，fInverseW，是裁剪坐标系中w的倒数。最初不需要深度和逆w值；浮点窗口坐标被截断为整数值，并用于开始边缘缓冲区构造。 

后向三角形的光栅化器测试是一个精确的测试，因此光栅化的任何三角形都保证是前向的。 其基本思想是对连接顶点的边进行光栅化，将通过对边的顶点端点进行透视插值得到的属性赋给每个边像素。

### 3.1.5  Edge Buffers

Edge buffer算法设计成识别每个扫描线上包含三角形所覆盖像素的起始和结束的x值。

**TODO**

### 3.1.6  Scan Line Processing

### 3.1.7  Pixel Shaders

### 3.1.8  Stencil Buffering

### 3.1.9  Depth Buffering

### 3.1.10 Alpha Blending

### 3.1.11 Color Masking

### 3.1.12 Texture Sampling

### 3.1.13 Frame Buffers