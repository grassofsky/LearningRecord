# 《Unity Shader编程》阅读笔记

## 渲染管线

![](./image/render_pipeline.png)



1. ⽤户给 GPU 喂顶点数据给流⽔线，顶点数据是⼀组属性，诸如位置、法线、颜⾊之类的组合信息。然后通过 API(所谓的 drawcall)让 GPU 渲染; 
2. 上图中的 Vertex Shader 接⼿处理每个顶点，对 Vertex Shader 来说，得到的输⼊就是第⼀步给到的信息流中的每个顶点属性；⾄于输出什么，在可编程管线中，完全是看⽤户写的代码要做什么。当然，这个步骤输出的信息有⼀定约束，⾄少有⼀个“位置(position)”信息必须被输出，作为⼀个有效的顶点信息
3. 元件组装（Primitive Assembly），对于 OpenGL 来说，元件有 3 种，点、线和三⾓形。从第⼆步出来的顶点数据，再结合⽤户喂给流⽔线的其他数据（告知这些顶点之间的关系），会被这个过程组装成这三个类型中的⼀种。镶嵌/或者说是细分（Tessellation)会在此后运⾏，让前⾯的primitive 能被细分成更⼩的元件以满⾜不同的精度需求
4. ⼏何体着⾊器（Geometry Shader)，该着⾊器得到第三步出来的 primitive，它的输出是 0 个或者更多的 primitives
5. Transform Feedback，在 OpenGL 这⾥，⼏何体着⾊器后可以吐出相关的 primitive到所谓的缓冲物体(buffer object)中（其实就是定义好格式的⼀些数据结构），这些buffer object 可以被保存⽤作其他的作⽤。如果在这⾥抛弃流⽔线后续的⼯作，那么整个流⽔线的产出就是这些东西
6. 顺图中的实线，后续是裁剪（Clipping）。根据⽤户设置的视椎体的可视范围，primitives会被剔除（整体在可视范围外），部分在外的则被按照规则切割成⼏个 primitives。在现在的 GPU 上，⽤户除了视椎体，也可以通过裁剪⾯（Clip plane）来指定额外的裁剪信息。在 3D 中，⼤多数是单⾯的 primitive，这个时候，背⾯朝向相机的primitive 也会被裁剪掉。
7. 能达到光栅化（Rasterization）这步的物体，会被这个步骤进⾏光栅化，举例来说，⼀个三⾓形 primitive，会产⽣填充其内部的许多所谓的碎⽚（fragment）。fragment跟 pixel（像素）不同的地⽅在于，在专业⾓度，它是⼀系列信息，⽤来判断对应的像素最后的数据。
8. 碎⽚着⾊器（Fragment Shader）得到上⾯处理出来的数据，然后运⾏⽤户给定的⼀段程序，计算出最后的像素的相关信息。它的输出是⼀系列的值：颜⾊和深度信息
9. fragment shader 的输出进⼊ Fragment Tests 这个阶段，会⾸先进⾏ Alpha 测试（alpha test），继⽽模板测试（stencil test），如果 fragment 没有经过测试，那么就不会进⼊到最后的缓冲区（framebuffer）；然后进⾏深度测试（depth test），如果通不过，就不会进⼊ framebuffer。【注：OpenGL 的相关⽂档有提到，如果 fragment shader对 fragment 的深度不会修改，那么这个测试可以提到 fragment shader 之前，所谓的“Early Z Test”】
10. Blending 是后续的⼯作，如果 fragment 可以被写⼊ framebuffer，则这个 fragment和已经在 framebuffer 中的值可以做⼀定的混合，实现不同的效果，举例，半透明效果。
11. 上⼀步出来的值，就准备开始往 framebuffer 中写了，写的时候可以通过 Writing Mask来控制哪些值要写到 framebuffer，可以控制到颜⾊、深度、模板值，也可以控制到颜⾊的某个通道
12. 结束。

