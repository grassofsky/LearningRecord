# Unity Ply网格读取

## Ply格式说明

The version 1.0 PLY format, 详细参见：https://ww2.mathworks.cn/help/vision/ug/the-ply-format.html

### File Header

文件头示例’{}‘为说明：

```
ply                             {文件头}
format binary_big_endian 1.0    {数据格式以及版本}
element vertex 9200             {定义vertex元素}
property float x                {定义属性}
property float y
property float z
element face 18000              {定义面元素}
property list uchar int vertex_indices
end_header                      {这行后面就是数据了}
```

其中，`format <data format> <PLY version> `中的`<data format>`支持（little/big endian refers to the byte ordering of multi-byte data）：

- ascii
- binary_little_endian
- binary_big_endian

元素定义方式如下：

```
element <element name> <number in file>
property <data type> <property name 1>
property <data type> <property name 2>
property <data type> <property name 3>
...
```

其中对应的`<data type>`有：

| Name   | Type                            |
| ------ | ------------------------------- |
| char   | (8-bit) character               |
| uchar  | (8-bit) unsigned character      |
| short  | (16-bit) short integer          |
| ushort | (16-bit) unsigned short integer |
| int    | (32-bit) integer                |
| uint   | (32-bit) unsigned integer       |
| float  | (32-bit) single-precision float |
| double | (64-bit) double-precision float |

对于list的属性为：

```
property list <count data type> <data type> <property name> 
```

在文件头里面还可以添加注释，具体格式如下：

```
comment <comment text>
```

### Data

header之后存数据。按照之前约定的元素顺序和属性类型，依次存储。

### Common Elements and Properties

常见元素和属性为：

| Required Core Property | Element  | Property       | Data Type   | Property Description          |
| ---------------------- | -------- | -------------- | ----------- | ----------------------------- |
| ✓                      | vertex   | x              | `float`     | *x*,*y*,*z* coordinates       |
| ✓                      |          | y              | `float`     |                               |
| ✓                      |          | z              | `float`     |                               |
|                        |          | nx             | `float`     | *x*,*y*,*z* of normal         |
|                        |          | ny             | `float`     |                               |
|                        |          | nz             | `float`     |                               |
|                        |          | red            | `uchar`     | vertex color                  |
|                        |          | green          | `uchar`     |                               |
|                        |          | blue           | `uchar`     |                               |
|                        |          | alpha          | `uchar`     | amount of transparency        |
|                        |          | material_index | `int`       | index to list of materials    |
|                        | face     | vertex_indices | list of int | indices to vertices           |
|                        |          | back_red       | `uchar`     | backside color                |
|                        |          | back_green     | `uchar`     |                               |
|                        |          | back_blue      | `uchar`     |                               |
|                        | edge     | vertex1        | `int`       | index to vertex               |
|                        |          | vertex2        | `int`       | index to other vertex         |
|                        |          | crease_tag     | `uchar`     | crease in subdivision surface |
|                        | material | red            | `uchar`     | material color                |
|                        |          | green          | `uchar`     |                               |
|                        |          | blue           | `uchar`     |                               |
|                        |          | alpha          | `uchar`     | amount of transparency        |
|                        |          | reflect_coeff  | `float`     | amount of light reflected     |
|                        |          | refract_coeff  | `float`     | amount of light refracted     |
|                        |          | refract_index  | `float`     | index of refraction           |
|                        |          | extinct_coeff  | `float`     | extinction coefficent         |

## Unity Ply Reader

附上项目路径：https://github.com/grassofsky/PlyImporter

### PlyImporter

原始项目的说明见：https://github.com/3DBear/PlyImporter

在原来基础上的改动主要有

- 增加了PlyElement基类，专门用来处理不同的element，方便后期进行扩展，目前支持PlyFaceElement，PlyVertexElement；
- 增加了PlyProperty基类，处理不同的属性，按照属性的功能又分为PlyMultiProperty，用来支持单个property定义；PlyListProperty支持property list定义。

### TODO

- 支持PlyEdgeElement；
- 考虑到Unity中的shader基本没有用到顶点的颜色，增加PlyMaterialElement用来设定材质颜色；
- 通常对于加载的文件，需要知道该文件对应的网格名称，扩展comment支持更多的选项；
- Support for Binary Big Endian
- PLY exporting