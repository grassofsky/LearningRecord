# Chapter 3 Renderers

## 3.3 An Abstract Rendering API

这一节用来介绍`Renderer`抽象基类函数，扮演了用于绘制对象的资源管理者。

### 3.3.1 Construction and destruction

该抽象类的构造函数是protected的。相关的函数有：

```c++
Renderer (
    FrameBuffer::FormatType eFormat,
    FrameBuffer::DepthType eDepth,
    FrameBuffer::StencilType eStencil,
    FrameBuffer::BufferingType eBuffering,
    FrameBuffer::MultisamplingType eMultisampling,
    int iWidth, int iHeight);
virtual ~Renderer ();
FrameBuffer::FormatType GetFormatType () const;
FrameBuffer::DepthType GetDepthType () const;
FrameBuffer::StencilType GetStencilType () const;
FrameBuffer::BufferingType GetBufferingType () const;
FrameBuffer::MultisamplingType GetMultisamplingType () const;
int GetWidth () const;
int GetHeight () const;
enum
{
    OPENGL,
    DIRECTX,
    SOFTWARE,
    MAX_RENDERER_TYPES
};
virtual int GetType () const = 0;
```

### 3.3.2 Camera management

用来指定空间中的哪些区域需要参与绘制。相关的接口有：

```c++
void SetCamera (Camera* pkCamera);
Camera* GetCamera () const;
virtual void OnFrameChange ();
virtual void OnFrustumChange ();
virtual void OnViewportChange () = 0;
```

### 3.3.3 Global-state management

renderer维护了当前激活的全局状态，包括：alpha blending，culling，fog，materials，polygon offset（how to deal with depth aliasing problems），stenciling，wireframe，and depth buffering。相关接口有：

```c++
virtual void SetAlphaState (AlphaState* pkState);
virtual void SetCullState (CullState* pkState);
virtual void SetFogState (FogState* pkState);
virtual void SetMaterialState (MaterialState* pkState);
virtual void SetPolygonOffsetState (PolygonOffsetState* pkState);
virtual void SetStencilState (StencilState* pkState);
virtual void SetWireframeState (WireframeState* pkState);
virtual void SetZBufferState (ZBufferState* pkState);
AlphaState* GetAlphaState ();
CullState* GetCullState ();
FogState* GetFogState ();
MaterialState* GetMaterialState ();
PolygonOffsetState* GetPolygonOffsetState ();
StencilState* GetStencilState ();
WireframeState* GetWireframeState ();
ZBufferState* GetZBufferState ();
void SetReverseCullFace (bool bReverseCullFace);
bool GetReverseCullFace () const;

void SetGlobalState (GlobalStatePtr aspkState[]);
void RestoreGlobalState (GlobalStatePtr aspkState[]);

void SetLight (int i, Light* pkLight);
Light* GetLight (int i);
```

### 3.3.4 Buffer clearing

在绘制对象之前，组成帧缓冲区的各种缓冲区可能必须被清除，相关接口如下：

```c++
virtual void SetBackgroundColor (const ColorRGBA& rkColor);
const ColorRGBA& GetBackgroundColor () const;
virtual void ClearBackBuffer () = 0;
virtual void ClearZBuffer () = 0;
virtual void ClearStencilBuffer () = 0;
virtual void ClearBuffers () = 0;
virtual void DisplayBackBuffer () = 0;
virtual void ClearBackBuffer (int iXPos, int iYPos, int iWidth,
int iHeight) = 0;
virtual void ClearZBuffer (int iXPos, int iYPos, int iWidth,
int iHeight) = 0;
virtual void ClearStencilBuffer (int iXPos, int iYPos, int iWidth,
int iHeight) = 0;
virtual void ClearBuffers (int iXPos, int iYPos, int iWidth,
int iHeight) = 0;
```

操作的常规流程为：

```c++
clear all buffers;
render the scene;
display back buffer;
```

但是，这不是必需的。例如，如果您有一个天空圆顶，其内容填满了窗口，然后在天空圆顶渲染后渲染场景对象，则无需清除后缓冲区。您可能还有一个带有预渲染静态几何体的级别，该级别同时生成颜色缓冲区和深度缓冲区。其目的是从磁盘加载这些缓冲区，并通过每个帧的图形API调用将其写入硬件。然后渲染其他（动态）对象。在这种情况下，不需要清除后缓冲区或深度缓冲区。

### 3.3.5 Object Drawing

**TODO**



