# Chapter2 The Fundation

## 2.4 Culling and Clipping

Culling用于将视线不可见的部分剔除掉，cliping用来计算对象和view frustum的相交，对于三角网格，clipping操作通常针对不同的view frustum平面对三角网格进行划分，只绘制view frustum内部的三角形。

### 2.4.1 Object Culling

通常情况下你想要实现的目标是：

```
Cost(inexact_culling) < Cost(exact_culling)
Cost(inexact_drawing) > Cost(exact_drawing)
Cost(inexact_culling) + Cost(inexact_drawing) < Cost(exact_culling) + Cost(exact_drawing)
```

测试这个假设的方法只有通过对你的app进行不断的测试。

![](./image/figure2-22.png)

### 2.4.2 Back-Face culling

假设三角形的三个点在view space的坐标为：$\bold V_i = (r_i, u_i, d_i, 1), \ 0\le 1 \le2$。三角形的法向量为：
$$
\bold N = (\bold V_1 - \bold V_0) \times (\bold V_2 - \bold V_0)
$$


### 2.4.3 Clipping to the View Frustum

