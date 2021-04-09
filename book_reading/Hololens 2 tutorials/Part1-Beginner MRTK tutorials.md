# Part1 - beginner MRTK tutorials

## 2. Initializing your project and first application

[Initializing your project and deploying your first application - Mixed Reality | Microsoft Docs](https://docs.microsoft.com/en-us/windows/mixed-reality/develop/unity/tutorials/mr-learning-base-02?tabs=winxr)

整个流程如下图所示

![MRTK](https://docs.microsoft.com/en-us/windows/mixed-reality/develop/images/unity_mrtk_mrft_flow.png)

对上面步骤的补充有：

- 第2步之前，还有，
  - Switching the build platform
  - Importing the TextMeshPro Essential Resources
- 第三步之前，还有，
  - Configuring the Unity project：project-setting > xr plugin management > install  xr plugin management; project setting > player > publishing settings > package name

为了抓住物体或和物体进行交互，对象必须要有：**Collider component**（比如，**Box Collider**），**Object Manipulator(Script)**组件，以及**NearInteractionGrabble(Script)**组件。

# 3. Configuring the MRTK profiles

[Getting started tutorials - 3. Configuring the MRTK profiles - Mixed Reality | Microsoft Docs](https://docs.microsoft.com/en-us/windows/mixed-reality/develop/unity/tutorials/mr-learning-base-03?tabs=winxr)

用于隐藏spatial awareness mesh的主要步骤 ：

1. Clone the default Configuration Profile
2. Enable the Spatial Awareness System
3. Clone the default Spatial Awareness System Profile
4. Clone the default Spatial Awareness Mesh Observer Profile
5. Change the visibility of the spatial awareness mesh

## 4. Positioning objects in the scene

[MRTK tutorials - 4. Positioning objects in the scene - Mixed Reality | Microsoft Docs](https://docs.microsoft.com/en-us/windows/mixed-reality/develop/unity/tutorials/mr-learning-base-04)

学习了如何相对于用户定位场景中的对象，并使用MRTK的GridObjectCollection来组织集合中的对象

其中关于GridObjectCollection组件可以参见：[ObjectCollection | Microsoft Docs](https://docs.microsoft.com/zh-cn/windows/mixed-reality/mrtk-unity/features/ux-building-blocks/object-collection)

## 5. Creating dynamic content using Solvers

[Creating dynamic content using Solvers - Mixed Reality | Microsoft Docs](https://docs.microsoft.com/en-us/windows/mixed-reality/develop/unity/tutorials/mr-learning-base-05)

在本教程中，您将探索使用MRTK可用的放置工具（称为Solvers）动态放置全息图的方法，以解决复杂的空间放置场景。在MRTK中，Solvers是一个脚本和行为系统，用于允许对象跟随您、用户或场景中的其他游戏对象。他们也可以用来捕捉到某些位置，使你的应用程序更直观。

Solver的介绍参见： [Solvers](https://docs.microsoft.com/en-us/windows/mixed-reality/mrtk-unity/features/ux-building-blocks/solvers/solver.md) 

此处主要介绍Directional Indicator Solver （ackages > Mixed Reality Toolkit Foundation > SDK > Experimental > Features > Utilities）和tap to place solver（**Packages** > **Mixed Reality Toolkit Foundation** > **SDK** > **Features** > **Utilities** > **Solvers**）。

效果是当远离RoverExplorer的时候，会出现箭头指示。

![Unity Play mode split view with DirectionalIndicator solver in use](https://docs.microsoft.com/en-us/windows/mixed-reality/develop/unity/tutorials/images/mr-learning-base/base-05-section2-step1-4.png)