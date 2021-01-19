# Geometric tools - Introduction - Graphics

[toc]

https://www.geometrictools.com/Source/Graphics.html

这是组成图形引擎的大量文件集合。该引擎支持DirectX 11和OpenGL 4.5。几何工具库最终将支持DirectX 12和Vulkan。

## Resources

https://www.geometrictools.com/Source/Resources.html

GraphicsObject类是资源、着色器和绘图状态的抽象基类。它旨在支持GTEngine对象（前端）和graphics API对象（后端）之间的桥接模式。这些资源包括缓冲区对象、纹理和纹理数组，包括对mipmapping、渲染目标、深度模具纹理和立方体贴图的支持。

- 资源类派生自GraphicsObject，并充当独立于平台的缓冲区和纹理的基类。如果需要，该类具有系统内存存储，并向图形引擎提供有关如何管理GPU资源的提示。枚举DFType直接映射到Direct3D 11.0顶点和纹理格式，尽管GTEngine不支持所有格式。支持的被标记在DataFormat::msSupported[]中。`Resource.h/.cpp, DataFormat.h/.cpp`
- Buffer类是所有缓冲区类型的抽象基类。顶点和索引格式文件为创建顶点和索引缓冲区提供了方便的枚举。StructuredBufferN模板用于在资源表示一维、二维或三维数组时方便地键入StructuredBuffer的元素。 `Buffer, ConstantBuffer, IndexBuffer, IndexFormat, IndirectArgumentsBuffer, MemberLayout, RawBuffer, StructuredBuffer, TextureBuffer, TypedBuffer, VertexBuffer, VertexFormat`
- 纹理类是所有纹理类型的抽象基类。纹理可以作为单个对象出现；这些对象的基类是TextureSingle。纹理也可以作为对象数组出现；这些对象的基类是TextureArray。DrawTarget是用于绘制到屏幕外内存的渲染目标（TextureRT）和深度模具纹理（TextureD）的类包装器。`Texture, TextureSingle, Texture1, Texture2, Texture3, TextureArray, Texture(1-3)Array, TextureCube, TextureCubeArray, DrawTarget, TextureRT, TextureDS`

## Shaders

https://www.geometrictools.com/Source/Shaders.html

引擎当前支持顶点、几何体、像素和计算着色器。我们不支持域着色器、外壳着色器或细分。引擎的开发是由我们的目标驱动的，即拥有GPGPU支持，这不需要细分。我们将考虑在以后添加细分支持。提供了一个着色器工厂，允许您编译和反射着色器，可以从磁盘上的文件加载，也可以作为std::string存储在内存中。

- 用于vertex,geometry, pixel, compute shader的类；
- 工厂，用来编译来自disk或std::string中的shader。`ProgramFactory, ProgramDefines, ComputeProgram, VisualProgram` 

## Drawing State

https://www.geometrictools.com/Source/DrawingState.html

基本渲染使用状态对象，如混合、深度模具和光栅化状态（消隐、实体与线框、抗锯齿）以及纹理采样。它们被封装在一个小型的类层次结构中。

## Effects

https://www.geometrictools.com/Source/Effects.html

图形引擎有一些简单的内置视觉效果。这些包括位图字体和文本绘制、基本的2D和3D纹理、恒定颜色（材质颜色）和顶点着色，以及具有环境光、平行光、点光源或聚光灯的标准光源。覆盖也提供了一种效果（绘制按钮、滑块和其他屏幕空间对象）。

