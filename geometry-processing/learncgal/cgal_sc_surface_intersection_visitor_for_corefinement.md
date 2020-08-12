# CGAL - SourceCode - Surface_intersection_visitor_for_corefinement 源码阅读

在[CGAL - SourceCode - box_intersection_d源码阅读     ](https://www.cnblogs.com/grass-and-moon/p/13353521.html)一文中介绍了如何利用box intersection快速找出相交对。

并在[PaperImpl- Fast Software for Box Intersections     ](https://www.cnblogs.com/grass-and-moon/p/13361323.html)介绍了如何利用c++实现box intersection。

在[CGAL - SourceCode - Intersection_of_triangle_meshes 源码阅读](https://www.cnblogs.com/grass-and-moon/p/13219952.html)一文中已经介绍了两个三角网格之间相交线的具体计算实现。但是对于相交线计算出来之后如何重新构建网格，并没有进一步展开。文本为Surface_intersection_visitor_for_corefinement代码阅读的记录。

代码位置：`CGAL\Polygon_mesh_processing\internal\Corefinement\Visitor.h`

实现的代码入口函数为：`finalize`，如下：

```c++
  void finalize(Intersection_nodes<TriangleMesh,
                 VertexPointMap, Predicates_on_constructions_needed>& nodes,
                 const TriangleMesh& tm1,
                 const TriangleMesh& tm2,
                 const VertexPointMap& vpm1,
                 const VertexPointMap& vpm2)
  {
    nodes.all_nodes_created();

    TriangleMesh* tm1_ptr = const_cast<TriangleMesh*>(&tm1);
    TriangleMesh* tm2_ptr = const_cast<TriangleMesh*>(&tm2);

    std::map<TriangleMesh*, VertexPointMap> vpms;
    vpms[tm1_ptr] = vpm1;
    vpms[tm2_ptr] = vpm2;

    vertex_descriptor null_vertex = Graph_traits::null_vertex();
    const Node_id nb_nodes = nodes.size();
    // we reserve nb_nodes+3 because we use the last three entries for the
    // face triangulation
    mesh_to_node_id_to_vertex[tm1_ptr].resize(nb_nodes+3, null_vertex);
    mesh_to_node_id_to_vertex[tm2_ptr].resize(nb_nodes+3, null_vertex);

    //store for each triangle face which boundary is intersected by the other surface,
    //original vertices (and halfedges in the refined mesh pointing on these vertices)
    typedef boost::unordered_map<face_descriptor,Face_boundary> Face_boundaries;
    std::map<TriangleMesh*,Face_boundaries> mesh_to_face_boundaries;

    //0) For each triangle mesh, collect original vertices that belongs to the intersection.
    //   From the graph of constraints, extract intersection edges that are incident to such vertices. In case
    //   there exists another original vertex adjacent to the first one found, this halfedge must be
    //   marked on the boundary (and possibly update an_edge_per_polyline).
    //   This is done first to avoid halfedges stored to be modified in the steps following.
    for (typename Mesh_to_vertices_on_intersection_map::iterator
          it=mesh_to_vertices_on_inter.begin();
          it!=mesh_to_vertices_on_inter.end();
          ++it)
    {
      // ......
    }

    //1) First split halfedges cut by the intersection polyline(s)
    for (typename std::map<TriangleMesh*,On_edge_map>::iterator
      it=on_edge.begin(); it!=on_edge.end(); ++it)
    {
      // ......
    }

    //2)triangulation of the triangle faces containing intersection point in their interior
    //  and also those with intersection points only on the boundary.
    for (typename std::map<TriangleMesh*,On_face_map>::iterator
      it=on_face.begin(); it!=on_face.end(); ++it)
    {
      // ...
    }

    nodes.finalize();

    // additional operations
    output_builder(nodes,
                   input_with_coplanar_faces,
                   is_node_of_degree_one,
                   mesh_to_node_id_to_vertex);
  }
```

从代码注释中发现，该实现的主要有三个步骤：

1. 对于每个三角网格，收集原始顶点中有哪些点属于交点。从约束图中，找到和这些顶点相关的相交的边。如果有两个相邻的原始顶点构成了一条相交边，那么这个半边必须标记成边界（并可能更新 `an_edge_per_polyline`）。首先执行这一步操作，可以避免存储的半边在以下步骤中被修改。
2. 根据相交线，将半边进行切分；
3. 对内部包含交点的三角形面以及仅在边界上有交点的三角形面进行三角剖分；

如果只需要对原理有个简单的认识，那么看到这里就可以结束了。

## 1. 原始顶点相交性判断

首先需要了解的数据结构是：`Mesh_to_vertices_on_intersection_map`。

```c++
   typedef std::vector<Node_id>                                        Node_ids;      // 存储了node id的数组
   typedef boost::unordered_map<face_descriptor,Node_ids>           On_face_map;      // 三角形面片和对应nodes的关系
   typedef boost::unordered_map<edge_descriptor,Node_ids>           On_edge_map;      // 三角形边和对应nodes的关系
   //to keep the correspondance between node_id and vertex_handle in each mesh
   typedef std::vector<vertex_descriptor>                     Node_id_to_vertex;      // nodeid和vertex对应关系
   typedef std::map<TriangleMesh*, Node_id_to_vertex >         Mesh_to_map_node;
   //to handle coplanar halfedge of polyhedra that are full in the intersection
   typedef std::multimap<Node_id,halfedge_descriptor>    Node_to_target_of_hedge_map; // nodeid和半边的关系
   typedef std::map<TriangleMesh*,Node_to_target_of_hedge_map>
                                           Mesh_to_vertices_on_intersection_map;      // 每个网格上，交点和半边的映射关系
```

然后看一下第一步的具体实现：

```c++
for (typename Mesh_to_vertices_on_intersection_map::iterator
     it=mesh_to_vertices_on_inter.begin();
     it!=mesh_to_vertices_on_inter.end();
     ++it)  // 对每个mesh进行遍历
{
    TriangleMesh& tm=*it->first;
    //   Face_boundaries& face_boundaries=mesh_to_face_boundaries[&tm];

    Node_to_target_of_hedge_map& nodes_to_hedge=it->second;
    for(typename Node_to_target_of_hedge_map::iterator
        it_node_2_hedge=nodes_to_hedge.begin();
        it_node_2_hedge!=nodes_to_hedge.end();
        ++it_node_2_hedge) // 对node进行遍历
    {
        Node_id node_id_of_first=it_node_2_hedge->first;
        // 获取节点的邻域节点，如果邻域节点为空，则为孤立点
        std::vector<Node_id>& neighbors=graph_of_constraints[node_id_of_first];
        if ( !neighbors.empty() )
        {
            // 遍历邻域点
            BOOST_FOREACH(Node_id node_id, neighbors)
            {
                //if already done for the opposite
                if (node_id >= node_id_of_first) continue;

                typename Node_to_target_of_hedge_map::iterator
                    it_node_2_hedge_two = nodes_to_hedge.find(node_id);
                // 相邻的点也在半边上
                if ( it_node_2_hedge_two!=nodes_to_hedge.end() ) //a full edge is on intersection
                {
                    //get the corresponding halfedge with vertex corresponding to node_id_of_first
                    halfedge_descriptor hedge=it_node_2_hedge->second;
                    halfedge_descriptor start=hedge;
                    bool did_break=false;
                    while ( source(hedge,tm) !=
                           target(it_node_2_hedge_two->second,tm) )
                    {
                        hedge=opposite(next(hedge,tm),tm);
                        if (tm1_ptr==tm2_ptr && hedge==start)
                        {
                            // ... 特殊情况处理
                        }
                    }
                    if (did_break) continue;
                    // 构建关联的边，放到marks_on_edges，output_builder内部。
                    std::pair<Node_id,Node_id> edge_pair(node_id,node_id_of_first);
                    call_put(marks_on_edges,tm,edge(hedge,tm),true);
                    // output_builder用来针对布尔关系，构建网格
                    output_builder.set_edge_per_polyline(tm,edge_pair,hedge);
                }
            }
        }
        #ifdef CGAL_COREFINEMENT_DEBUG
        else
        {
            std::cout << "X1: Found an isolated point" << std::endl;
        }
        #endif
    }
}
```

## 2. 半边切分

```c++
// on_edge的类型是：std::map<TriangleMesh*,On_edge_map> on_edge;
// 表示每个mesh，对应边和交点之间的关系
for (typename std::map<TriangleMesh*,On_edge_map>::iterator
     it=on_edge.begin(); it!=on_edge.end(); ++it)
{
    TriangleMesh& tm=*it->first;
    const VertexPointMap& vpm=vpms[&tm];
    On_edge_map& on_edge_map=it->second;
    On_face_map& on_face_map=on_face[&tm];
    Face_boundaries& face_boundaries=mesh_to_face_boundaries[&tm];

    for(typename On_edge_map::iterator it2=on_edge_map.begin();
        it2!=on_edge_map.end();
        ++it2)
    {
        //获取被split的半边
        halfedge_descriptor hedge=halfedge(it2->first,tm);
        //indices of the nodes to be inserted
        Node_ids& node_ids=it2->second;
        CGAL_assertion( std::set<Node_id>(node_ids.begin(), node_ids.end())
                       .size()==node_ids.size() );
        //对边上的交点进行排序
        sort_vertices_along_hedge(node_ids,hedge,tm,vpm,nodes);

        //save original face and nodes for face of hedge (1)
        // 保存hedge的面的原始面和nodes（1）
        if ( !is_border(hedge,tm) ){
            face_descriptor f=face(hedge,tm);
            typename Face_boundaries::iterator it_face = face_boundaries.find(f);
            if (it_face==face_boundaries.end())
                it_face=face_boundaries.insert(std::make_pair(f,Face_boundary(hedge,tm))).first;
            it_face->second.copy_node_ids(hedge,node_ids.begin(),node_ids.end());
        }

        //save original face and nodes for face of hedge->opposite (2)
        // 保存hedge对边的面的原始面和nodes
        typename Face_boundaries::iterator opposite_original_info=face_boundaries.end();
        halfedge_descriptor hedge_opp = opposite(hedge,tm);
        if ( !is_border(hedge_opp,tm) ){
            face_descriptor f=face(hedge_opp,tm);
            opposite_original_info=face_boundaries.find(f);
            if (opposite_original_info==face_boundaries.end())
                opposite_original_info=face_boundaries.insert(std::make_pair(f,Face_boundary(hedge_opp,tm))).first;
            opposite_original_info->second.copy_node_ids(hedge_opp,node_ids.rbegin(),node_ids.rend());
        }

        /// ......
        
        //do split the edges
        //split的关键代码如下：
        CGAL_assertion_code(vertex_descriptor expected_src=source(hedge,tm));
        BOOST_FOREACH(std::size_t node_id, node_ids)
        {
            // Split new hedge
            halfedge_descriptor hnew = Euler::split_edge(hedge, tm);
            CGAL_assertion(expected_src==source(hnew,tm));
            /// New vertex
            vertex_descriptor vnew=target(hnew,tm);
            //          user_visitor.new_vertex_added(node_id, vnew, tm); // NODE_VISITOR_TAG
            nodes.call_put(vpm, vnew, node_id, tm);
            // register the new vertex in the output builder
            // 将得到的新的vertex注册到output_builder中
            output_builder.set_vertex_id(vnew, node_id, tm);
            // 建立新创建的vertex和node之间的关系
            node_id_to_vertex[node_id]=vnew;
            if (first){
                first=false;
                hedge_incident_to_src=next(opposite(hedge,tm),tm);
            }

            //update marker tags. If the edge was marked, then the resulting edges in the split must be marked
            if ( hedge_is_marked )
                call_put(marks_on_edges,tm,edge(hnew,tm),true);

            CGAL_assertion_code(expected_src=vnew);
        }

        /// ......
    }
}
```

## 3. 三角剖分

**TODO：这部分代码很复杂，暂时只能理解是用来做三角剖分的，具体怎么做需要先了解单纯的三角剖分要怎么实现，在后续的文章中，会对这一块进行学习，还有再对这部分内容进行补充。**



