# Tinyply 源码阅读

ply格式介绍见：http://paulbourke.net/dataformats/ply/

tinyply项目路径为：https://github.com/ddiakopoulos/tinyply/

## 主要数据结构

- Type，定义支持的数据类型；
- PropertyInfo，包装对应数据类型所占字节大小，以及对应的文本表示；
- PolyData？
- PlyProperty，定义property对应的名字，以及类型，是不是list，以及list的大小；
- PlyElement，定义element对应的名字， 以及关联的PlyProperty
- PlyFile，文件解析的具体实现；

