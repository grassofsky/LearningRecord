# STL 如何对STL进行扩展，简单小结

## 前言

本文为：《STL源码分析》阅读之后的小结。

主要的扩展方式是，按照特定的规则定义一个容器，一个迭代器。

## 定义一个迭代器

迭代器需要构造的类型有
  - value_type
  - difference_type
  - reference
  - pointer
  - iterator_category

构造的函数有：

  - operator*()
  - operator->()
  - operator++()
  - operator++(int);
  - operator--()
  - operator--(int)

## 定义一个容器

组合迭代器，参照类型，构建对应的typedef

  - value_type
  - difference_type
  - reference
  - pointer
  - iterator_category

## 定义一个算法

算法利用迭代器，实现对容器的弱依赖关系。