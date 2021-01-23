# 通过Hololens 2采集网格数据

https://forums.hololens.com/discussion/9827/how-to-detect-a-real-object-using-hololens-for-unity

需要包括的内容有：

- 实现hololens通过spatial mapping采集对应的网格数据；
- 将网格数据存储到Hololens2的内部目录；

## 实现通过Hololens 2采集网格数据

帮助文档有：

- [空间感知系统](https://hololenscndev.github.io/MRTKDoc/Documentation/SpatialAwareness/SpatialAwarenessGettingStarted.html)
- [为设备设置observers](https://hololenscndev.github.io/MRTKDoc/Documentation/SpatialAwareness/ConfiguringSpatialAwarenessMeshObserver.html)
- [通过代码控制Observers](https://hololenscndev.github.io/MRTKDoc/Documentation/SpatialAwareness/UsageGuide.html)

## 将网格数据存储到Hololens2的内部目录

文件读写参见：

- http://longqian.me/2017/02/08/hololens-file-transfer/
- https://blog.csdn.net/qq_41452267/article/details/109029998
- https://www.cnblogs.com/bwzydr/p/6858370.html

## 利用research mode

https://github.com/microsoft/HoloLens2ForCV

### 背景知识 - 坐标系

**静止的参考坐标系**

应用启动之后物体相对现实世界的位置确定了，不会随着人的移动而移动。如果你在HoloLens上的一个应用程序的静止参考系中渲染全息图，其中用户在大约5米宽的区域之外漫游，你的应用程序可能会在这些全息图的观察位置观察到漂移。如果你的体验中有超过5米的用户漫游，那么你正在构建一个世界级的体验，这将需要其他技术来保持全息图的稳定，如下所述。

另一的相对的坐标系，就是相对于hololens，会随hololens移动而移动的坐标系，称为**Attached frame of reference**



Photo capture：https://github.com/microsoft/MixedRealityToolkit-Unity/issues/6313

Unity预编译宏：https://blog.csdn.net/ZFSR05255134/article/details/86527116