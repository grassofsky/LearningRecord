# PaperRead - Recent Development of Augmented Reality in Surgery: A Review

> Vávra P,  Roman J ,  Zon?A P , et al. Recent Development of Augmented Reality in Surgery: A Review[J]. Journal of Healthcare Engineering, 2017,  2017:1-9.
>
> https://www.hindawi.com/journals/jhe/2017/4574172/

主要集中在结果与讨论的小结。

## 3. Result and Discussion

### 3.1 增强现实的基本原理

![](https://static-01.hindawi.com/articles/jhe/volume-2017/4574172/figures/4574172.fig.001.svgz)

AR的主要优点是，与普通的可视化技术相比，外科医生不必被迫将视线从手术部位移开。

图中主要部件有：

- 追踪器，Tracker，用来确定不同对象的位置关系；
- video camera，获取看到的外界现实；
- computer processing，计算看到的现实，和虚拟对象之间的关系，创建显示结果；
- Electronic display，显示增强现实的呈现结果；

另一种形式是head-mounted display（smart glasses），带有特殊的projector，head tracking，深度相机。案例：

- [Augmented reality–guided neurosurgery: accuracy and intraoperative application of an image projection technique](https://thejns.org/view/journals/j-neurosurg/123/1/article-p206.xml)，
- [Augmented reality as an aid in maxillofacial surgery: Validation of a wearable system allowing maxillary repositioning](https://www.sciencedirect.com/science/article/abs/pii/S1010518214002522?via%3Dihub)

目前，由于医学影像术前三维重建的必要性，限制了AR的应用。这样的重建可以用于对靶区进行虚拟探测，预先规划有效的手术入路，并在手术区域内更好地定位和导航。

AR在显示重要结构如主要血管、神经或其他重要组织时特别有用。通过将这些结构直接投射到病人身上，增强了安全性，减少了完成手术所需的时间。AR的另一个有用特性是能够控制显示对象的不透明度。*还强调了声控，和手势的作用*。

### 3.2. 手术区域监控

大多数手术的目标是可变形结构，在手术过程中这些结构会发生显著变化（如，切除术在切除过程中切除组织）。利用超声，CT，或MRI来更新手术区域模型，会需要大量的时间。CT使用次数受限。自动医疗重建往往包括许多不同的结构，这使得定位困难，尤其是在腹部手术中。  Sugimoto et al. （https://doi.org/10.1007/s00534-009-0199-y）二氧化碳与静脉造影剂一起被引入胃肠道和胰胆管，这样可以更好地显示这些单独的结构 。**这一块的技术有待提升**。

### 3.3. Augmented Reality for Education and Cooperation

AR使有经验的外科医生能够通过互联网连接远程协助住院医师，从而为优秀的远程教学开辟了道路。具体参见：

> M. B. Shenai, M. Dillavou, C. Shum et al., “Virtual interactive presence and augmented reality (VIPAR) for remote surgical  assistance,” *Neurosurgery*, vol. 68, p. 8, 2011. 
>
> M. C. Davis, D. D. Can, J. Pindrik, B. G. Rocque,  and J. M. Johnston, “Virtual interactive presence in global surgical  education: international collaboration through augmented reality,” *World Neurosurgery*, vol. 86, pp. 103–111, 2016. 

### 3.4. Methods of Image Alignment

真实环境和CG图像之间的配准。

最简单的方式就是进行手动对齐（缓慢，不精确）。术前图像和当前治疗患者之间的位置关系的同步，需要在变化后连续进行（如呼吸过程中，器官布局的变化）。准确配准的方法可以通过一组跟踪器实现，这些跟踪器用来确定病人身体的确切位置。这些跟踪器通常跟踪放置在特定结构表面的基准标记，这些标记在手术期间保持静止（例如髂骨嵴、锁骨等），从而为系统提供参考点。

**文献中描述了不同类型的marker。**

optical markers：https://doi.org/10.1007/s00464-014-3433-x，https://doi.org/10.2176/nmc.tn.2014-0278等；

红外标记，infrared markers：https://doi.org/10.1007/s11548-011-0660-7，https://doi.org/10.1055/s-0032-1333415等；

荧光标记，fluorescent markers：https://doi.org/10.1007/s11548-016-1385-4；

magnetic tracking，https://www.sciencedirect.com/science/article/abs/pii/S0531513105004929

passive coordinate measurement arm，https://doi.org/10.1109/TMI.2007.911003

另一种配准方法是跟踪腹腔镜摄像机，然后通过三维重建自动对准捕获的视频，https://doi.org/10.1016/j.jamcollsurg.2016.06.392

用于AR的商用系统，大多基于infrared or electromagnetic tracking，https://doi.org/10.1016/j.suronc.2011.07.002

**也可以采用没有marker的技术：**

- 基于物理的配准变形模型，https://doi.org/10.3233/978-1-61499-209-7-182；
- 利用高精度立体相机，https://doi.org/10.1007/s00464-012-2355-8
- 激光表面扫描技术，https://doi.org/10.3171/jns.2007.106.4.704；

AR-related 配准时间在6-8 min。（https://doi.org/10.1007/s00423-014-1256-9）

### 3.5. Precision

光学系统在一些研究中的精度在5mm以内（可以用于临床应用）。

The PCMA method精度最高，<1mm。（https://doi.org/10.1002/rcs.1513）

一些研究在使用head-mounted display获得<2.7 mm的精度。（https://doi.org/10.1016/j.jcms.2014.09.001，https://doi.org/10.1007/s11548-013-0912-9，https://doi.org/10.1007/s11548-014-1005-0）。

### 3.6. Uses in Clinical Practice

增强现实技术可以有效地用于术前计划和及时完成实际手术。

通常情况下，AR用于切割平面的选择（https://doi.org/10.1007/s11548-013-0828-4，https://doi.org/10.3171/2014.9.JNS141001），针放置的最佳位置（https://doi.org/10.1007/s00534-011-0385-6），通过显示主要器官部件的位置来提高安全性（https://doi.org/10.1007/s00423-014-1256-9），AR的另一个好处是能够在新辅助化疗或放疗后帮助外科医生[64]。AR可用于设想和优化切除手术体积[16]。在许多手术中，AR辅助手术可与其他辅助方法相媲美，此类设备的使用取决于外科医生的偏好[58]。

最适合采用AR技术的是，几乎没有运动和变形的器官手术（如头骨，大脑和胰腺），因为需要最少的跟踪和处理能力，而移动器官，如肠，是明显更复杂的跟踪和显示。

由于头部和大脑运动的固有局限性，**神经外科手术成功地应用了AR系统**。据报道，AR在16.7%的神经血管外科手术中有重要影响[34]，与传统的2D方法相比，AR可以提高病变的精确定位率，缩短手术时间[66]。神经外科医生主要受益于对单个脑回、血管、重要神经束的精确定位，以及规划手术通道的可能性，例如，在切除浅表肿瘤[10,66]、癫痫手术[65]或神经血管外科[80]中。

**AR在骨科手术和重建过程中也被证明是有用的**，特别是因为它可以直接在患者身体上方观看重建，这减少了因观看外部显示器而引起的注意力分散。

 在腹部手术中使用AR更困难，因为器官的运动量非常大；然而，由于这些器官相对静止的性质，目前在肝脏和胰腺手术中使用AR，因为它可以更好地投射大血管或肿瘤部位[39]。 通过将重建数据与术中超声进行对比，**AR可用于肝切除术中的术中指导**（https://doi.org/10.1007/s00268-015-3229-8）。

### 3.7. Advanced Image in Fusion Using Augmented Reality

光学图像与荧光图像结合等。

### 3.8. Problems of Augmented Reality

增强现实为外科学带来了许多新的可能性和新的维度。外科医生可以使用额外的数据来做决定，提高安全性和有效性。技术的进步使AR设备能够以更高的精确度和更低的延迟来显示信息。尽管不断改进，仍有一些困难需要解决。目前需要用功能强大的计算机进行图像重建。然而，由于预期的技术进步，实时获取高分辨率医学扫描和三维重建可能成为可能。这样的手术重建将大大提高整个视野的准确性。尽管AR可以加快手术过程，但准备整个系统并进行必要的登记和测量通常会增加完成手术所需的时间，时间量取决于手术类型和AR系统的复杂性。采用全自动系统将消除这一问题，并减少完成工作所需的总时间。

 在使用任何形式的AR时，完成一个程序所需的时间已经减少；但是，也有某些限制。其中之一是不注意性失明（一种外科医生没有看到突然出现在视野中的意外物体的事件），这是一个值得关注的问题，需要在使用3D覆盖时加以解决[101]。外科医生在手术过程中通过AR呈现的信息量正在增加，可能会分散注意力[60]。因此，有必要只显示重要的数据，或者提供一种根据需要在不同信息集之间切换的方法。Hansen等人提出了一种降低认知需求的方法。通过优化单个结构的空间关系，将AR引起的遮挡降低到最小，并最大化对比度，外科医生能够在某些情况下减少视觉混乱[102]。在直接投射到器官上时，达到足够的图像对比度也是必要的[13]。创建正确的三维和深度感知的困难也依然存在。整个系统的潜伏期也值得关注，因为过多的潜伏期可能会降低精确度，降低外科医生的舒适度。Kang等人测量了腹腔镜手术的光学系统的潜伏期为144±19毫秒[15]。 

目前使用的头戴式显示器通常重达几百克，并产生大量热量；因此，长期佩戴舒适性是一个问题。这些都需要在未来解决，以更好地符合人体工程学，并允许长期连续使用。众所周知，头盔显示器中的虚拟和增强现实投影会产生模拟病，在最坏的情况下会表现为恶心、头痛、眩晕或呕吐。模拟器疾病背后的确切原因尚不清楚；然而，视觉、本体感觉和前庭输入之间可能存在差异。

