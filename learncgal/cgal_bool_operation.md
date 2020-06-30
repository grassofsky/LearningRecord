# CGAL - Intersection_of_triangle_meshes

`CGAL\Polygon_mesh_processing\internal\Corefinement\intersection_impl.h`

## 基本原理

该functor用来计算三角形网格的两两相交。相交结果是polylines（多线段）。该算法的基本步骤如下：

每个三角形网格中，我们从边中提取出segments集合，从面中，提取出三角形集合。

### 第一步：利用Box_intersection_d粗筛

我们使用Box_intersection_d过滤出一个三角网格segments和另一个三角网格相交的区域。

### 第二步：求交

从这个过滤的集合中，每个(segment，triangle)对，我们查看相交类型。

1) segment和三角形内部相交：我们计算出相交的交点，对于每个与线段相关的面，我们保存交点和相交三角形的面的关系；

2) segment和三角形的边相交：计算相交的交点，保留这条边相关的三角形；

3) segment和三角形顶点相交：执行和2)相同的操作；

In case a segment belong to the triangle, we repeat the same procedure for each segment incident to this endpoint.？？？

注意给定一对（段，三角形）=（S，T），如果S属于T所在的平面，满足下面情况的话，我们不做任何事情：

- 没有三角形T'包含S使得T和T'共面

- 至少有一个三角形包含S

### 特殊步骤：共面处理

共面的三角形被过滤出来单独处理。

## 具体实现

### Operator()全览

```c++
  template <class OutputIterator>
  OutputIterator operator()(OutputIterator output,
                            bool throw_on_self_intersection,
                            bool build_polylines)
  {
    CGAL_assertion(!doing_autorefinement);

    const TriangleMesh& tm1=nodes.tm1;       // 第一个三角网格
    const TriangleMesh& tm2=nodes.tm2;       // 第二个三角网格
    const VertexPointMap& vpm1=nodes.vpm1;   // 第一个vertex point map
    const VertexPointMap& vpm2=nodes.vpm2;   // 第二个vertex point map

    // 第一步：利用Box_intersection_d过滤出有交集的部分
    filter_intersections(tm1, tm2, vpm1, vpm2, throw_on_self_intersection);
    filter_intersections(tm2, tm1, vpm2, vpm1, throw_on_self_intersection);

    Node_id current_node((std::numeric_limits<Node_id>::max)());
    CGAL_assertion(current_node+1==0);

    // 特殊步骤：处理共面的情况
    #ifndef DO_NOT_HANDLE_COPLANAR_FACES
    //first handle coplanar triangles
    if (&tm1<&tm2)
      compute_intersection_of_coplanar_faces(current_node, tm1, tm2, vpm1, vpm2);
    else
      compute_intersection_of_coplanar_faces(current_node, tm2, tm1, vpm2, vpm1);
    visitor.set_number_of_intersection_points_from_coplanar_faces(current_node+1);
    if (!coplanar_faces.empty())
      visitor.input_have_coplanar_faces();
    #endif // not DO_NOT_HANDLE_COPLANAR_FACES

    // 第二步：求交
    //compute intersection points of segments and triangles.
    //build the nodes of the graph and connectivity infos
    Edge_to_faces& tm1_edge_to_tm2_faces = (&tm1<&tm2)
                                         ? stm_edge_to_ltm_faces
                                         : ltm_edge_to_stm_faces;
    Edge_to_faces& tm2_edge_to_tm1_faces = (&tm1>&tm2)
                                         ? stm_edge_to_ltm_faces
                                         : ltm_edge_to_stm_faces;

    compute_intersection_points(tm1_edge_to_tm2_faces, tm1, tm2, vpm1, vpm2, current_node);
    compute_intersection_points(tm2_edge_to_tm1_faces, tm2, tm1, vpm2, vpm1, current_node);
    if (!build_polylines){
      visitor.finalize(nodes,tm1,tm2,vpm1,vpm2);
      return output;
    }
    //remove duplicated intersecting edges:
    //  In case two faces are incident along an intersection edge coplanar
    //  in a face of another polyhedron (and one extremity inside the face),
    //  the intersection will be reported twice. We kept track
    //  (check_coplanar_edge(s)) of this so that,
    //  we can remove one intersecting edge out of the two
    remove_duplicated_intersecting_edges();

    // 构建多线段
    //using the graph approach (at some point we know all
    // connections between intersection points)
    construct_polylines(output);

    visitor.finalize(nodes,tm1,tm2,vpm1,vpm2);
    return output;
  }

```

