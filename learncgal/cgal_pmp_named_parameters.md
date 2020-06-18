# CGAL Polygon mesh processing named parameters

cgal文档见：https://doc.cgal.org/latest/Polygon_mesh_processing/group__pmp__namedparameters.html

本文对named parameters做些介绍，以及简单的描述下Polygon Mesh Processing中具有对应示例的named parameters。

## 什么是named parameters

参见：https://isocpp.org/wiki/faq/ctors

在此之前先简单的描述下什么是**method chaining**。

method chaining，就是一个方法返回一个对象，然后这个对象还能够继续调用别的方法，示例如：`object.method1().method2()`。c++中使用最多的就是`cout << x << y`。

### 背景

c++只支持位置参数，也就是在调用函数的时候，参数的顺序是确定的。但是对于想Graph这样的实现而言，又是后一个函数需要很多的参数，那么如果像平常实现函数一样用`fun(int, int, int, int)`类似的形式给定参数的话，很容易忘记参数应该是什么顺序的。named parameters就在这种情况下发挥出了他的作用。具体是怎么做的呢？

它将函数的参数定义成了一个新类的方法，并且这个方法可以返回该类的引用。这样就简单的解除了对顺序的依赖。接下来看一下简单的例子。

### 例子描述

例如“打开一个文件”。需要传入文件名，需要选择是以只读方式打开，还是以可写方式打开，还是允许在文件不存在的时候进行创建。在写入的时候，是进行追加写入，还是进行覆盖写入，创建文件的时候，块的大小，I/O是缓冲的还是非缓冲的，缓冲区大小，共享还是独占访问，等。如果我们使用带有位置参数的普通函数实现这个概念，那么调用方代码将非常难以读取：将有多达8个位置参数，调用方可能会犯很多错误。因此，我们使用命名参数习惯用法

### 例子实现

```c++
class File;

// OpenFile
class OpenFile {
public:
  OpenFile(const std::string& filename);
    // sets all the default values for each data member
  OpenFile& readonly();  // changes readonly_ to true
  OpenFile& readwrite(); // changes readonly_ to false
  OpenFile& createIfNotExist();
  OpenFile& blockSize(unsigned nbytes);
  // ...
private:
  friend class File;
  std::string filename_;
  bool readonly_;          // defaults to false [for example]
  bool createIfNotExist_;  // defaults to false [for example]
  // ...
  unsigned blockSize_;     // defaults to 4096 [for example]
  // ...
};
inline OpenFile::OpenFile(const std::string& filename)
  : filename_         (filename)
  , readonly_         (false)
  , createIfNotExist_ (false)
  , blockSize_        (4096u)
{ }
inline OpenFile& OpenFile::readonly()
{ readonly_ = true; return *this; }
inline OpenFile& OpenFile::readwrite()
{ readonly_ = false; return *this; }
inline OpenFile& OpenFile::createIfNotExist()
{ createIfNotExist_ = true; return *this; }
inline OpenFile& OpenFile::blockSize(unsigned nbytes)
{ blockSize_ = nbytes; return *this; }

class File {
    public:
    File(const OpenFile& params);
    // ...
};

// with default parameters
File f = OpenFile("foo.txt"); 

// change parameters
File f = OpenFile("foo.txt")
    		.readonly()
    		.createIfNotExist()
    		.appendWhenWriting()
    		.blockSize(1024)
    		.unbuffered()
    		.exclusiveAccess();
```

## Named Parameters for Polygon Mesh Processing

其参数的构建方式和上面类似，如：

```c++
typedef CGAL::Extract_predicates_inexact_constructions_kernel K;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
namespace PMP = CGAL::Polygon_mesh_processing;

Mesh mesh;
PMP::parameters::vertex_point_map(get_property_map(CGAL::vertex_point, mesh))
    .face_index_map(get_property_map(CGAL::face_index, mesh))
    .clip_volume(true);
```

与mesh直接相关的参数有：`vertex_point_map`, `vertex_index_map`, `face_index_map`, `edge_is_constrained_map`。

下面有很多概念不理解。不理解的有：geom_traits, vertex_incident_patches_map, vertex_feature_degree_map, vertex_is_constrained_map, face_patch_map, first_index, sparse_linear_solver, apply_per_connected_component, visitor, 

### 部分Named Parameters理解

#### density_control_factor

控制网格细分的程度。详细参考，仅用refine(细分网格)函数，调整density_control_factor参数：https://doc.cgal.org/latest/Polygon_mesh_processing/Polygon_mesh_processing_2refine_fair_example_8cpp-example.html#a4。

```c++
// 代码片段
PMP::refine(poly,
            faces(poly),
            std::back_inserter(new_facets),
            std::back_inserter(new_vertices),
            Params::density_control_factor(2.));
PMP::refine(poly,
            faces(poly),
            std::back_inserter(new_facets),
            std::back_inserter(new_vertices),
            Params::density_control_factor(10));
```

效果对比如下：

![](./image/refine_density.png)

#### fairing_continuity

参见：https://doc.cgal.org/latest/Polygon_mesh_processing/Polygon_mesh_processing_2refine_fair_example_8cpp-example.html#a4。

效果如下：

![](./image/fair.png)

从效果中可见，函数fair是用来对一定区域的网格做平滑处理。具体要参考论文了：https://doc.cgal.org/latest/Polygon_mesh_processing/citelist.html#CITEREF_Botsch2008OnLinearVariational。

#### parameter for isotropic_remeshing

- number_of_iterations:
- protect_constraints 
- collapse_constraints 
- relax_constraints 
- number_of_relaxation_steps 

remesh 依次执行边分割、边折叠、边翻转、切线松弛和投影到初始曲面，以生成具有指定边长度的平滑网格。 参考demo见：https://doc.cgal.org/latest/Polygon_mesh_processing/Polygon_mesh_processing_2isotropic_remeshing_example_8cpp-example.html#a5

如果remesh参数种给的target_edge_length为0，那么不会执行边分割，边折叠。



#### use_delaunay_triangulation 

#### use_random_uniform_sampling 

#### use_grid_sampling 

#### use_monte_carlo_sampling 

#### sample_edges 

#### sample_vertices 

#### sample_faces 

#### number_of_points_on_faces 

#### number_of_points_on_edges 

#### number_of_points_per_face

#### number_of_points_per_edge 

#### grid_spacing 

#### number_of_points_per_area_unit 

#### number_of_points_per_distance_unit

#### do_project

#### random_seed 

#### outward_orientation 

#### do_overlap_test_of_bounded_sides 

#### projection_functor 

#### throw_on_self_intersection 

#### clip_volume

#### use_compact_clipper 

#### output_iterator 

#### erase_all_duplicates 

#### require_same_orientation 

#### snapping_tolerance 

#### use_angle_smoothing_t 

#### use_area_smoothing_t 

#### use_Delaunay_flips_t 

#### use_safety_constraints 

#### face_size_map 