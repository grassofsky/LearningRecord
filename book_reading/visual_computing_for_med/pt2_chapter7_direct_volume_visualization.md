# visual computing for med reading  - pt2 chapter 7 直接体可视化

## 7.1 理论模型

### *介绍下只考虑吸收和发射的模型，density emitter model*

**发射模型**，对光线方向上，volume数据对最终结果的贡献进行累加。每个位置的对最终结果的贡献能力的大小可以用传输函数`T_F()`进行表示。又可以细分为透明度传输函数（OTF），颜色传输函数（CTF）。$I(s) = I(s_0) + \int_{s_0}^{s}Q(t)dt$

**吸收模型**，光线在传播过程中被吸收，那么没有被吸收的强度为: $I(s) = I(s_0) \cdot e^{(-\int_{s_0}^{s}\tau(t)dt)}$。

直接体渲染的标准模型，通常将**发射模型**和**吸收模型**一起考虑 $dI/ds = Q(s)-\tau(s)\cdot I(s)$。可以得到体渲染方程：
$$
I(s) = I(s_0) \cdot e^{(-\int_{s_0}^{s}\tau(t)dt)} +\int_{s_0}^{s}Q(p)\cdot e^{(-\int_{p}^{s}\tau(t)dt)}dp = I_{s0}\cdot T_{s0}(s) + \int_{s_0}^{s}Q(p) \cdot T_p(p)dp
$$
**这个积分是怎么实现的？**参见：https://blog.csdn.net/qq_32172681/article/details/101618070

解析解求解十分困难，可以得到离散解如下：
$$
I(s) = I(0)\prod_{k=0}^{n-1}t_k + \sum_{k-0}^{n-1}Q(k\cdot \Delta s)\cdot \Delta s \prod_{j=k+1}^{n-1}t_j
$$

## 7.2 渲染管线

### *介绍下体渲染管线*

常见的体渲染管线有两种模式，一种是，分类和光照放在采样后面， 一种是分类和光照放在采样前面，分别如下：

- ------> sampling ------> Classification& illumination -----> Compositing --->
- ------> Classification& illumination -----> sampling ------> Compositing --->

**采样**：

这里**采样**的概念是，在光线传输方向上选择合适的采样步长，并计算对应体位置上的体素值。（采样步长，插值方式）；

**classification & illumination**：

这里通常使用phong reflection model进行着色。确定对应位置的着色的公式为：

$` \ Q(s) = I_ak_aO_d(s) + I_dk_dO_d(s)(N(s)\cdot L) + I_sk_s(V\cdot R)^n \ `$

公式中对应概念为：

- Ia，环境光强度；
- ka，环境光反射系数；
- Od，对应位置上物体的漫反射颜色；
- Id，光源的漫反射光成分；
- Is，光源的镜面反射光成分；
- kd，漫反射系数；
- N(s), s点的梯度或是表面法向量；
- L，光源的方向；
- ks，镜面反射系数；
- R，光源镜面反射方向；
- V, 视线方向；
- n，镜面反射系数；

**compositing**：

当光线上的每个点的贡献都计算出来后，就需要对各个点进行组合。

## 7.3 组合 Compositing

### *双向compositing是怎么进行的*?

### *解释下下列Compositing实现，MIP，AVERAGE，CVP，MIDA*？

### *什么是预集成体渲染（preintegrated volume rendering）？*

## 7.5 常见的加速体渲染

### *常见的加速体渲染的方法有哪些？*

## 7.6 GPU体渲染

### *GPU Raycasting算法实现的基本流程介绍？*










