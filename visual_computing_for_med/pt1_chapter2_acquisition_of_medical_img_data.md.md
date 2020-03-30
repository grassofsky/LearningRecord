+++
date="2020-01-02" 
title="visual computing for med reading - pt1 chapter 2 获取医学影像数据"
categories=["visual computing for med reading"] 

+++

#  visual computing for med - pt1 chapter 2 获取医学影像数据

## 2.1 简介

*如何选择成像模态？*

- 相关的解剖结构必须描述完整，
- 数据的分辨率足够以回答特定的诊断和治疗问题，
- 与对比度、信噪比和伪影有关的图像质量必须足以解释与诊断和治疗问题有关的数据，
- 应尽量减少对病人和医生的负担，
- cost需要做一定的限制

对于医学影像而言，最佳空间分辨率和最佳图像质量都不是临床实践中的相关目标。

## 2.2 医学影像数据

医学影像数据的形式，很简单，不用上图了，均匀分布的2D网格或3D网格。对应的像素（或体素）位于网格点上。**这里可能需要注意的是，网格的顶点对应像素值或体素，而不是网格的中心**。

然后给出了线性插值，双线性插值，三线性插值的公式。引出了triubic spline functions。

**TODO：add python code**

最后介绍了领域，6-neighborhood，以及26-neighborhood。

## 2.3 数据伪影和信号处理

*如何理解傅里叶变换？*

**TODO: 采样定律，fig2.5 2.6 重现**

```python
# ref: https://blog.csdn.net/qq_40587575/article/details/83316980

import numpy as np 
import matplotlib.pyplot as plt 

sampling_rate = 300    #采样率
fft_size = 300         #FFT长度
w = 100                 # 频率
t = np.arange(0, 1.0, 1.0/sampling_rate)
x = np.sin(2*np.pi*w*t)
xs = x[:fft_size]

xf = np.fft.rfft(xs) / fft_size  #返回fft_size/2+1 个频率

freqs = np.linspace(0, sampling_rate/2, int(fft_size/2+1))   #表示频率
xfp = np.abs(xf) * 2    #代表信号的幅值，即振幅

plt.figure(num='original', figsize=(15, 6))
plt.plot(x[:fft_size])

plt.figure(figsize=(8,4))
plt.subplot(211)
plt.plot(t[:fft_size], xs)
plt.xlabel(u"时间(秒)", fontproperties='FangSong')
plt.title(u"100Hz的波形和频谱", fontproperties='FangSong')

plt.subplot(212)
plt.plot(freqs, xfp)
plt.xlabel(u"频率(Hz)", fontproperties='FangSong')
plt.ylabel(u'幅值', fontproperties='FangSong')
plt.subplots_adjust(hspace=0.4)
plt.show()
```

*如何利用信号处理解释伪影？*

*什么是欠采样，什么是信号混叠，怎么办？*

*什么是插值伪影，怎么引起的？*

## 2.4 X-RAY成像

介绍了不同的X-RAY成像，包括胸片，mammograph，血管造影，数字体层摄影。

*什么是康普顿效应？*

*入射能量和出射能量之间的关系是什么？*

*X-RAY成像当前和未来的状态？*

## 2.5 计算机断层扫描成像

*CT影像单位HU介绍*

*CT成像的基本原理？*

*与CT扫描相关的参数有哪些？*

*CT成像伪影产生的原因？*

*CT影像常见的伪影有哪些？*

*CT成像当前和未来的状态？*

## 2.6 磁共振成像

*MRI成像的基本原理？*

*与MRI扫描相关的参数有哪些？*

*MRI影像的伪影？*

*MRI成像的常见分类？*

## 2.7 超声成像

*超声成像基本原理？*

## 2.8 核医学成像

*PET成像的基本原理？*

## 2.9 内窥成像

*内窥成像相比较其他成像方式的优点是什么？*

*内窥成像的常见分类有哪些？*





