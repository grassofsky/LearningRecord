# PMP SRC Algo - 表面平滑

源代码见：https://github.com/pmp-library/pmp-library/blob/master/src/pmp/algorithms/SurfaceSmoothing.h

## laplacin平滑

其中SurfaceSmoothing的explicit_smoothing实现了常规的Laplacian平滑。

顶点vi的Laplacian算子为：
$$
\Delta v_i = L_i = \sum_{{v_j \in N_1(v_i)} }w_{ij}(v_j-v_i)
$$
其中
$$
w_{ij} = \frac{\omega_{ij}}{\sum_{{v_k \in N_1(v_i)} }\omega_{ik}}
$$
uniform的权重选择为：$\omega_{ij} = 1$;

cot的权重选择为：$\omega_{ij} = \cot\alpha + \cot \beta$;

最后更新后的点为：$v'_i = v_i + \Delta v_i$

Laplacian用矩阵形式进行表示如下（需要注意的是，此处的laplacian矩阵形式和上述的Laplacian算子概念上不一样的，上面的是$w_{ij}$加上$v_j-v_i$信息，此处仅仅是$w_{ij}$）：
$$
\begin{bmatrix}
\Delta v_1 \\
 \vdots \\
\Delta v_n
\end{bmatrix} = L \times 
\begin{bmatrix}
 v_1 \\
 \vdots \\
