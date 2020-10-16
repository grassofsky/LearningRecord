# PaperImpl - 邻近凹点角平分线的多边形顶点快速凸分算法研究及应用

原文地址：http://nldxb.njfu.edu.cn/oa/darticle.aspx?id=201305032

## 算法基本原理介绍

**判断多变形的走向**：可以借助多边形面积的符号进行判断，<0，多边形为顺时针，>0，多边形为逆时针，多边形面积计算可以参见：https://www.cnblogs.com/TenosDoIt/p/4047211.html；面积公式如下：
$$
S = \frac{1}{2}\sum_{i=1}^{n}(y_i + y_{i+1})(x_i-x_{i+1})
$$
**顶点凸凹性判断**：顶点的入边向量和出边向量的叉乘，<0，则点为凹点，>0，则该点为凸点；

**算法过程简单描述**：

1）利用**顶点凸凹性判断**找出凹点，然后对该凹点做角平分线，假设该点为$p_i$，前一个点为$p_{i-1}$，后一个点为$p_{i+1}$，那么可以计算得到角平分线的法向量：
$$
\vec{n} = normalize(normalize(p_{i} - p_{i-1}) + normalize(p_{i+1} - p_{i}))
$$
$normalize$表示对向量进行归一化。接着利用原点到$p_i$点的向量求出原点到直线的距离，如下：
$$
d = \vec{op_i} \cdot \vec{n}
$$
此时，可以得到角平分线的隐式方程（关于直线隐式方程的介绍可以参见：[BookRead - 《3D数学基础 ：图形与游戏开发》 读后小结](https://www.cnblogs.com/grass-and-moon/p/13598114.html)），如下：
$$
\vec{op} \cdot \vec{n} = d
$$
其中$\vec{op}$表示，原点到直线上的任一点p构成的向量。

2）寻找凹点的可视点。可视点是除凹点及前后相邻顶点外的多边形其他顶点，且凹点与可视点的连接线段与多边形没有其他的交点。伪代码如下：

```c++
// algorithm find visible pts
// \parma[in] vector<Point> pts, pt_n, pt_1, ...., pt_n, pt_1
// \param[in] int id [1, n]
// \parma[out] vector<int> visiblePtIndex;
void CalculateAllVisiblePts(const vector<Point>& vecPts, int id, vector<int>& vecVisiblePtIndices)
{
    int nPts = vecPts.size() - 2;
    for (int i=1; i<= nPts; ++i)
    {
        if (i == id || i == id-1 || i == id+1)
        {
            continue;
        }
        
        auto segment = segment(vecPts[i], vecPts[id]);
        bool bInter = false;
        for (int j=1; j<nPts; ++j)
        {
            if (j == i || j == id-1 || j == id+1 || j == id ||
               j+1 == i || j+1 == id-1 || j+1 == id+1 || j+1 == id)
            {
                continue;
            }
            
            auto testsegment = segment(vecPts[j], vecPts[j+1]);
            if (IsSegmentIntersection(segment, testsegment))
            {
                bInter = true;
                break;
            }
        }
        if (!bInter)
        {
            vecVisiblePtIndices.push_back(i);
        }
    }
}
```

可视点查找的时候关键一步是判断线段的相交性。关于线段相交性判断的实现，类似伪代码如下：

```c++
// test segment intersection
class Segment
{
public:
    Segment(Point first, Point second) : m_first(first), m_second(second)
    {
        Vector lineDir = second - first;
        Vector3 lineDir(lineDir.x, lineDir.y, 0);
        Vector3 z(0, 0, 1);
        Vector3 normal = CrossProduct(z, lineDir);
        m_normal = Vector(normal.x, normal.y);
        if (Dot(first, m_normal) < 0)
        {
            m_normal = -m_normal;
        }
        m_normal = Normal(m_normal);
    }

    Point m_first;
    Point m_second;
    Vector m_normal;
    double m_d;
};
bool IsSegmentIntersection(Segment segment1, Segment segment2)
{
    double seg2pt1ToSeg1 = Dot(segment2.m_first, segment1.m_normal) - segment1.m_d;
    double seg2pt2ToSeg1 = Dot(segment2.m_second, segment1.m_normal) - segment1.m_d;
    double seg1pt1ToSeg2 = Dot(segment1.m_first, segment2.m_normal) - segment2.m_d;
    double seg1pt2ToSeg2 = Dot(segment1.m_second, segment2.m_normal) - segment2.m_d;
    
    if ((seg2pt1ToSeg1 * seg2pt2ToSeg1) < 0 && (seg1pt1ToSeg2 * seg1pt2ToSeg2) < 0)
    {
        return true;
    }
    
    if ((seg2pt1ToSeg1 * seg2pt2ToSeg1) == 0 && (seg1pt1ToSeg2 * seg1pt2ToSeg2) == 0)
    {
        return true;
    }
    return false;
}
```

3）找到可视点中里角平分线绝对距离最近的点为划分点，利用该划分点，将凹多边形划分为两个多边形；

4）判断划分后的多边形的凹凸性；如果是凹多边形进入下一步；否则停止；

5）递归循环。循环1）-4）步骤，直到全部多边形为凸多边形。

## 算法实现示例

