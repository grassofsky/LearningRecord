# C++ 创建动态属性

## 功能清单

- 支持属性动态添加，如：

  ```c++
  Properties p;
  
  p.Add<int>("age", 10);      // 属性名称用枚举，或者字符串
  
  int age = p.Get<int>("age");
  ```

- 支持查看属性是不是有更新，如：

  ```c++
  p.IsDirty<int>("age");
  ```

- 支持数据拷贝，如：

  ```c++
  std::vector<PropertyName> propertyNames = p.GetAllPropertyNames();
  for (auto& name : propertyNames)
  {
      if (p.IsDirty(name))
      {
          p_inner.Set(p.Get(name));
      }
  }
  ```

- 重置dirty属性，如：

  ```c++
  p.ResetAll();
  ```

- 支持内存大小动态变化的属性。

## 基于模板的实现

from: https://stackoverflow.com/questions/20225392/c-dynamic-properties

```c++
#include <string>
#include <map>

using namespace std;

class Prop
{
public:
    virtual ~Prop()
    {
    }
};

template<typename T>
class Property : public Prop
{
private:
    T data;
    bool isDirty;
public:
    virtual ~Property()
    {
    }

    Property(T d) : isDirty(true)
    {
        data = d;
    }

    void SetValue(T& d)
    {
        data = d;
    }
    
    T GetValue()
    {
        return data;
    }
};

class Properties
{
private:
    map<string, Prop*> props;
public:
    ~Properties()
    {
        map<string, Prop*>::iterator iter;

        for(iter = props.begin(); iter != props.end(); ++iter)
            delete (*iter).second;

        props.clear();
    }

    template<typename T>
    void Add(string name, T data)
    {
        if (props.count(name) != 0)
        {
            props[name]->SetValue(data);
        }
        else
        {
            props[name] = new Property<T>(data);
        }
    }

    template<typename T>
    T Get(string name)
    {
        Property<T>* p = (Property<T>*)props[name];
        return p->GetValue();
    }
    
    bool IsDirty(string name)
    {
        if (props.count(name) == 0)
        {
            return false;
        }
        return props[name]->IsDirty();
    }
};

int main()
{
    Properties p;

    p.Add<int>("age", 10);

    int age = p.Get<int>("age");

    return 0;
}
```

## 基于boost::any

参见：https://www.cnblogs.com/youxin/p/4451950.html

```c++
class Property
{  
    boost::any value_;
    bool dirty_;
public:  
    property(const std::string& name,const boost::any& value)  
        : value_(value)
        ,dirty_(true)
    {}
	
    void SetValue(const boost::any& value)
    {
        value_ = value;
        dirty_ = true;
    }

    boost::any& GetValue() { return value_; } 
    
    bool IsDirty()
    {
        return dirty_;
    }
    
    friend bool operator<(const property& lhs, const property& rhs)
    {
        return lhs.name_<rhs.name_;
    }
};

class Properties
{
private:
    map<string, Property> props;
public:
    void Add(string name, boost::any data)
    {
        if (props.count(name) != 0)
        {
            props[name].SetData(data);
        }
        else
        {
            props[name] = Property(data);
        }
    }
    
    template<typename T>
    T& Get(string name)
    {
        return boost::any_cast<T>(props[name].GetValue());
    }
    
    bool IsDirty(string name)
    {
        if (props.count(name) == 0)
        {
            return false;
        }
        return props[name].IsDirty();
    }
};

int main()
{
    Properties p;

    p.Add("age", 10);

    int age = p.Get<int>("age");

    return 0;
}
```

## 扩展

利用动态属性进行继承的时候，需要让调用者知道具体的属性是什么，因此可以构建辅助类。具体如下：

```c++
class CommonColorHelper
{
public:
    typedef Vector3D ColorType;

    string GetColorName();
    ColorType GenerateColorValue(double r, double g, double b, double a);
    
    string GetAmbientColorName();
    ColorType GenerateAmbientColorValue(double r, double g, double b, double a);
    
    string GetDiffuseColorName();
    ColorType GenerateDiffuseColorValue(double r, double g, double b, double a);
};

int main()
{
    Properties p;
    CommonColorHelper colorHelper;
    p.Add(colorHelper.GetColorName(), colorHelper.GenerateColorValue(1.0,1.0,1.0,1.0));
    auto color = p.Get<CommonColorHelper::ColorType>(colorHelper.GetColorName());
}
```

## VTK的实现

概念参考：[VTK修炼之道82：VTK管线机制_信息对象类VTKInformation](https://blog.csdn.net/shenziheng1/article/details/54986880)

VTK最内部的实现代码如下：

```c++
//----------------------------------------------------------------------------
class vtkInformationInternals
{
public:
  typedef vtkInformationKey* KeyType;
  typedef vtkObjectBase* DataType;
#ifdef VTK_INFORMATION_USE_HASH_MAP
  struct HashFun
  {
    size_t operator()(KeyType key) const { return static_cast<size_t>(key - KeyType(nullptr)); }
  };
  typedef std::unordered_map<KeyType, DataType, HashFun> MapType;
#else
  typedef std::map<KeyType, DataType> MapType;
#endif
  MapType Map;

#ifdef VTK_INFORMATION_USE_HASH_MAP
  vtkInformationInternals()
    : Map(33)
  {
  }
#endif

  ~vtkInformationInternals()
  {
    for (MapType::iterator i = this->Map.begin(); i != this->Map.end(); ++i)
    {
      if (vtkObjectBase* value = i->second)
      {
        value->UnRegister(nullptr);
      }
    }
  }

private:
  vtkInformationInternals(vtkInformationInternals const&) = delete;
};
```

最后是通过一个map将KeyType和DataType关联起来。

具体KeyType都是继承vtkInformationKey实现，具体的DataType都是继承vtkObjectBase实现。

另一个关键的类是vtkInformation，这里仅截取片段：

```c++
class VTKCOMMONCORE_EXPORT vtkInformation : public vtkObject
{
public:
  // 实现了很多get set一类的函数，如：
  //@{
  /**
   * Get/Set an integer-pointer-valued entry.
   */
  void Set(vtkInformationIntegerPointerKey* key, int* value, int length);
  int* Get(vtkInformationIntegerPointerKey* key);
  void Get(vtkInformationIntegerPointerKey* key, int* value);
  int Length(vtkInformationIntegerPointerKey* key);
  void Remove(vtkInformationIntegerPointerKey* key);
  int Has(vtkInformationIntegerPointerKey* key)
  //@}
};
```

