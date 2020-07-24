# CGAL - SourceCode - box_intersection_d源码阅读

[toc]

## box_intersection_d

源代码入口：`CGAL\box_intersection_d.h`

关于该代码实现的相关论文阅读，可参见：https://www.cnblogs.com/grass-and-moon/p/13344913.html

很容易找到对应的入口函数，如下：

```c++
// file: CGAL\box_intersection_d.h

// Generic call with box traits parameter.
// - make all default parameters explicit overloads (workaround)
template< class RandomAccessIter1, class RandomAccessIter2,
          class Callback, class BoxTraits >
void box_intersection_d(
    RandomAccessIter1 begin1, RandomAccessIter1 end1,
    RandomAccessIter2 begin2, RandomAccessIter2 end2,
    Callback callback,
    BoxTraits,
    std::ptrdiff_t cutoff,
    Box_intersection_d::Topology topology,
    Box_intersection_d::Setting  setting)
{
    if (topology == Box_intersection_d::CLOSED) {
        typedef Box_intersection_d::Predicate_traits_d<BoxTraits,true> Traits;
        box_intersection_custom_predicates_d(begin1, end1, begin2, end2,
                                         callback, Traits(), cutoff, setting);
    } else {
        typedef Box_intersection_d::Predicate_traits_d<BoxTraits,false> Traits;
        box_intersection_custom_predicates_d(begin1, end1, begin2, end2,
                                         callback, Traits(), cutoff, setting);
    }
}
```

此处需要关注的是`Box_intersection_d::Predicate_traits_d`，这个在后面的实现中会被用到。

## Predicate_traits_d

代码实现如下（路径：`CGAL\Box_intersection_d\Box_traits_d.h`）：

```c++
template< class BoxTraits, bool closed > // close 闭区间还是开区间
struct Predicate_traits_d : public BoxTraits {
    typedef typename BoxTraits::Box_parameter Box_parameter;
    typedef typename BoxTraits::NT         NT;

    template<bool b> struct Bool_t {};

    static bool hi_greater(NT hi, NT val, Bool_t<true> ) { return hi >= val;}
    static bool hi_greater(NT hi, NT val, Bool_t<false> ){ return hi >  val;}
    static bool hi_greater (NT hi, NT val) {
        return hi_greater(hi,val, Bool_t<closed>()); 
    }

    // compare dim a b = islolesslo a b dim
    class Compare : 
        public CGAL::cpp98::binary_function<Box_parameter,Box_parameter,bool>
    {
        int dim;
    public:
        Compare(int dim) : dim(dim) {}
        /// 某个维度下，box a的最小坐标小于box b的最小坐标（相等的话，对id进行判断），则返回true
        bool operator()(Box_parameter a, Box_parameter b) const { 
            return is_lo_less_lo(a,b,dim); 
        }
    };

    // loless val dim box = getlo box dim < val
    class Lo_less : public CGAL::cpp98::unary_function<Box_parameter,bool> {
        NT value;
        int dim;
    public:
        Lo_less(NT value, int dim) : value(value), dim(dim) {}
        /// box的最小坐标对应dim的值是不是比lo还小
        bool operator() (Box_parameter box) const { 
            return BoxTraits::min_coord(box, dim) < value;
        }
    };

    class Hi_greater : public CGAL::cpp98::unary_function<Box_parameter,bool> {
        NT value;
        int dim;
    public:
        Hi_greater(NT value, int dim) : value(value), dim(dim) {}
        /// box的最大坐标对应dim的值是不是比hi还大
        bool operator() (Box_parameter box) const {
            return hi_greater( BoxTraits::max_coord(box, dim), value);
        }
    };

    // spanning lo hi dim box = getlo box dim < lo && gethi box dim > hi
    class Spanning : public CGAL::cpp98::unary_function<Box_parameter,bool> {
        NT lo, hi;
        int dim;
    public:
        Spanning(NT lo, NT hi, int dim) : lo(lo), hi(hi), dim(dim) {}
        // returns true <=> box spans [lo,hi) in dimension dim
        // 如果[lo, hi)在box的范围内，那么返回true
        bool operator() (Box_parameter box) const {
            return BoxTraits::min_coord(box,dim) < lo 
                && BoxTraits::max_coord(box,dim) > hi;
        }
    };

    static Compare    compare_object(int dim) { return Compare(dim); }

    static Lo_less    lo_less_object(NT value, int dim) {
                          return Lo_less(value, dim);
    }
    static Hi_greater hi_greater_object(NT value, int dim) {
                          return Hi_greater( value, dim );
    }
    static Spanning   spanning_object(NT lo, NT hi, int dim) {
                          return Spanning( lo, hi, dim );
    }
    
    /// 某个维度下，box a的最小坐标小于box b的最小坐标（相等的话，对id进行判断），则返回true
    static bool is_lo_less_lo(Box_parameter a, Box_parameter b, int dim) {
        return BoxTraits::min_coord(a,dim)  < BoxTraits::min_coord(b,dim) ||
               ( BoxTraits::min_coord(a,dim) == BoxTraits::min_coord(b,dim) && 
                 BoxTraits::id(a) < BoxTraits::id(b) );
    }

    /// 某个维度下，box b的最大坐标，大于box a的最小坐标
    static bool is_lo_less_hi(Box_parameter a, Box_parameter b, int dim) {
        return hi_greater( BoxTraits::max_coord(b,dim), 
                           BoxTraits::min_coord(a,dim));
    }
    
    /// 某个维度下，box a的最小坐标小于box b的最大坐标，box b的最小坐标小于box a的最大坐标，
    /// 那么两个box在这个维度相交
    static bool does_intersect (Box_parameter a, Box_parameter b, int dim) {
        return is_lo_less_hi(a,b,dim) && is_lo_less_hi(b,a,dim);
    }
    
    /// 某个维度下，box a的最小坐标小于box b的最小坐标，box b的最小坐标，小于box a的最大坐标，
    /// 那么box b的最小坐标位于box a内
    static bool contains_lo_point(Box_parameter a, Box_parameter b, int dim) {
        return is_lo_less_lo(a,b,dim) && is_lo_less_hi(b,a,dim);
    }
};
```

## box_intersection_custom_predicates_d

这部分代码和，

```c++
// Generic call with custom predicate traits parameter.
template< class RandomAccessIter1, class RandomAccessIter2,
          class Callback, class BoxPredicateTraits >
void box_intersection_custom_predicates_d(
    RandomAccessIter1 begin1, RandomAccessIter1 end1,
    RandomAccessIter2 begin2, RandomAccessIter2 end2,
    Callback callback,
    BoxPredicateTraits traits,
    std::ptrdiff_t cutoff = 10,
    Box_intersection_d::Setting setting = Box_intersection_d::BIPARTITE)
{
    typedef BoxPredicateTraits Traits;
    typedef typename Traits::NT NT;
    CGAL_assertion( Traits::dimension() > 0 );
    const int dim = Traits::dimension() - 1;
    const NT inf = Box_intersection_d::box_limits<NT>::inf(); // 定义负无穷大
    const NT sup = Box_intersection_d::box_limits<NT>::sup(); // 定义正无穷大
    Box_intersection_d::segment_tree(begin1, end1, begin2, end2,
                              inf, sup, callback, traits, cutoff, dim, true);
    if(setting == Box_intersection_d::BIPARTITE)
        Box_intersection_d::segment_tree(begin2, end2, begin1, end1,
                              inf, sup, callback, traits, cutoff, dim, false);
}
```

重点实现见：`Box_intersection_d::segment_tree`和论文中的`Hybrid`伪代码实现相对应。

## segment_tree

先放上伪代码实现，如下，详细内容可参见：https://www.cnblogs.com/grass-and-moon/p/13344913.html

```c++
Algorithm Hybrid(I, P, lo, hi, d):
1.  if I = null or P = null or hi <= lo then
		return;
	endif;
2.  if d = 0 then
		OneWayScan(I, P, 0);
	endif;
3.  if |I| < c or |P| < c then
		ModifiedTwoWayScan(I, P, d);
	endif;
4.  Im = { i in I | [lo, hi) belong i};
	Hybrid(Im, P, -infinity, +infinity, d-1);
	Hybrid(P, Im, -infinity, +infinity, d-1);
5.  mi = ApproxMedian(P);
6.	Pl = {p in P | p < mi};
	Il = {i in I - Im | i and [lo, mi) \ne nul};
	Hybrid(Il, Pl, lo, mi, d);
7.	Pr = {p in P | p >= mi};
	Ir = {i in I - Im | i and [mi, hi) \ne nul};
	Hybrid(Ir, Pr, mi, hi, d);	
```

CGAL中的segment_tree的实现如下：

```c++
template< class RandomAccessIter1, class RandomAccessIter2,
          class Callback, class T, class Predicate_traits >
void segment_tree( RandomAccessIter1 p_begin, RandomAccessIter1 p_end,
                   RandomAccessIter2 i_begin, RandomAccessIter2 i_end,
                   T lo, T hi,
                   Callback callback, Predicate_traits traits,
                   std::ptrdiff_t cutoff, int dim, bool in_order )
{
    typedef typename Predicate_traits::Spanning   Spanning;
    typedef typename Predicate_traits::Lo_less    Lo_less;
    typedef typename Predicate_traits::Hi_greater Hi_greater;

    const T inf = box_limits< T >::inf();  // 负无穷大
    const T sup = box_limits< T >::sup();  // 正无穷大
	// 遍历结束判断
    if( p_begin == p_end || i_begin == i_end || lo >= hi )
        return;

    // 最后一个维度，采用one_way_scan算法实现
    if( dim == 0 )  {
        CGAL_BOX_INTERSECTION_DUMP( "dim = 0. scanning ... " << std::endl )
        one_way_scan( p_begin, p_end, i_begin, i_end,
                      callback, traits, dim, in_order );
        return;
    }

    // 当begin，end的距离小于某个值的时候，采用
    // modified_two_way_scan实现
    if( std::distance( p_begin, p_end ) < cutoff ||
        std::distance( i_begin, i_end ) < cutoff  )
    {
        CGAL_BOX_INTERSECTION_DUMP( "scanning ... " << std::endl )
        modified_two_way_scan( p_begin, p_end, i_begin, i_end,
                               callback, traits, dim, in_order );
        return;
    }

    // 如果查找过程中，segment interval [lo,hi)在一部分box的interval内
    // 那么，需要在其他的维度进行判断；
    // [i_begin, i_span_end) 包含[lo,hi)
    // [i_span_end, i_end)  : 不包含[lo,hi)
    RandomAccessIter2 i_span_end = lo == inf || hi == sup ? i_begin :
        std::partition( i_begin, i_end, Spanning( lo, hi, dim ) );

    if( i_begin != i_span_end ) {
        CGAL_BOX_INTERSECTION_DUMP( "checking spanning intervals ... " 
                                    << std::endl )
        // make two calls for roots of segment tree at next level.
        segment_tree( p_begin, p_end, i_begin, i_span_end, inf, sup,
                      callback, traits, cutoff, dim - 1,  in_order );
        segment_tree( i_begin, i_span_end, p_begin, p_end, inf, sup,
                      callback, traits, cutoff, dim - 1, !in_order );
    }

    // 将点分割成两部分
    T mi;
    RandomAccessIter1 p_mid = split_points( p_begin, p_end, traits, dim, mi );

    // 如果没有合适的p_mid位置了，那么执行modified_two_way_scan
    if( p_mid == p_begin || p_mid == p_end )  {
        CGAL_BOX_INTERSECTION_DUMP( "unable to split points! ")
        //dump_points( p_begin, p_end, traits, dim );
        CGAL_BOX_INTERSECTION_DUMP( "performing modified two_way_san ... " 
                                     << std::endl )
        modified_two_way_scan( p_begin, p_end, i_span_end, i_end,
                               callback, traits, dim, in_order );
        return;
    }

    // 找到interval中的mid位置
    RandomAccessIter2 i_mid;
    // separate left intervals.
    // left intervals have a low point strictly less than mi
    // 重新将intervals分割成两部分，
    // [i_span_end, i_mid): lo值小于mi
    // [i_mid, i_end) lo值大于mi
    i_mid = std::partition( i_span_end, i_end, Lo_less( mi, dim ) );
    CGAL_BOX_INTERSECTION_DUMP("->left" << std::endl )
    segment_tree( p_begin, p_mid, i_span_end, i_mid, lo, mi,
                  callback, traits, cutoff, dim, in_order );
    // separate right intervals.
    // right intervals have a high point strictly higher than mi
    // 重新将intervals分割成两部分，
    // [i_span_end, i_mid): hi值大于mi
    // [i_mid, i_end): hi值小于mi
    i_mid = std::partition( i_span_end, i_end, Hi_greater( mi, dim ) );
    CGAL_BOX_INTERSECTION_DUMP("->right"<< std::endl )
    segment_tree( p_mid, p_end, i_span_end, i_mid, mi, hi,
                  callback, traits, cutoff, dim, in_order );
}
```

此处segment_tree的实现，并不是按照传统方式，先构建segment_tree节点的数据结构，而是直接采用二分递归的方式实现的。基本的实现思路是：

- 对传递进来的两组box序列，一组box作为interval，一组box作为points，points中选择二分划分位置，然后对左边和右边采用递归处理的方式；
- 递归结束的条件，见上述代码中的注释；

接下来还需要介绍的函数有：one_way_scan，modified_two_way_scan，split_points。

## one_way_scan

```c++
template< class RandomAccessIter1, class RandomAccessIter2,
          class Callback, class Traits >
void one_way_scan( RandomAccessIter1 p_begin, RandomAccessIter1 p_end,
                   RandomAccessIter2 i_begin, RandomAccessIter2 i_end,
                   Callback callback, Traits, int last_dim,
                   bool in_order = true )
{
    typedef typename Traits::Compare Compare;
    /// 按照box的第0个维度最小坐标进行排序
    std::sort( p_begin, p_end, Compare( 0 ) ); 
    std::sort( i_begin, i_end, Compare( 0 ) );

    // for each box viewed as interval i
    for( RandomAccessIter2 i = i_begin; i != i_end; ++i ) {
        // look for the first box b with i.min <= p.min
        for( ; p_begin != p_end && Traits::is_lo_less_lo( *p_begin, *i, 0 );
             ++p_begin ) {}

        // look for all boxes with p.min < i.max
        // p的min在interval内部，那么interval的box和p的box就相交
        for( RandomAccessIter1 p = p_begin;
             p != p_end && Traits::is_lo_less_hi( *p, *i, 0 );
             ++p )
        {
            if( Traits::id( *p ) == Traits::id( *i ) )
                continue;
            // 当last_dim为0的时候，并不会进入这个循环
            for( int dim = 1; dim <= last_dim; ++dim )
                if( !Traits::does_intersect( *p, *i, dim ) )
                    goto no_intersection;
            if( in_order )
                callback( *p, *i );
            else
                callback( *i, *p );
        no_intersection:
            ;
        }
    }
}
```

## modified_two_way_scan

```c++
template< class RandomAccessIter1, class RandomAccessIter2,
          class Callback, class Traits >
void modified_two_way_scan(
    RandomAccessIter1 p_begin, RandomAccessIter1 p_end,
    RandomAccessIter2 i_begin, RandomAccessIter2 i_end,
    Callback callback, Traits, int last_dim,
    bool in_order = true )
{
    typedef typename Traits::Compare Compare;
	/// 按照box的第0个维度最小坐标进行排序
    std::sort( p_begin, p_end, Compare( 0 ) );
    std::sort( i_begin, i_end, Compare( 0 ) );

    // for each box viewed as interval
    while( i_begin != i_end && p_begin != p_end ) {
        // 如果第0个维度，i_begin的lo值比p_begin的lo值小
        if( Traits::is_lo_less_lo( *i_begin, *p_begin, 0 ) ) {
            // 对第0个维度，p的lo值小于i_begin的hi值小的box进行遍历
            // 即，对i的interval的内的lo点进行遍历
            for( RandomAccessIter1 p = p_begin;
                 p != p_end && Traits::is_lo_less_hi( *p, *i_begin, 0 );
                 ++p )
            {
                if( Traits::id( *p ) == Traits::id( *i_begin ) )
                    continue;

                // 判断在高维度上有没有不相交的可能
                for( int dim = 1; dim <= last_dim; ++dim )
                    if( !Traits::does_intersect( *p, *i_begin, dim ) )
                        goto no_intersection1;
                /// 判断p的最小坐标位于box i_begin内
                if( Traits::contains_lo_point( *i_begin, *p, last_dim ) ) {
                    if( in_order )
                        callback( *p, *i_begin );
                    else
                        callback( *i_begin, *p );
                }
            no_intersection1:
                ;
            }
            ++i_begin;
        } else { // 如果第0个维度，i_begin的lo值不小于p_begin的lo值
            for( RandomAccessIter2 i = i_begin;
                 i != i_end && Traits::is_lo_less_hi( *i, *p_begin, 0 );
                 ++i )
            {
                if( Traits::id( *p_begin ) == Traits::id( *i ) )
                    continue;
                for( int dim = 1; dim <= last_dim; ++dim )
                    if( !Traits::does_intersect( *p_begin, *i, dim ) )
                        goto no_intersection2;
                if( Traits::contains_lo_point( *i, *p_begin, last_dim ) ) {
                    if( in_order )
                        callback( *p_begin, *i );
                    else
                        callback( *i, *p_begin );
                }
            no_intersection2:
                ;
            }
            ++p_begin;
        }
    }
}
```

## split_points

```c++
// returns iterator for first element in [begin,end) which does not satisfy
// the Split_Points_Predicate: [begin,mid) contains only points strictly less
// than mi. so, elements from [mid,end) are equal or higher than mi.
template< class RandomAccessIter, class Predicate_traits, class T >
RandomAccessIter
split_points( RandomAccessIter begin, RandomAccessIter end,
              Predicate_traits traits, int dim, T& mi )
{
    // magic formula
    int levels = (int)(.91*std::log(((double)std::distance(begin,end))/137.0)+1);
    levels = (levels <= 0) ? 1 : levels;
    RandomAccessIter it = iterative_radon( begin, end, traits, dim, levels );
    mi = Predicate_traits::min_coord( *it, dim );
    return std::partition( begin, end,
                           typename Predicate_traits::Lo_less( mi, dim ) );
}
```

**关于这部分内容，并没找到合适的解释。**

