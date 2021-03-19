# ARFoundation 获取深度图和RGB图像，python分析

## 获取深度图

关于深度图的获取参考了sample中的`arfoundation-samples-4.1\Assets\Scenes\Depth\DepthImages.unity`中的实现。

深度图获取的主要代码添加到了示例的`DisplayDepthImage.cs`脚本中，具体如下：

```c#
private byte[] duplicateTexture(Texture2D source)
{
    RenderTexture renderTex = RenderTexture.GetTemporary(
        source.width,
        source.height,
        0,
        RenderTextureFormat.RHalf,
        RenderTextureReadWrite.Linear);

    Graphics.Blit(source, renderTex);
    RenderTexture previous = RenderTexture.active;
    RenderTexture.active = renderTex;
    Texture2D readableText = new Texture2D(source.width, source.height, TextureFormat.RHalf, false);
    readableText.ReadPixels(new Rect(0, 0, renderTex.width, renderTex.height), 0, 0);
    readableText.Apply();
    //这里可以转 JPG PNG EXR  Unity都封装了固定的Api
    byte[] bytes = readableText.EncodeToEXR();
    RenderTexture.active = previous;
    RenderTexture.ReleaseTemporary(renderTex);
    return bytes;
}
```

**RenderTexture介绍参见：https://blog.csdn.net/weixin_39106746/article/details/105044892**

文件保存类似如下：

```c#
var bytes = duplicateTexture(envDepth);

string filePath = Application.persistentDataPath + "/depth-" 
    + bytes.Length.ToString() + "-"
    + m_iCount.ToString() + ".exr";

System.IO.File.WriteAllBytes(filePath, bytes);
```

## 利用python读取对应的exr文件

依赖的库，OpenEXR，Numpy，matplotlib。

OpenEXR安装：

- 从https://www.lfd.uci.edu/~gohlke/pythonlibs/#openexr 下载windows下对应的whl文件；
- `pip install ******.whl`进行安装；

OpenEXR文件读取可以参见：https://excamera.com/articles/26/doc/intro.html#interfacing-with-other-packages

```python
import OpenEXR, Imath, numpy
pt = Imath.PixelType(Imath.PixelType.FLOAT)
golden = OpenEXR.InputFile("GoldenGate.exr")
dw = golden.header()['dataWindow']
size = (dw.max.x - dw.min.x + 1, dw.max.y - dw.min.y + 1)
redstr = golden.channel('R', pt)
red = numpy.fromstring(redstr, dtype = numpy.float32)
red.shape = (size[1], size[0]) # Numpy arrays are (row, col)

import matplotlib.pyplot as plt
plt.matshow(red);
```

## 获取RGB图

参考了sample中的`arfoundation-samples-4.1\Assets\Scenes\CpuImages.unity`中的实现。主要的实现逻辑见：`arfoundation-samples-4.1\Assets\Scripts\CpuImageSample.cs`。主要代码如下：

```c#
unsafe void GetCPUImage()
{
    if (m_CameraManager.TryAcquireLatestCpuImage(out XRCpuImage cpuImage))
    {
        var format = TextureFormat.RGBA32;
        var cameraTexture = new Texture2D(cpuImage.width, cpuImage.height, format, false);
        var conversionParams = new XRCpuImage.ConversionParams(cpuImage, format, XRCpuImage.Transformation.MirrorY);
        var rawTextureData = cameraTexture.GetRawTextureData<byte>();
        try
        {
            cpuImage.Convert(conversionParams, new IntPtr(rawTextureData.GetUnsafePtr()), rawTextureData.Length);
        }
        finally
        {
            cpuImage.Dispose();
        }

        cameraTexture.Apply();
        var bytes = cameraTexture.EncodeToPNG();
        string filePath = Application.persistentDataPath + "/rgb-" 
            + bytes.Length.ToString() + "-"
            + m_iCount.ToString() + ".png";
        System.IO.File.WriteAllBytes(filePath, bytes);
    }
}
```

由于启用了unsafe，需要勾选project settings-player-other settings-allow ‘unsafe’ code。

## 待办项

TODO：深度图和RGB图，图像大小不一致。

