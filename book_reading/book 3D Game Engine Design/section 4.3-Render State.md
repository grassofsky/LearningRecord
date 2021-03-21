# Chapter 4 Scene Graphs

## 4.3 Render state

### 4.3.1 Global state

当前引擎中引入的状态有：alpha blending，triangle culling，material，polygon offset，stenciling，wireframe，depth buffering。对于global state，场景图中一个节点的状态，会作用到其下的所有子节点。类`GlobalState`接口如下：

```c++
class GlobalState : public Object
{
public:
    virtual ~GlobalState();
    
    enum StateType
    {
        ALPHA,
        CULL,
        MATERIAL,
        POLYGONOFFSET,
        STENCIL,
        WIREFRAME,
        ZBUFFER,
        MAX_STATE_TYPE
    };
    
    virtual StateType GetStateType() const = 0;
    
    // default states
    static Pointer<GlobalState> Default[MAX_STATE_TYPE];
protected:
    GlobalState();
};
```

gloablstate会被Spatial类中用到，如下：

```c++
class Spatial : public Object
{
public:
    int GetGlobalStateQuantity () const;
    GlobalState* GetGlobalState (int i) const;
    GlobalState* GetGlobalState (GlobalState::StateType eType) const;
    void AttachGlobalState (GlobalState* pkState);
    void DetachGlobalState (GlobalState::StateType eType);
    void DetachAllGlobalStates ();
protected:
	std::vector<GlobalStatePtr> m_kGlobalStates;
};
```

类Geometry中也存储了GlobalState，这里存储的状态是从根节点到子节点遍历的时候，遇到的状态的记录。在render-state update的时候赋值的。

```c++
class Geometry : public Spatial
{
// internal use
public:
    GlobalStatePtr States[GlobalState::MAX_STATE_TYPE];
};
```

### 4.3.2 Lights

如果Light类没有数据，或者只有ambient，intensity，那么可以使用标准类层级：

```c++
class Light
    [ambient, intensity];

class AmbientLight : public Light
    [no additional data];

class DirectionLight : public Light
    [direction, diffuse, specular];

class PointLight : public Light
    [position, diffuse, specular, attenuation];

class SpotLight : public PointLight
    [cone axis, cone angle, spot exponent]
```

但使用继承的关系，在使用类的成员的时候，需要针对具体的类型使用`dynamic_cast`进行转换，而这种开销通常需要避免。最后选用一个Light类，以及枚举，还实现不同的光源类型。

#### The light class

```c++
class Light : public Object
{
public:
    enum LightType
    {
        LT_AMBIENT,
        LT_DIRECTIONAL,
        LT_POINT,
        LT_SPOT,
        LT_QUANTITY
    };
    Light (LightType eType = LT_AMBIENT);
    virtual ~Light ();
    LightType Type; // default: LT_AMBIENT
    
    ColorRGB Ambient; // default: ColorRGB(0,0,0)
    ColorRGB Diffuse; // default: ColorRGB(0,0,0)
    ColorRGB Specular; // default: ColorRGB(0,0,0)
    
    float Constant; // default: 1
    float Linear; // default: 0
    float Quadratic; // default: 0
    float Intensity; // default: 1
    
    float Angle; // default: pi
    float CosAngle; // default: -1
    float SinAngle; // default: 0
    float Exponent; // default: 1
    void SetAngle (float fAngle);
    
    Vector3f Position, DVector, UVector, RVector;
    void SetDirection (const Vector3f& rkDirection,
   		bool bUnitLength = true);
    bool IsValidFrame () const;
};
```

#### Support for lights in spatial and geometry classes

`Spatial`类存储了lights的数组。如果light添加到这个数组，并且会作用到所有的子节点。与光源相关的接口有：

```c++
class Spatial : public Object
{
public:
    int GetLightQuantity () const;
    Light* GetLight (int i) const;
    void AttachLight (Light* pkLight);
    void DetachLight (Light* pkLight);
    void DetachAllLights ();
protected:
    std::vector<ObjectPtr> m_kLights; // 用objectptr，而不是lightptr，为了避免循环依赖问题
};
```

在Wild Magic version 3中，`Geometry`类存储了light的数组，这些light是从根节点到geometry叶子节点，遍历的过程中遇到的light的记录。Wild Magic version 4，采用基于shader的架构，lighting被shader处理。

### 4.3.3 Effects

在Wild Magic 4，effect类只有一个简单的虚函数：

```c++
virtual void Draw(Renderer* pkRenderer, Spatial* pkGlobalObject, int iVisibleQuantity, VisibleObject* akVisible);
```

只需要标准顶点属性和纹理属性的local effect，不需要这个函数。`ShaderEffect`用来表示local effects。其他高级的效果，会需要重写Draw函数。但通常情况下，Renderer类能够处理任何一个全局的effect，因此在添加新的effect的时候Renderer的代码不需要改变。默认的Draw函数如下：

```c++
void Effect::Draw (Renderer* pkRenderer,
Spatial* pkGlobalObject, int iVisibleQuantity,
VisibleObject* akVisible)
{
    // The default drawing function for global effects.
    // Essentially, this is a local effect applied to
    // all the visible leaf geometry.
    VisibleObject* pkCurrent = akVisible;
    for (int i = 0; i < iVisibleQuantity; i++, pkCurrent++)
    {
        Geometry* pkGeometry = (Geometry*)pkCurrent->Object;
        pkGeometry->AttachEffect(this);
        pkRenderer->Draw(pkGeometry);
        pkGeometry->DetachEffect(this);
	} 
}
```

需要关注的是，这些pass如何被blended到一块儿。