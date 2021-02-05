# Chapter2 The Fundation

## 2.3 相机

本书针对相机，仅考虑透视投影。

### 2.3.1 透视相机模型

![](./image/figure2-16.png)



下面确定view frustrum六个平面的方程。

**近平面**

令$\bold E$点沿视线方向，在近平面的投影为$\bold E_{near}$，那么平面上的点$\bold X$:
$$
\begin{align}
(\bold X - \bold E_{near})\cdot \bold D &= 0 \\
(\bold X - (\bold E + d_{min}\bold D))\cdot \bold D &= 0 \\
\bold D \cdot \bold X &= \bold D\cdot(\bold E + d_{min}\bold D)
\end{align}
$$
**远平面**

与近平面类似，可以得到远平面如下：
$$
\bold D \cdot \bold X = \bold D \cdot \bold E + d_{max}
$$
**左平面**

可以通过叉乘计算得到，具体推导略：
$$
\bold N_l = \frac{d_{min}\bold R - r_{min}\bold D}{\sqrt{d_{min}^2 + r_{min}^2}},\ \bold N_l\cdot (\bold X - \bold E) = 0
$$
**右平面**
$$
\bold N_r = \frac{-d_{min}\bold R + r_{max}\bold D}{\sqrt{d_{min}^2 + r_{max}^2}},\ \bold N_r\cdot (\bold X - \bold E) = 0
$$
**下平面**
$$
\bold N_b = \frac{d_{min}\bold U - u_{min}\bold D}{\sqrt{d_{min}^2 + u_{min}^2}},\ \bold N_b\cdot (\bold X - \bold E) = 0
$$
**上平面**
$$
\bold N_t = \frac{-d_{min}\bold U + u_{max}\bold D}{\sqrt{d_{min}^2 + u_{max}^2}},\ \bold N_t\cdot (\bold X - \bold E) = 0
$$
还可以根据viewport选定可视范围，如下图所示：

![](./image/figure2-17.png)

### 2.3.2 物体模型空间

物体自己的坐标系，称为object space，也称为model space。

### 2.3.3 世界空间

游戏里面最重要的坐标系是世界坐标系。关于模型坐标系和世界坐标系如下图所示：

![](./image/figure2-18.png)

向量之间的变换不需要考虑到平移量，因此将模型坐标系下的点转换到世界坐标系下的点，可以理解成通过仿射变换将$\bold P_0$变换到$\bold Q_0$，向量$\bold P_i - \bold P_0$变换到向量$\bold Q_i - \bold Q_0$，该变换如下：
$$
\bold Q = \bold Q_0 + M (\bold P - \bold P_0)
$$
那么矩阵M（模型到世界变换的坐标系）即：
$$
M = \begin{bmatrix}\bold Q_1 - \bold Q_0 & \bold Q_2 - \bold Q_0 & \bold Q_3 - \bold Q_0 
\end{bmatrix}\begin{bmatrix}\bold P_1 - \bold P_0 & \bold P_2 - \bold P_0 & \bold P_3 - \bold P_0\end{bmatrix}^{-1}
$$

模型点到世界点的转换矩阵的齐次形式如下：
$$
\begin{bmatrix}
\bold X_{world} \\ 1
\end{bmatrix} = 
\begin{bmatrix}
M & \bold B \\
0^T & 1
\end{bmatrix}  \begin{bmatrix} \bold X_{model} \\1 \end{bmatrix} = H_{world}\begin{bmatrix} \bold X_{model} \\1 \end{bmatrix}
$$
反之：
$$
\begin{bmatrix}
\bold X_{model} \\ 1
\end{bmatrix} = 
\begin{bmatrix}
M^{-1} & \bold -M^{-1}B \\
0^T & 1
\end{bmatrix}  \begin{bmatrix} \bold X_{world} \\1 \end{bmatrix} = H_{world}^{-1}\begin{bmatrix} \bold X_{world} \\1 \end{bmatrix}
$$

### 2.3.4 View, Camera, OR Eye Space

将世界坐标系的点转换到相机坐标系下，如下：
$$
\bold X_{view} = \begin{bmatrix} r \\ u \\ d \end{bmatrix} = 
\begin{bmatrix}
\bold R \cdot (\bold X_{world} - \bold E) \\
\bold U \cdot (\bold X_{world} - \bold E) \\
\bold D \cdot (\bold X_{world} - \bold E)
\end{bmatrix} =
\begin{bmatrix}
\bold R & \bold U & \bold D
\end{bmatrix}^T(\bold X_{world} - \bold E)
$$
**注意：文中提到该形式，RUD是基于左手坐标系的，要转换到右手坐标的话，为: {E;R,U,-D}**

那么齐次矩阵的形式如下：
$$
\begin{bmatrix}\bold X_{view} \\ 1\end{bmatrix} = \begin{bmatrix}Q^T & \bold -Q^T\bold E \\0^T & 1\end{bmatrix}  \begin{bmatrix} \bold X_{world} \\1 \end{bmatrix} = H_{view}\begin{bmatrix} \bold X_{world} \\1 \end{bmatrix}
$$
其中Q=[R U D]，逆变换如下：
$$
\begin{bmatrix}\bold X_{world} \\ 1\end{bmatrix} = \begin{bmatrix}Q & \bold E \\0^T & 1\end{bmatrix}  \begin{bmatrix} \bold X_{view} \\1 \end{bmatrix} = H_{view}^{-1}\begin{bmatrix} \bold X_{view} \\1 \end{bmatrix}
$$

### 2.3.5 Clip, Projection, OR Homogeneous Space

接下来将要实现的是，如何将camera坐标系下的点投影到屏幕上。在2.2节-

### 2.3.6 Window Space

### 2.3.7 Putting Them All together

