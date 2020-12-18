# VTK - Blogs in 2020 导读

kitwareblog上面带[vtk的blog](https://blog.kitware.com/tag/vtk/)，2020年更新了10篇文章，更新时间从新到旧排列：

- [Looking Glass Factory Expands Reach into R&D Labs with New Holographic Kitware Integrations](https://blog.kitware.com/looking-glass-factory-expands-reach-into-rd-labs-with-new-holographic-kitware-integrations/)：寻找合作，**在全息界面上的应用**。
- [Looking Glass Factory and Kitware: Desktop Holographic Displays Driven by VTK, ParaView, and 3D Slicer](https://blog.kitware.com/vtk-holographic-display/)：Kitware与全息界面领域的全球领导者Looking Glass Factory（LGF）合作，从任何基于VTK的应用程序（包括ParaView、3D切片器和任何自定义应用程序）中添加对LGF全息显示的支持。**在全息界面上的应用**。
- [Integrating ITK and VTK: Give Us Your Feedback!](https://blog.kitware.com/itk-plus-vtk/)：考虑到vtk和itk通常会结合到一起使用，但是两者之间的数据格式，以及坐标系定义是不一致的，此处征集反馈，**后期是不是需要提供enhanced version，将ITK和VTK结合到一起**。
- [F3D v1.0, a fast and minimalist 3D viewer, has been released !](https://blog.kitware.com/f3d-v1-0-a-fast-and-minimalist-3d-viewer-has-been-released/)：基于VTK构建的极简3Dviewer。方便用来快速查看硬盘中的数据集。
- [PBR Journey Part 1: High Dynamic Range Image Based Lighting with VTK](https://blog.kitware.com/pbrj1/) ：**支持载入 High Dynamic Range (.hdr file) 文件**。
- [VTK 9.0.0 available for download](https://blog.kitware.com/vtk-9-0-0-available-for-download/)：发布新版本主要更新点有（摘录部分）：
  - 支持vtkPolyDataMapper构建的**surface的PBR渲染**（Physically Based Rendering）；
  - 添加新的renderpass，vtkSSAOPass ，采用了**模拟环境遮挡的屏幕空间技术**；增强了物体的空间深度感知；
  - vtkOpenGLFluidMapper 提供了一种新的mapper用于**粒子流动数据模拟的实时渲染**；
  - vtkVtkJSSceneGraphSerializer 提供了一种新的场景遍历算法，可以将**vtk场景转换成vtk.js支持的场景**；
- [Implementation of Rational Bézier Cells into VTK](https://blog.kitware.com/implementation-of-rational-bezier-cells-into-vtk/)：原先对于形状表达的时候，采用的是 Lagrange polynomials基函数，而在CAD软件中，通常会用Bernstein-Bezier 基函数来进行建模，**作为往CAD方向放的进一步推力**。
- [VTK for Climate Science](https://blog.kitware.com/vtk-for-climate-science/)：**介绍了VTK在气候科学上的推力**，主要提供了新的功能如下：
- 导出到pdf或svg格式；
  - 2D图中的等距流线绘制；
- 为了方便结果出刊打印，提供了vtkCookieCutter ，用来将vtkPolyData拆解成特殊多边形的组合；
  - 带标签的等高线；
- 支持将vtkPolyData绘制到Context2D；
