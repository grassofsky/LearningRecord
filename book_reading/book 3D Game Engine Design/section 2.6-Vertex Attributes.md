# Chapter 2 The Fundation

## 2.6 Vertex Attributes

### 2.6.1 Colors

### 2.6.2 Lighting and materials

在实时渲染引擎中，标准的光源类型包括：

- 方向光；
- 点光源；
- Spotlights；
- 环境光，并不是实际的光源；

这里给出的光照强度衰减系数：
$$
\alpha =\left\{\begin{matrix}
I, \text{Ambient and directional lights}\\ 
\frac{I}{c_0 + c_1{|\bold V - \bold P|}+c_2{|\bold V - \bold P|^2}}

\end{matrix}\right.
$$
材质参数包括：发射（$M_{emis}$），环境（$M_{ambi}$），漫反射（$M_{diff}$），镜面反射（$M_{spec}$），scalar of shininess（$M_{shine}$），alpha blending（$M_{alpha}$）。

光照效果的计算需要考虑到光源和材质。着色模型常见的有：flat，Gourand，和Phong。Flat指的是渲染三角形的时候所有的像素采用相同的颜色。Gourand着色

**TODO**

### 2.6.3 Textures



### 2.6.4 Transparency, opacity, and blending

### 2.6.5 Fog

### 2.6.6 And many more

### 2.6.7 Rasterizing attributes

