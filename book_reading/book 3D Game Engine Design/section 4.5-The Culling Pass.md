# Chapter 4 Scene Graphs

## 4.5 The culling pass

在第4版本之前，culling和drawing pass是一起的。如果发现一个节点的包围盒不可见，那么会裁减掉该节点，如果遍历中发现某个叶子节点可见，那么会立刻调用draw进行绘制。在第4版本，首先进行culling pass，然后再进行drawing pass。针对裁剪剔除，引入了新类：`Culler, VisibleObject, VisibleSet`。

```c++
class VisibleObject
{
public:
    Spatial* object;
    Effect* GlobalEffect;
};
```

VisibleSet是VisibleObject的容器，VisibleObject是包围盒没有在Frustum外部的object（可能可见的对象）。

```c++
class VisibleSet
{
public:
    VisibleSet(int iMaxQuantity = 0, int iGrowBy = 0);
    virtual ~VisibleSet();
    
    int GetQuantity() const;
    VisibleObject* GetVisible();
    VisibleObject& GetVisible(int i);
    
    void Insert(Spatial* pkObject, Effect* pkGlobalEffect);
    void Clear();
    void Resize(int iMaxQuantity, int iGrowBy);
    
private:
    enum
    {
        VS_DEFAULT_MAX_QUANTITY = 32,
        VS_DEFAULT_GROWBY = 32,
    };
    int m_iMaxQuantity, m_iGrowBy, m_iQuantity;
	VisibleObject* m_akVisible;
};
```

Culler构造，析构，和一系列数据成员和访问有：

```c++
class Culler
{
public:
    Culler(int iMaxQuantity = 0, int iGrowBy = 0,
          const Camera* pkCamera = 0);
    virtual ~Culler();
    
    void SetCamera(const Camera* pkCamera);
    const Camera* GetCamera() const;
    void SetFrustum(const float* afFrustum);
    const float* GetFrustum() const;
    virtual void Insert(Spatial* pkObject, Effect* pkGlobalEffect);
    
protected:
    const Camera* m_pkCamera;
    float m_afFrustum[Camera::VF_QUANTITY];
};
```

Culler维护了裁剪面，除了6个frustum plane，用户还可以设置额外的平面，以及控制利用那些平面进行cull。

```c++
class Culler
{
	enum { VS_MAX_PLANE_QUANTITY = 32 };
    int GetPlaneQuantity() const;
    const Plane3f* GetPlanes() const;
    void SetPlaneState(unsigned int uiPlaneState);
    unsigned int GetPlaneState() const;
    void PushPlane(const Plane3f& rkPlane);
    void PopPlane();
    
    bool IsVisible(const BoundingVolume* pkWound);
    bool IsVisible(int iVertexQuantity, const Vector3f* akVertex, bool bIgnoreNearPlane);
    int WhichSid(const Plane3f& rkPlane) const;
    
protected:
    int m_iPlaneQuantity;
    Plane3f m_akPlane[VS_MAX_PLANE_QUANTITY];
    unsigned int m_uiPlaneState;
};
```

剩下的接口就是获取潜在可见对象集合。

```c++
class Culler
{
public:
    VisibleSet& GetVisibleSet();
    void ComputeVisibleSet(Spatial* pkScene);
    
protected:
    VisibleSet m_kVisible;
};
```

### 4.5.1 Hierarchical culling

Spatial和Culler类，在culling pass的过程中必须进行交互。Spatial中相关的接口有：

```c++
class Spatial : public Object
{
public:
    enum CullingMode
    {
        CULL_DYNAMIC,
        CULL_ALWAYS,
        CULL_NEVER
    };
    
    CullingMode Culling;
    
    void OnGetVisibleSet(Culler& rkCuller, bool bNoCull);
    virtual void GetVisible(Culler& rkCuller, bool bNoCull) = 0;
};
```

CullingMode三种状态的作用是，CULL_DYNAMIC，表示利用culler算法实现剔除，CULL_ALWAYS表示总是会被剔除，在预知可见性的时候会有用。但是，有些情况下一些相机不可见的物体并不能直接被剔除掉，比如不可见的物体的反光作用。Culler计算VisibleSet的接口如下：

```c++
void Culler::ComputeVisibleSet(Spatial* pkScene)
{
    SetFrustum(m_pkCamera->GetFrustum());
    m_kVisible.Clear();
    pkScene->OnGetVisible(*this, false);
}
```

frustum平面是在当前的世界坐标系下的。

TODO：其他代码见原文

### 4.5.2 Sorted culling

TODO: