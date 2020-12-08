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

关于unity导入网格主要需要考虑的问题有：

- 怎么支持颜色信息；
- 坐标系转换；
- 三角形顶点组成顺序转换；
- 距离单位；

因为通常的软件，在网格处理导出的时候，都是基于右手坐标系，顶点逆时针顺序表明网格的正面，距离单位不固定；而在unity中，坐标系是基于左手坐标系，同时顶点的顺时针顺序表明是网格的正面，网格的背面通常不参与绘制，unity自带的shader通常不支持基于顶点的颜色设置。为此，该项目在原有项目的基础上做了一些改进以及扩展。

### PlyImporter

原始项目的说明见：https://github.com/3DBear/PlyImporter

PLY (Polygon File Format) importer for Unity.

在原来基础上的改动主要有

- 增加了PlyElement基类，专门用来处理不同的element，方便后期进行扩展，目前支持PlyFaceElement，PlyVertexElement；
- 增加了PlyProperty基类，处理不同的属性，按照属性的功能又分为PlyMultiProperty，用来支持单个property定义；PlyListProperty支持property list定义。

### 支持的功能以及需要满足的要求

#### 1. element vertex

元素顶点支持的格式类似如下：

```
element vertex 6462
property float x
property float y
property float z
property float nx
property float ny
property float nz
property uchar red
property uchar green
property uchar blue
property uchar alpha
```

其中

- `x,y,z,nx,ny,nz,red,green,blue,alpha`的名字是固定的；
- `x,y,z`必须同时出现；类型必须是`float`
- `nx,ny,nz`必须同时出现；类型必须是`float`
- `red,green,blue,alpha`属性必须同时出现；类型必须是`uchar`
- `property`定义的顺序可以是乱序的；
- `vertex`的名字是固定的；

#### 2. element face

类似如下：

```
element face 12920
property list uchar int vertex_indices
```

其中

- `vertex_indices`的名字是固定的；
- `face`的名字是固定的；
- list中的具体index类型支持int和uint两种格式；
- list中的count类型为uchar；

#### 3. element g_material

考虑到unity中提供的shader一般不支持顶点颜色设置，此处对常规的ply进行了一定的扩展；在comment区域给出全局元素和属性的设置；

```
comment element g_material
comment property uchar red 255
comment property uchar green 0
comment property uchar blue 0
comment property uchar alpha 255
```

其中：

- `g_material`名字是固定的；`red,green,blue,alpha`名字是固定的，必须是`uchar`

#### 4. element g_meshinfo

考虑到需要识别网格名称，引入该元素，具体如下：

```
comment element g_meshinfo
comment property string name what_ever_you_want
comment property string unit mm               { one of cm/m/mm }
comment property string x_inner x                   { one of x/y/z/-x/-y/-z}
comment property string y_inner y                   { one of x/y/z/-x/-y/-z}
comment property string z_inner z                   { one of x/y/z/-x/-y/-z}
comment property string coordinate right      { one of right/left }
```

其中：

- `g_meshinfo`名字是不定的；
- `name,unit,x_inner,y_inner,z_inner,coordinate`名字是固定的；
- `name,unit,x_inner,y_inner,z_inner,coordinate`的类型必须是`string`；
- 具体的名字`what_ever_you_want`中不能带空格；
- `unit`的值为`mm/cm/m`中的一个;
- `coordinate`的值为`right/left`中的一个，right表示右手坐标系，left表示左手坐标系；
- `comment property string x_inner x`前一个`x_inner`指的是unity坐标系中的x轴，后一个x对应网格数据中的x轴；用于定义坐标变换；
- property的顺序也可变化；

#### 5. data store order

必须先存储顶点element，然后再存储face element；

### TODO

- 支持PlyEdgeElement；
- Support for Binary Big Endian
- PLY exporting