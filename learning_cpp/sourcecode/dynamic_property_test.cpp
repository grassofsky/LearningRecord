#include <boost/any.hpp>
#include <iostream>
#include <map>

class Property
{
public:
    Property()
    {
    }
    
    Property(const std::string name, const boost::any& value) 
    : m_value(value)
    , m_bDirty(true) 
    , m_name(name)
    {}
    
    void Set(const boost::any& value) 
    { 
        m_value = value; 
    }
    
    const boost::any& Get() const
    { 
        return m_value; 
    }

    void SetDirty(bool flag) 
    { 
        m_bDirty = flag; 
    }
    
    bool IsDirty() const
    {
        return m_bDirty; 
    }

   
private:
    std::string m_name;
    boost::any m_value;
    bool m_bDirty;
};

class Properties
{
public:
    Properties() {};
    ~Properties() {};

    void Set(std::string name, boost::any value) 
    { 
        if (m_mapProps.count(name) == 0) 
        {
            m_mapProps[name] = Property(name, value); 
        }
        else
        {
            m_mapProps[name].Set(value);
        }
    }


    template<typename T> 
    bool Get(std::string name, T& value) 
    { 
        auto iter = m_mapProps.find(name);
        if (iter == m_mapProps.end())
        {
            return false;
        }
        
        value = boost::any_cast<T>(iter->second.Get()); 
        return true;
    }
    
    template<>
    bool Get(std::string name, boost::any& value) 
    {
        auto iter = m_mapProps.find(name);
        if (iter == m_mapProps.end())
        {
            return false;
        }

        value = iter->second.Get(); 
        return true;
    }


    bool IsDirty(std::string name) 
    { 
        auto iter = m_mapProps.find(name);
        if (iter == m_mapProps.end())
        {
            return false;
        }

        return iter->second.IsDirty(); 
    }

    void ClearAllDirty()
    {
        for (auto iter = m_mapProps.begin(); iter != m_mapProps.end(); ++iter)
        {
            iter->second.SetDirty(false);
        }
    }
    
private:
    std::map<std::string, Property> m_mapProps;
};

struct Vector3D
{
    double x, y, z;
};

int main()
{
    Properties properties;

    Vector3D vec;
    vec.x = 1.0;
    vec.y = 2.0;
    vec.z = 3.0;
    properties.Set("Vector3D", vec);
    std::cout << "Vector3D is dirty: " << properties.IsDirty("Vector3D") << std::endl;
    Vector3D vec2;
    properties.Get<Vector3D>("Vector3D", vec2);
    std::cout << "Vector3D is : " << vec2.x << ", " << vec2.y << ", " << vec2.z << std::endl;
    
    int ivalue = 21;
    properties.Set("ivalue", ivalue);
    std::cout << "ivalue is dirty: " << properties.IsDirty("ivalue") << std::endl;
    boost::any value;
    properties.Get("ivalue", value);
    std::cout << "ivalue is: " << boost::any_cast<int>(value) << std::endl;
    
    
    std::shared_ptr<Vector3D> spVec(new Vector3D);
    spVec->x = 1.0;
    spVec->y = 2.0;
    spVec->z = 3.0;
    properties.Set("SPVector3D", spVec);
    std::cout << "SPVector3D is dirty: " << properties.IsDirty("SPVector3D") << std::endl;
    std::shared_ptr<Vector3D> spVec2;
    properties.Get<std::shared_ptr<Vector3D>>("SPVector3D", spVec2);
    std::cout << "SPVector3D is : " << spVec2->x << ", " << spVec2->y << ", " << spVec2->z << std::endl;
    
    return 0;
}