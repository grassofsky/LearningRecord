# AR Mobile学习记录

[toc]

## ARCore 学习记录

https://developers.google.cn/ar/discover?hl=zh-cn

ARCore 使用三个主要功能将虚拟内容与通过手机摄像头看到的现实世界整合：

- [**运动跟踪**](https://developers.google.cn/ar/discover/concepts?hl=zh-cn#motion_tracking)让手机可以理解和跟踪它相对于现实世界的位置。
- [**环境理解**](https://developers.google.cn/ar/discover/concepts?hl=zh-cn#environmental_understanding)让手机可以检测各类表面（例如地面、咖啡桌或墙壁等水平、垂直和倾斜表面）的大小和位置。
- [**光估测**](https://developers.google.cn/ar/discover/concepts?hl=zh-cn#light_estimation)让手机可以估测环境当前的光照条件

### ARCore的工作原理

从本质上讲，ARCore 在做两件事：在移动设备移动时**跟踪它的位置**和**构建自己对现实世界的理解**。

ARCore 的运动跟踪技术使用手机摄像头**标识兴趣点**（称为特征点），并跟踪这些点随着时间变化的移动。 将这些点的移动与手机惯性传感器的读数组合，ARCore 可以在手机移动时确定它的位置和屏幕方向。

除了标识关键点外，ARCore 还会检测**平坦的表面**（例如桌子或地面），并估测周围区域的平均光照强度。 这些功能共同让 ARCore 可以构建自己对周围世界的理解。

借助 ARCore 对现实世界的理解，您能够以一种与现实世界无缝整合的方式添加物体、注释或其他信息。 您可以将一只打盹的小猫放在咖啡桌的一角，或者利用艺术家的生平信息为一幅画添加注释。 运动跟踪意味着您可以移动和从任意角度查看这些物体，即使您转身离开房间，当您回来后，小猫或注释还会在您添加的地方。

### Unity开发入门指南

https://developers.google.cn/ar/reference/unity?hl=zh-cn

https://developers.google.cn/ar/develop/unity/quickstart-android

### 从API探测功能

- [DepthSensorUsageFilter](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/ARCoreCameraConfigFilter/DepthSensorUsageFilter?hl=zh-cn)：支持获取深度信息，如果有；
- [StereoCameraUsageFilter](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/ARCoreCameraConfigFilter/StereoCameraUsageFilter?hl=zh-cn)：支持使用stereo相机，如果有；
- [ARCoreRecordingConfig](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/ARCoreRecordingConfig?hl=zh-cn)：支持记录camera和sensor的数据；
- [Anchor](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/Anchor?hl=zh-cn)：支持将GameObject放到ARCore可追踪的地方；
- [AugmentedFace](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/AugmentedFace?hl=zh-cn)：ARCore能够检测、追踪的人脸；https://developers.google.cn/ar/develop/developer-guides/creating-assets-for-augmented-faces
- [AugmentedImage](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/AugmentedImage?hl=zh-cn)：ARCore能够检测、追踪的图像；
- [DetectedPlane](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/DetectedPlane?hl=zh-cn)：ARCore能够检测、追踪平面；
- [EnvironmentalLight](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/EnvironmentalLight?hl=zh-cn)：组件，用来自动调整光照信息；
- [FeaturePoint](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/FeaturePoint?hl=zh-cn)：能够追踪的现实世界的点；
- [PointCloud](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/Frame/PointCloud?hl=zh-cn)：获取当前frame的点云数据；
- [Trackable](https://developers.google.cn/ar/reference/unity/class/GoogleARCore/Trackable?hl=zh-cn)：ARCore现实世界中追踪的对象；
- [CameraImageBytes](https://developers.google.cn/ar/reference/unity/struct/GoogleARCore/CameraImageBytes?hl=zh-cn)：获取的图像，YUV-420-888 format；
- [CameraIntrinsics](https://developers.google.cn/ar/reference/unity/struct/GoogleARCore/CameraIntrinsics?hl=zh-cn)：ARCore相机intrinsics；

### ARCore Unity Demo开发

需要注意的有：

- gradle版本问题：https://developers.google.cn/ar/develop/unity/android-11-build?hl=zh-cn
- 对于unity 2019需要安装Multiplayer HLAPI， XR Legacy Input Helpers Packages，见：https://developers.google.cn/ar/develop/unity/quickstart-android

考虑到unity 2019的以上问题，安装unity 2020。

## ARKit 学习记录

https://developer.apple.com/cn/documentation/arkit/

### 总览

ARKit 结合运用设备运动跟踪、摄像头场景捕捉、高级场景处理和显示便捷性，简化了打造 AR 体验的工作。您可以使用这些技术，通过 iOS 设备的前置或后置摄像头打造各种不同的增强现实体验。

上述链接中将ARKit主题划分为如下：

- 基础知识：权限，选择摄像头，管理会话的声明周期；重点为：

  - [`ARAnchor (英文)`](https://developer.apple.com/documentation/arkit/aranchor)：描述感兴趣物体在物理环境中的位置和朝向；

- 相机：获取用户iOS设备的相关细节，如3D空间中的位置和方向，以及相机视屏数据，曝光度。重点为：

  - [`ARFrame (英文)`](https://developer.apple.com/documentation/arkit/arframe)：从设备相机中获取的视屏帧，能够访问到的主要数据有：
    - 相机的姿态，
    - 拍摄到的图像，
    - 光照估计，
    - 归一化的图像坐标系和屏幕图像坐标系之间的变换矩阵，
    - ARSession对应的世界坐标系中的点云，
    - 深度相关数据，
    - 场景中追踪到的anchors；
    - 人体检测；

  - [`ARCamera (英文)`](https://developer.apple.com/documentation/arkit/arcamera)
    - tracking status；
    - camera geometry；
    - 图像分辨率；
    - 2D camera plane到3D世界坐标系变换矩阵；
    - 投影矩阵；
    - 光照后处理；

- AR快速查看：在 app 或网站中增添 AR 体验的最简单方法

- 现实场景跟踪：通过跟踪表面、图像、物体、人物或用户面孔，打造后置摄像头增强现实体验。 

- 面部跟踪：	跟踪出现在前置摄像头馈送中的面孔。

- 人物：对 ARKit 在相机视频流中检测到的人物做出响应。

- 图像跟踪：识别物理环境中的图像，并跟踪他们的位置和方向；

- 物体检测：通过先使用app扫描，在运行时识别已知的对象；

- 方向跟踪，位置跟踪，渲染效果；			

- 多用户：与其他设备通信，打造共享的 AR 体验。；

- 音频，文本，自定显示；	

## AR Foundation

https://unity.com/unity/features/arfoundation

https://docs.unity3d.com/Packages/com.unity.xr.arfoundation@4.1/manual/index.html

https://blog.csdn.net/yolon3000/category_9023477.html

部分摘录：

不同平台支持的特性，这些特性基本上包括了移动端能够支持的功能项。

|                            | ARCore | ARKit | Magic Leap | HoloLens |
| -------------------------- | :----: | :---: | :--------: | :------: |
| Device tracking            |   ✓    |   ✓   |     ✓      |    ✓     |
| Plane tracking             |   ✓    |   ✓   |     ✓      |          |
| Point clouds               |   ✓    |   ✓   |            |          |
| Anchors                    |   ✓    |   ✓   |     ✓      |    ✓     |
| Light estimation           |   ✓    |   ✓   |            |          |
| Environment probes         |   ✓    |   ✓   |            |          |
| Face tracking              |   ✓    |   ✓   |            |          |
| 2D Image tracking          |   ✓    |   ✓   |     ✓      |          |
| 3D Object tracking         |        |   ✓   |            |          |
| Meshing                    |        |   ✓   |     ✓      |    ✓     |
| 2D & 3D body tracking      |        |   ✓   |            |          |
| Collaborative participants |        |   ✓   |            |          |
| Human segmentation         |        |   ✓   |            |          |
| Raycast                    |   ✓    |   ✓   |     ✓      |          |
| Pass-through video         |   ✓    |   ✓   |            |          |
| Session management         |   ✓    |   ✓   |     ✓      |    ✓     |
| Occlusion                  |   ✓    |   ✓   |            |          |

术语：

| **Term**          | **Description**                                              |
| ----------------- | ------------------------------------------------------------ |
| **Tracking**      | The AR device's ability to determine its relative position and  orientation in the physical world. If the environment is too dark, for  example, the device might "lose tracking", which means it can no longer  accurately report its position.<br />AR设备能够确定物理世界中对应的相对位置和方向。如果环境太黑，设备可能会lose tracking，意味不能准确的报告自己的位置。 |
| **Trackable**     | A real-world feature, such as a planar surface, that the AR device tracks and/or detects.<br />真实世界的特征，比如AR设备检测到的平面 |
| **Feature Point** | A specific point in a point cloud. An AR device uses the device’s  camera and image analysis to track specific points in the world, and  uses these points to build a map of its environment. These are usually  high-frequency elements, such as a knot in a wood-grain surface.<br />点云中的特征点。ar设备使用设备的相机和图像分析追踪世界中特定的点，使用这些点构建环境map。 |
| **Session**       | An AR instance. AR实例                                       |
| **Session Space** | The coordinate system relative to the beginning of the AR session.  For example, session space (0, 0, 0) refers to the position at which the AR session was created. An AR device typically reports trackables and  tracking information relative to its session origin.<br />相对于AR Session初始化时的坐标系。比如，创建时session空间的位置为（0,0,0）。AR设备会追踪和记录相对于初始位置的变换。 |

### AR Foundation Sample

https://github.com/Unity-Technologies/arfoundation-samples

生成的apk，安装到安卓手机后，提示“**需要安装面向AR的google play服务**”。网络上检索，安装后即可。

google play service for ar 见：https://github.com/google-ar/arcore-android-sdk/releases

在当前最新版本[v1.22.0](https://github.com/google-ar/arcore-android-sdk/releases/tag/v1.22.0)的knowissue上提到，**Support for stereo camera depth is expected to become available in ARCore SDK 1.23.0.**，支持深度相机信息的获取要等到下个版本。

### Depth maps with smartphone ar part

相关资料如下：

- [Easily Create Depth Maps with Smartphone AR (Part 1)](https://www.andreasjakl.com/easily-create-depth-maps-with-smartphone-ar-part-1/)
- [Understand and Apply Stereo Rectification for Depth Maps (Part 2)](https://www.andreasjakl.com/understand-and-apply-stereo-rectification-for-depth-maps-part-2/)
- [How to Apply Stereo Matching to Generate Depth Maps (Part 3)](https://www.andreasjakl.com/how-to-apply-stereo-matching-to-generate-depth-maps-part-3/)
- [Compare AR Foundation Depth Maps (Part 4)](https://www.andreasjakl.com/compare-ar-foundation-depth-maps-part-4/)
- [Visualize AR Depth Maps in Unity (Part 5)](https://www.andreasjakl.com/visualize-ar-depth-maps-in-unity-part-5/)

## WebXR

https://developer.mozilla.org/en-US/docs/Web/API/WebXR_Device_API

https://developer.mozilla.org/en-US/docs/Web/API/WebXR_Device_API/Fundamentals

在Fundamentals页面中提到：

> #### Augmented reality
>
> In augmented reality (AR), the user sees the imagery you render  presented on top of the physical, real-world environment around them.  Because AR is always an immersive experience, in which the scene is the  entire world around the user (rather than being enclosed in a box on a  screen), the only AR session mode is `immersive-ar`.
>
> **[The WebXR Augmented Reality Module](https://immersive-web.github.io/webxr-ar-module/) is still in a state of early development and is not yet stable enough  for regular use. We do not yet include documentation for it here on MDN, but it will come as soon as the module's specification settles down**.

说明了Augmented Reality模块仍然处于早期的开发阶段。

尝试了一下https://immersive-web.github.io/webxr-samples/上面的demo：

- Oppo Find X2 ：在Google Chrome_v88.0.4324.141版本中能够查看AR现象，但是过一会儿场景就消失了；Chrome 87.0.4280.66版本，无法查看AR场景；
- Iphone 12 Pro：WebXR Viewer能够很好的查看AR现象；
- IPhone 8： WebXR Viewer能够很好的查看AR现象；

因此关于WebXR的技术可以继续跟进，但在当前不会采用该技术项。

## Mobile相关其他内容

- 文件读写
  - https://docs.unity3d.com/ScriptReference/Application-persistentDataPath.html
  - https://blog.csdn.net/silk2018/article/details/72677136
  - **注意：需要添加权限**

- 日志查看：https://blog.csdn.net/yanhui2012/article/details/51586364
- Unity android真机调试：https://www.cnblogs.com/lonkiss/p/6599305.html



