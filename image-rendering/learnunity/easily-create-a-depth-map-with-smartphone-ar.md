# 简单的利用Smartphone的AR创建一个深度图

相关资料如下：

- [Easily Create Depth Maps with Smartphone AR (Part 1)](https://www.andreasjakl.com/easily-create-depth-maps-with-smartphone-ar-part-1/)
- [Understand and Apply Stereo Rectification for Depth Maps (Part 2)](https://www.andreasjakl.com/understand-and-apply-stereo-rectification-for-depth-maps-part-2/)
- [How to Apply Stereo Matching to Generate Depth Maps (Part 3)](https://www.andreasjakl.com/how-to-apply-stereo-matching-to-generate-depth-maps-part-3/)
- [Compare AR Foundation Depth Maps (Part 4)](https://www.andreasjakl.com/compare-ar-foundation-depth-maps-part-4/)
- [Visualize AR Depth Maps in Unity (Part 5)](https://www.andreasjakl.com/visualize-ar-depth-maps-in-unity-part-5/)

python demo地址：https://github.com/andijakl/python-depthmaps

unity完整的地址：https://github.com/andijakl/arfoundation-depth

## 获取深度图概览

方法描述：手机通过拍摄的视屏中当前帧的图像与之前的图像进行比较，估计出手机的变化。选择从不同的位置拍摄的包括相同场景的两张图，利用parallax effect（近的物体比远的物体运动的快，比如，靠近火车的树，比远离火车的山运动的快），算法可以计算出距离的变化。https://en.wikipedia.org/wiki/Parallax

## Research: Depth from Motion

> [1] J. Valentin *et al.*, “Depth from motion for smartphone AR,” *ACM Trans. Graph.*, vol. 37, no. 6, p. 193:1–193:19, Dec. 2018, doi: 10.1145/3272127.3275041.

主要流程如下：

1. **关键帧选择**：选择以前捕获的不同图像。 它们的相对6自由度（6DoF）运动是已知的。具体可以查看：[visual inertial odometry (VIO) / simultaneous localization and mapping (SLAM)](https://www.andreasjakl.com/basics-of-ar-slam-simultaneous-localization-and-mapping/)；
2. 预处理：关键点检测，关键点匹配，立体校正（Stereo rectification）：使图像彼此不失真并对齐；
3. **立体匹配**：在这些选定的图像之间创建视差/深度贴图；
4. **Depth map densification**，生成完整的map，将depth map和RGB图像进行匹配。

### 1. 选择关键帧

通常情况下：相机从选定的帧开始移动的次数越多，深度精度就越好——然而，这样做的代价是图像之间的时间差会变大，从而导致深度不一致。 

折中的办法是：

- frames之间基线的距离越大越好，至少4cm；
- 面积重叠：只有重叠部分会计算得到深度信息，至少40%重叠，同样，越大，越好；
- 错误： 如果6自由度对其运动估计的置信度较低，则不应选择图像；

### 2. 相机图像预处理

![](https://www.andreasjakl.com/wp-content/uploads/2020/12/05-AR-App-Development-Google-ARCore-Depth-01-1024x576.png)

用更专业的术语来说，这意味着经过立体校正后，所有的极线(*epipolar lines*)都平行于图像的水平轴。

为了实现立体校正，需要处理的任务有：

- 检测每个图像的关键点；
- 确定两个图像中都有的关键点，以及对应关系，计算reprojection matrices；
- 利用这些信息，我们可以将图像校正到一个共同的图像平面上。在两幅图像中，匹配的关键点位于同一水平极线上。这使得高效的像素/块比较能够为图像的所有区域（不仅仅是关键点！）计算视差图（=同一块在两个图像之间的偏移量）；

#### 2a) 检测关键点

理想的情况下需要对相机进行校正。相关检测的资料有：

- [tutorial that lists multiple ways of matching features between frames](https://docs.opencv.org/master/dc/dc3/tutorial_py_matcher.html).
-  [Simultaneous Localization and Mapping (SLAM) article series](https://www.andreasjakl.com/basics-of-ar-slam-simultaneous-localization-and-mapping/)
- [SIFT algorithm OpenCV implementation](https://docs.opencv.org/master/da/df5/tutorial_py_sift_intro.html).

结果如下：

![](https://www.andreasjakl.com/wp-content/uploads/2020/11/sift_keypoints.png)

#### 2b) 关键点匹配

一个有效的方法是： [FLANN matcher](https://github.com/mariusmuja/flann).。依据：`D. G. Lowe, “Distinctive Image Features from Scale-Invariant Keypoints,” *International Journal of Computer Vision*, vol. 60, no. 2, pp. 91–110, Nov. 2004, doi: 10.1023/B:VISI.0000029664.99615.94.`，选择合适的点。

结果如下所示：

![](https://www.andreasjakl.com/wp-content/uploads/2020/11/keypoint_matches-1024x384.png)

#### 3c) 立体校正

相关资料：

- [fundamental matrix](https://en.wikipedia.org/wiki/Fundamental_matrix_(computer_vision)) 
- [“Computer Vision” course of the Carnegie Mellon University](https://www.cs.cmu.edu/~16385/) 
- [normalized cross correlation (NCC)](https://en.wikipedia.org/wiki/Cross-correlation#Normalized_cross-correlation_(NCC))
- [sum of absolute differences (SAD)](https://en.wikipedia.org/wiki/Sum_of_absolute_differences)

![](https://www.andreasjakl.com/wp-content/uploads/2020/11/epipolar_geometry_1-1024x576.png)

![](https://www.andreasjakl.com/wp-content/uploads/2020/11/epipolar_geometry_2.png)

极线约束可以用来帮助立体校正，如下：

![](https://www.andreasjakl.com/wp-content/uploads/2020/11/epipolar_constraint-1024x576.png)

