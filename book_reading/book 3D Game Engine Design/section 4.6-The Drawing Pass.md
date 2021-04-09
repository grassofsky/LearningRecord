# Chapter 4 Scene Graphs

## 4.6 The drawing pass

culler得到的潜在可见的对象会传递给renderer用于绘制。

```c++
// *** application initialization code ***

// creation of objects
create Culler m_kCuller;
create Camera m_spCamera;
create scene m_spKScene;

// initial update of objects
m_spkScene->UpdateGS();
m_spkScene->UpdateRS();

// initial culling scene
m_kCuller.SetCamera(m_spkCamera);
m_kCuller.ComputeVisibleSet(m_spkScene);

// *** in the idle loop ***
if (MoveCamera())
{
    m_kCuller.ComputeVisibleSet(m_spkScene);
}

if (MoveObject())
{
    m_spkScene->UpdateGS();
    m_kCuller.ComputeVisibleSet(m_spkScene);
}

m_pkRenderer->ClearBuffers();
if (m_pkRenderer->BeginScene())
{
    m_pkRenderer->DrawScene(m_kCuller.GetVisibleSet());
    m_pkRenderer->EndScene();
}
m_pkRenderer->DisplayBackBuffer();
```

cg shader program位于：`GeometricTools/WildMagic4/Data/ShaderPrograms/Cg`

编译完的shader program位于`GeometricTools/WildMagic4/Data/Wmsp`

### 4.6.1 Single-pass drawing

一个single pass的示例是TextureEffect，构造函数如下：

```c++
TextureEffect::TextureEffect(const std::string& rkBaseName)
    : ShaderEffect(1) // single pass
{
    m_kVShader[0] = WM4_NEW VertexShader("Texture");
	m_kPShader[0] = WM4_NEW PixelShader("Texture");
    m_kPShader[0]->SetTextureQuantity(1);
    m_kPShader[0]->SetImageName(0, rkBaseName);
}
```

创建了vertex和pixel shader。创建包含单个三角形的场景的基本代码如下：

```c++
// The root node of the scene. NOTE: An application can draw
// multiple scenes during a single iteration through the idle
// loop. It is not necessary that everything in the world be 
// placed in one scene graph.
m_spScene = WM4_NEW Node;

// Create a single triangle whose vertices have positions and 
// 2D texture coordinates (in unit 0).
Attributes kAttr;
kAttr.SetPChannels(3);
kAttr.SetTChannels(0,2);
VertexBuffer* pkVBuffer = WM4_NEW VertexBuffer(kAttr,3);
pkVBuffer->Position3(0) = Vector3f(1.0f,0.0f,0.0f);
pkVBuffer->Position3(1) = Vector3f(0.0f,1.0f,0.0f);
pkVBuffer->Position3(2) = Vector3f(0.0f,0.0f,1.0f);
pkVBuffer->TCoord2(0,0) = Vector2f(0.0f,0.0f);
pkVBuffer->TCoord2(0,1) = Vector2f(1.0f,0.0f);
pkVBuffer->TCoord2(0,2) = Vector2f(0.0f,1.0f);
IndexBuffer* pkIBuffer = WM4_NEW IndexBuffer(3);
int* aiIndex = pkIBuffer->GetData();
aiIndex[0] = 0; aiIndex[1] = 1; aiIndex[2] = 2;
TriMesh* pkTriangle = WM4_NEW TriMesh(pkVBuffer,pkIBuffer);
m_spkScene->AttachChild(pkTriangle);

// Create a single-pass texture effect.
Effect* pkEffect = WM4_NEW TextureEffect("MyImage");

// Attach the effect to the geometric primitive.
pkTriangle->AttachEffect(pkEffect);
```

TODO: