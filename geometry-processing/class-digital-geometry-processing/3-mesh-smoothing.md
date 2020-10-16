# DGP 3 - mesh smoothing

[toc]

视屏地址：https://www.bilibili.com/video/BV1B54y1B7Uc

## Filter-based methods

### Laplacian smoothing Introduction

关于laplacian smoothing更多的内容可以参见：[几种网格平滑算法的实现](https://www.cnblogs.com/chnhideyoshi/p/MeshSmoothing.html)

通过扩散流量来理解smoothing，一个信号$f(x,t)$随着时间的变化变化，像热扩散，在一块铁板的中心高温加热一段时间后停止，然后发现温度会逐渐向周边扩散，相邻介质之间的温度差会变得越来越小，最后趋于平衡。

扩散方程就是信号随时间的变化率，如下：
$$
\frac{\part f(x,t)}{\part t} = \lambda \Delta f(x,t)
$$
空间中一点的信号对时间的变化率，可以用扩散系数和对位置的Laplace-Beltrami算子的乘积表示。可以参见：[百度百科-热传导方程式](https://baike.baidu.com/item/%E7%83%AD%E4%BC%A0%E5%AF%BC%E6%96%B9%E7%A8%8B%E5%BC%8F)。

#### 空间离散化

$$
\frac{\part f(x,t)}{\part t} = \lambda \Delta f(x,t)
$$

该式子就是空间离散化的接口，用矩阵表示：
$$
\frac{\part f(t)}{\part t} = \lambda \bold{L} \bold{f(t)}
$$

#### 时间离散化

对于微小增量h，可以得到梯度函数：$\frac{\part f(t)}{\part t} = \frac{f(t+h) - f(t)}{h}$，那么可得Explicit Euler integration：
$$
f(t+h) = f(t) + h\frac{\part f(t)}{\part t} = f(t) + h\lambda \cdot\bold{L}f(t)
$$
进一步可以得到隐式的Euler integration：
$$
f(t+h) = f(t) + h\lambda \cdot\bold{L}f(t+h)
$$

#### Laplacian smoothing

Laplacian smoothing的作用可以理解为，原始点邻域内点的加权平均结果：
$$
Pos_{new} = \frac{1}{\sum \omega_i}\omega_i Adj(Pos_{old}) \\
\omega_i = \frac{1}{2}(\cot(\alpha_i) + \cot(\beta_i))
$$

### Laplacian smoothing implement

根据曲率添加颜色可以参见：https://blog.csdn.net/qq_38517015/article/details/105185241

TODO

## Optimization-based methods

## Data-driven methods