- effects的抽象类：`VisualEffect`
- 支持位图字体和绘图文本作为三维图形绘制后的覆盖。工具Tools/BitmapFontCreator允许您选择字体并自动生成包含字体信息的源文件（h、cpp），您只需将其添加到引擎并在应用程序中使用即可。Arial字体是示例应用程序使用的默认字体；它是由BitmapFontCreator工具生成的。`Font*, TextEffect`
-  恒定（材质）着色和顶点着色的效果。 `ConstantColorEffect, VertexColorEffect`
- 基本的2D，3D纹理效果，`Texture2Effect, Texture3Effect`
- 使用环境光、平行光、点光源或聚光灯的标准光源的效果。为逐顶点照明或逐像素照明提供效果。`Lighting, Material, LightCameraGeometry, LightEffect, AmbientLightEffect, DirectionLightEffect, DirectionLightTextureEffect, PointLightEffect, PointLightTextureEffect, SpotLightEffect`
-  将覆盖图绘制为屏幕空间对象的方便效果；例如，它支持绘制按钮、滑块、控件等。这对于2D应用程序也很方便，其中覆盖的纹理充当客户端窗口的像素。 `OverlayEffect`
- 平面反射的全局效果。有关如何使用效果的示例，请参见物理示例应用程序BouncingBall。`PlanarReflectionEffect`
- 带面积的光源（矩形光源）`AreaLightEffect`
- 凹凸贴图的简单效果`BumpMapEffect`
- 通过cubemap设置环境光照`CubeMapEffect`
- 简单的光泽贴图效果`GlossMapEffect`
- 通过球体贴图的环境照明效果。`SphereMapEffect`

## Scene Graphs

https://www.geometrictools.com/Source/SceneGraphs.html

场景图支持比Wild Magic 5大大减少。只有基本结构可用，包括空间层次结构，其中空间类用于叶节点，节点类用于内部节点，以及附加几何体和视觉效果的视觉类（从空间派生）。仍然支持沿层次向下传播变换和沿层次向上创建边界球体（用于对象消隐）。拾取系统保持完整（空间层次中的线对象相交查询）。提供了一个网格工厂来创建一些简单的标准对象。

- *层次结构*。抽象类Spatial存储spatial层次结构的父指针。Node类存储层次结构的子指针，用于树的内部节点。Visual类，继承自Spatial，用来表示需要绘制视觉效果的几何对象。虽然我们目前还不支持3D声音，但是可以创建一个类Audial来表示声音发射器，并且可以添加一个声音渲染引擎，其中监听器类似于用于视觉渲染的摄像机。消隐系统也可以用来消除离听者足够远的声音发射器。摄影机用于渲染（光的接收器），灯光提供光源（光的发射器）。摄像机和光源都和view volume关联。PVWUpdater类用于更新projection-view-world矩阵。`Spatial, Node, Visual, BoundingSphere, ViewVolume, ViewVolumeNode, Camera, Light, PVWUpdater, Particles`
- *Culling:*基于场景图的使用边界体的对象剔除。`Culler, CullingPlane`
- *Controllers:*空间类派生自ControlledObject。可以将控制器附加到此类对象以实现动态行为，例如修改变换、关键帧动画、蒙皮、反向运动学、顶点变形、点系统和粒子系统. `ControlledObject, Controller, TransformController, BlendTransformController, KeyframeController, SkinController, IKController, MorphController, PointController, ParticleController`
- *Picking*. 拾取系统用于选择显示在窗口中的对象和对象的一部分。您可以通过鼠标左键拖动并单击操作来完成此操作（请参见应用程序层）。该系统还可用于支持物理引擎；例如，可能会发射激光枪，您需要确定场景中的哪个对象将被激光击中。可以在场景图中分层执行拾取以使操作快速（对具有N个对象的树的顺序Log(N)遍历）。当对象是三角形网格时，拾取将一直应用于mesh的三角形。相交三角形通过PickRecord类中的对象报告。`Picker, PickRecord`
- *Level of Detail.* 支持细节层次。目前我们只移植了野生魔术5的广告牌。下一步将移植其他详细级别的类。`BillboardNode`
- *Sorting*. 支持在场景图中排序。目前我们只移植了来自WildMagic5的BSP节点。下一步将移植其他排序类`BspNode`
- *Terrain.*环形环绕的分页地形系统。`Terrain`
- 网格工厂允许您创建简单的标准对象，包括矩形、磁盘、长方体、圆柱体、球体、托里体和柏拉图式实体。提供有关为对象创建的顶点缓冲区和索引缓冲区的信息，但必须填写除位置、法线和切线信息以外的所有信息。例如，如果使用颜色指定顶点缓冲区，则必须访问返回的可视对象的顶点缓冲区并自己填充颜色。`MeshFactory`

## Shading Language for Direct3D 10/11 (HLSL)

https://www.geometrictools.com/Source/HLSL.html

包含一组文件，用于编译和反映HLSL程序。该系统还生成了许多有用的信息，用于探索HLSL程序中的信息，包括查询HLSL结构和cbuffer在内存中的组织方式。HLSLFactory类是入口点，由上一个列表项中提到的着色器工厂使用。

## Direct3D 11

https://www.geometrictools.com/Source/Direct3D11.html

 DX11Engine类是使用Direct3D 11在GPU上进行图形和计算的接口。它封装了许多源代码，包括将Direct3D 11资源连接到独立于平台的资源的桥接模式。 

## Shading Language for OpenGL (GLSL)

https://www.geometrictools.com/Source/GLSL.html

包含一组文件，用于编译和反映GLSL程序。该系统还生成了大量有用的信息，用于探索GLSL程序中的信息，包括查询GLSL结构和制服在内存中的组织方式。GLSLProgramFactory类是入口点，由上一个列表项中提到的着色器工厂使用。

- GLSLProgramFactory是一个允许您编译和反映GLSL程序的系统。这里列出的类存储有关着色器中出现的对象的信息。ProgramDefines对象由应用程序用于指定GLSL程序中出现的条件定义，从而允许您控制着色器的编译方式。`ProgramDefines, GLSLProgramFactory, GLSLComputeProgram, GLSLVisualProgram, GLSLShader, GLSLReflection`

## OpenGL 4.5

https://www.geometrictools.com/Source/OpenGL4p5.html

GL45Engine类是使用opengl4.5或更高版本在GPU上进行图形和计算的接口。它封装了许多源代码，包括将OpenGL资源连接到独立于平台的资源的桥接模式。

- wrapper OpenGL的header和function calls：`GL45`
- GL45Engine类是图形显示和计算的顶级接口。Window类创建这样一个对象，以便在示例应用程序中使用。Classes GL45GraphicsObject是基于OpenGL的对象层次结构的基类，这些对象对应于以class GraphicsObject为基类的平台无关层次结构。`GL45Engine, GL45GraphicsObject`
- 特定于操作系统的引擎的包装器。`WGLEngine, WGLExtensions, GLXEngine, GLXExtensions`
- 就像GPU的类包装器是内存资源管理系统的类包装器一样。映射内存是在这个类中实现的，但是系统内存、暂存内存和GPU内存之间的拷贝依赖于资源类型，因此这些函数是纯虚拟的。`GL45Resource`
- 类来管理OpenGL缓冲区对象，这些对象对应于平台无关缓冲区对象的类。这两种类型的类对象使用桥接模式维护为成对的映射，`GL45Buffer, GL45AtomicCounterBuffer, GL45ConstantBuffer, GL45IndexBuffer, GL45StructuredBuffer, GL45VertexBuffer`
- 类来管理与平台无关的纹理对象的类相对应的OpenGL纹理对象。这两种类型的类对象使用桥接模式维护为成对的映射。`GL45Texture*, GL45DrawTarget`
- 类用来管理与平台无关的OpenGL状态对象，通过桥接模式实现。`GL45DrawingState, GL45BlendState, GL45DepthStencilState, GL45RasterizerState, GL45SamplerState`
- 输入布局界面描述如何组织顶点数据以供OpenGL使用。GL45InputLayout类是该对象的包装器，为活动VertexBuffer和可视化程序设置该类的管理器是GL45InputLayoutManager， `GL45InputLayout, GL45InputLayoutManager`

## 用于OpenGL和DirectX的基类有

`BaseEngine, GraphicsEngine, GraphicsObject, GEObject, GEDrawTarget, GEInputLayoutManager, Graphics`