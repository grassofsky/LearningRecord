# PaperRead - Implicit Fairing of Irregular Meshes using Diffusion and Curvature Flow

> Desbrun, Mathieu & Meyer, Mark & Schröder, Peter & Barr, Alan. (2001). Implicit Fairing of Irregular Meshes Using Diffusion and Curvature Flow. SIGGRAPH. 99. 

## 2. Implicit fairing

这一节介绍implicit fairing，一种用于网格平滑的扩散方程的隐式积分。证明几点相对于explicit methods的优点。虽然本节仅限于使用扩散项的线性近似，但隐式光顺将作为一种稳健而有效的数值方法贯穿全文，即使对于非线性算子也是如此。我们从建立框架和定义符号开始。

### 2.1 标记以及定义

X表示网格；xi表示网格上的顶点，eij表示连接xi和xj的边，N1(i)表示顶点的一邻域。

在曲面光顺（surface fairing）的文献中，大多技术使用了受限能量最小。最常使用的公式为，表面的总曲率：
$$
E(S) = \int_S\kappa_1^2 + \kappa_2^2dS
$$

由于主曲率是非线性的。在实际运用中更加倾向于membrane或thin-plate函数。

$$
E_{membrane}(X) = \frac{1}{2}\int_\Omega X_u^2 + X_v^2 dudv \\
E_{thin plate}(X) = \frac{1}{2}\int_\Omega X_{uu}^2 + 2X_{uv}^2 + X_{vv}^2 dudv
$$
TODO:

### 2.2 Diffusion equation for mesh fairing

正如我们指出的，在网格中衰减噪声的一种常见方法是通过扩散过程：
$$
\frac{\part X}{\part t} = \lambda L(X)
$$
将上面的公式随时间积分，一个小扰动将在其附近迅速分散，平滑高频，而主要形状只会轻微退化。拉普拉斯算子可以用伞形算子在每个顶点处线性逼近 ：
$$
L(x_i) = \frac{1}{m}\sum_{j\in N_1(i)}x_j - x_i
$$
其中m为邻域顶点个数。

那么网格的扩散方程可以表示为：
$$
X^{n+1} = (I + \lambda dtL)X^n
$$

对于伞算子，稳定性准则要求λdt<1。如果时间步长不满足这个条件，表面会出现涟漪，并最终在整个表面上产生越来越大的振荡。另一方面，如果满足这个条件，随着n的增加，初始网格会变得越来越平滑。

**TODO: 构建demo验证**

### 2.3 Time-shifted evaluation

2.2节中提供的方法是显式的方法，也叫做前向euler方法。不足的地方是，当网格很大的时候，需要很多次迭代才能够有个显著的平滑效果。

implicit的方法，可以避免上述的情况，具体公式如下：
$$
(I - \lambda dt L)X^{n+1} = X^n
$$
然后，通过增加值λdt，可以安全地获得后续平滑。但解决一个线性系统是要付出代价的。

**TODO: 构建demo验证**

### 2.3 Solving the sparse linear system

