# PaperRead - Fast Software for Box Intersections

Zomorodian, Afra, Edelsbrunner,等. Fast Software for Box Intersections.[J].  International Journal of Computational Geometry & Applications,  2002.

https://xueshu.baidu.com/usercenter/paper/show?paperid=263465aaf3c43ffc4c6ea791c3e8833b&site=xueshu_se

本文提出了一种用来检测box和cube相交的算法。相比较其他算法的优势，简单，数据结构只涉及到boxes数组，以及对boxes的引用。

## 问题以及简单的解决方案

我们考虑算法能够处理以下两种输入：

- （自相交）一个数据集中的box相交情况；
- （互相交）两个数据集中的box相互相交的情况；

**输入**：n个boxes的一个集合（或n个，和m个boxes的两个集合）

**输出：**k对相交的boxes

同时假设，盒子在low endpoints为闭合间，在high endpoint为开区间。

### 重铸问题