v_n
\end{bmatrix}
$$
其中：
$$
L_{ij} = 
\left\{\begin{matrix}
-\sum_{v_k \in N_1(v_i)} w_{ij} =  -1, i=j\\ 
w_{ij}, v_j \in N_1(v_i) \\
0, otherwise

\end{matrix}\right.
$$
对于显示的Laplacian平滑，是根据当前的状态预估出特定时间之后的状态。如下：
$$
V' = (I + \lambda dt L)V
$$
此处针对上述公式进行了简化，

1. 对权重进行了归一化，顶点v最后变换的结果，就是一邻域内，顶点vj的加权平均；
2. $\lambda dt$设定为1，通过迭代的方式来控制平滑程度；

具体实现如下：

```c++
void SurfaceSmoothing::explicit_smoothing(unsigned int iters,
                                          bool use_uniform_laplace)
{
    if (!mesh_.n_vertices())
        return;

    // compute Laplace weight per edge: cotan or uniform
    if (!mesh_.has_edge_property("e:cotan") ||
        how_many_edge_weights_ != mesh_.n_edges())
        compute_edge_weights(use_uniform_laplace);

    auto points = mesh_.get_vertex_property<Point>("v:point");
    auto eweight = mesh_.get_edge_property<Scalar>("e:cotan");
    auto laplace = mesh_.add_vertex_property<Point>("v:laplace");

    // smoothing iterations
    Vertex vv;
    Edge e;
    for (unsigned int i = 0; i < iters; ++i)
    {
        // step 1: compute Laplace for each vertex
        for (auto v : mesh_.vertices())
        {
            Point l(0, 0, 0);

            if (!mesh_.is_boundary(v))
            {
                Scalar w(0);

                for (auto h : mesh_.halfedges(v))
                {
                    vv = mesh_.to_vertex(h);
                    e = mesh_.edge(h);
                    l += eweight[e] * (points[vv] - points[v]);
                    w += eweight[e];
                }

                l /= w;
            }

            laplace[v] = l;
        }

        // step 2: move each vertex by its (damped) Laplacian
        for (auto v : mesh_.vertices())
        {
            points[v] += 0.5f * laplace[v];
        }
    }

    // clean-up custom properties
    mesh_.remove_vertex_property(laplace);
}
```

## implicit smoothing

隐式平滑公式如下：
$$
(I - \lambda dt L) V' = V
$$
此时关于Laplacian矩阵类似如下：
$$
L_{ij} = 
\left\{\begin{matrix}
-\sum_{v_k \in N_1(v_i)} w_{ij} , i=j\\ 
w_{ij}, v_j \in N_1(v_i) \\
0, otherwise

\end{matrix}\right.
$$
其中不同的是，对于implicit smoothing，此处的$w_{ij}$取值为非归一化的值，更为合适（**TODO：具体原因是啥？**），即：$w_{ij} = \omega_{ij}$。

具体代码实现如下：

```c++
void SurfaceSmoothing::implicit_smoothing(Scalar timestep,
                                          bool use_uniform_laplace,
                                          bool rescale)
{
    if (!mesh_.n_vertices())
        return;

    // compute edge weights if they don't exist or if the mesh changed
    if (!mesh_.has_edge_property("e:cotan") ||
        how_many_edge_weights_ != mesh_.n_edges())
        compute_edge_weights(use_uniform_laplace);

    // compute vertex weights
    compute_vertex_weights(use_uniform_laplace);

    // store center and area
    Point center_before = centroid(mesh_);
    Scalar area_before = surface_area(mesh_);

    // properties
    auto points = mesh_.get_vertex_property<Point>("v:point");
    auto vweight = mesh_.get_vertex_property<Scalar>("v:area");
    auto eweight = mesh_.get_edge_property<Scalar>("e:cotan");
    auto idx = mesh_.add_vertex_property<int>("v:idx", -1);

    // collect free (non-boundary) vertices in array free_vertices[]
    // assign indices such that idx[ free_vertices[i] ] == i
    unsigned i = 0;
    std::vector<Vertex> free_vertices;
    free_vertices.reserve(mesh_.n_vertices());
    for (auto v : mesh_.vertices())
    {
        if (!mesh_.is_boundary(v))
        {
            idx[v] = i++;
            free_vertices.push_back(v);
        }
    }
    const unsigned int n = free_vertices.size();

    // A*X = B
    SparseMatrix A(n, n);
    Eigen::MatrixXd B(n, 3);

    // nonzero elements of A as triplets: (row, column, value)
    std::vector<Triplet> triplets;

    // setup matrix A and rhs B
    dvec3 b;
    double ww;
    Vertex v, vv;
    Edge e;
    for (unsigned int i = 0; i < n; ++i)
    {
        v = free_vertices[i];

        // rhs row
        b = static_cast<dvec3>(points[v]) / vweight[v];

        // lhs row
        ww = 0.0;
        for (auto h : mesh_.halfedges(v))
        {
            vv = mesh_.to_vertex(h);
            e = mesh_.edge(h);
            ww += eweight[e];

            // TODO: 这一步的原理？
            // fixed boundary vertex -> right hand side
            if (mesh_.is_boundary(vv))
            {
                b -= -timestep * eweight[e] * static_cast<dvec3>(points[vv]);
            }
            // free interior vertex -> matrix
            else
            {
                triplets.emplace_back(i, idx[vv], -timestep * eweight[e]);
            }

            B.row(i) = (Eigen::Vector3d)b;
        }

        // center vertex -> matrix
        triplets.emplace_back(i, i, 1.0 / vweight[v] + timestep * ww);
    }

    // build sparse matrix from triplets
    A.setFromTriplets(triplets.begin(), triplets.end());

    // solve A*X = B
    Eigen::SimplicialLDLT<SparseMatrix> solver(A);
    Eigen::MatrixXd X = solver.solve(B);
    if (solver.info() != Eigen::Success)
    {
        std::cerr << "SurfaceSmoothing: Could not solve linear system\n";
    }
    else
    {
        // copy solution
        for (unsigned int i = 0; i < n; ++i)
        {
            points[free_vertices[i]] = X.row(i);
        }
    }

    if (rescale)
    {
        // restore original surface area
        Scalar area_after = surface_area(mesh_);
        Scalar scale = sqrt(area_before / area_after);
        for (auto v : mesh_.vertices())
            mesh_.position(v) *= scale;

        // restore original center
        Point center_after = centroid(mesh_);
        Point trans = center_before - center_after;
        for (auto v : mesh_.vertices())
            mesh_.position(v) += trans;
    }

    // clean-up
    mesh_.remove_vertex_property(idx);
}
```

对于代码中不理解的地方有：

1. fixed boundary vertex -> right hand side；
2. 为什么顶点需要除以顶点的权重，vweight[v]；

## 补充资料

- [Nealen, Andrew & Igarashi, Takeo & Sorkine, Olga & Alexa, Marc. (2006). Laplacian mesh optimization. 381-389. 10.1145/1174429.1174494. ](http://www.cs.jhu.edu/~misha/Fall07/Papers/Nealen06.pdf)
- 常见几种平滑算法的就介绍可以参见：
- 三角网格的Laplacian可以参见：
- Notes on Mesh Smoothing：http://mgarland.org/class/geom04/material/smoothing.pdf