### filter_intersections全览

```c++
  void filter_intersections(const TriangleMesh& tm_f,
                            const TriangleMesh& tm_e,
                            const VertexPointMap& vpm_f,
                            const VertexPointMap& vpm_e,
                            bool throw_on_self_intersection)
  {
    std::vector<Box> face_boxes, edge_boxes;
    std::vector<Box*> face_boxes_ptr, edge_boxes_ptr;

    face_boxes.reserve(num_faces(tm_f));
    face_boxes_ptr.reserve(num_faces(tm_f));
    BOOST_FOREACH(face_descriptor fd, faces(tm_f))
    {
      halfedge_descriptor h=halfedge(fd,tm_f);
      face_boxes.push_back( Box(
        get(vpm_f,source(h,tm_f)).bbox() +
        get(vpm_f,target(h,tm_f)).bbox() +
        get(vpm_f,target(next(h,tm_f),tm_f)).bbox(),
        h ) );
      face_boxes_ptr.push_back( &face_boxes.back() );
    }

    edge_boxes.reserve(num_edges(tm_e));
    edge_boxes_ptr.reserve(num_edges(tm_e));
    BOOST_FOREACH(edge_descriptor ed, edges(tm_e))
    {
      halfedge_descriptor h=halfedge(ed,tm_e);
      edge_boxes.push_back( Box(
        get(vpm_e,source(h,tm_e)).bbox() +
        get(vpm_e,target(h,tm_e)).bbox(),
        h ) );
      edge_boxes_ptr.push_back( &edge_boxes.back() );
    }

    /// \todo experiments different cutoff values
    std::ptrdiff_t cutoff = 2 * std::ptrdiff_t(
        std::sqrt(face_boxes.size()+edge_boxes.size()) );

    Edge_to_faces& edge_to_faces = &tm_e < &tm_f
                                 ? stm_edge_to_ltm_faces
                                 : ltm_edge_to_stm_faces;

    #ifdef DO_NOT_HANDLE_COPLANAR_FACES
    typedef Collect_face_bbox_per_edge_bbox<TriangleMesh, Edge_to_faces>
      Callback;
    Callback callback(tm_f, tm_e, edge_to_faces);
    #else
    typedef Collect_face_bbox_per_edge_bbox_with_coplanar_handling<
      TriangleMesh, VertexPointMap, Edge_to_faces, Coplanar_face_set>
     Callback;
    Callback  callback(tm_f, tm_e, vpm_f, vpm_e, edge_to_faces, coplanar_faces);
    #endif
    //using pointers in box_intersection_d is about 10% faster
    if (throw_on_self_intersection){
        Callback_with_self_intersection_report<TriangleMesh, Callback> callback_si(callback);
        CGAL::box_intersection_d( face_boxes_ptr.begin(), face_boxes_ptr.end(),
                                  edge_boxes_ptr.begin(), edge_boxes_ptr.end(),
                                  callback_si, cutoff );
        if (callback_si.self_intersections_found())
         throw Self_intersection_exception();
    }
    else
        CGAL::box_intersection_d( face_boxes_ptr.begin(), face_boxes_ptr.end(),
                              edge_boxes_ptr.begin(), edge_boxes_ptr.end(),
                              callback, cutoff );
  }
```

