# PMP SRC Core - 0 - 软件版本号控制

关于版本号的介绍参见：https://baike.baidu.com/item/%E8%BD%AF%E4%BB%B6%E7%89%88%E6%9C%AC%E5%8F%B7/188753

代码中有三个等级划分：

```c++
#define PMP_MAJOR_VERSION 1
#define PMP_MINOR_VERSION 2
#define PMP_PATCH_VERSION 1
```

分别为`主版本号.次版本号.补丁版本号`。按照常见的功能解释如下：

1. 项目初版本时，版本号可以为 0.1 或 0.1.0，也可以为 1.0 或 1.0.0，如果你为人很低调，我想你会选择那个主版本号为 0 的方式；
2. 当项目在进行了局部修改或 bug 修正时，主版本号和子版本号都不变，修正版本号加 1；
3. 当项目在原有的基础上增加了部分功能时，主版本号不变，子版本号加 1，修正版本号复位为 0，因而可以被忽略掉；
4. 当项目在进行了重大修改或局部修正累积较多，而导致项目整体发生全局变化时，主版本号加 1；

因此该类也提供了一些常见的接口，具体如下：

```c++
//! API version information
//! \ingroup core
class Version
{
public:
    //! return the major version number
    static int get_major();

    //! return the minor version number
    static int get_minor();

    //! return the patch version number
    static int get_patch();

    //! return the current API version number as a string
    static std::string get_version();

    //! return true if the current version >= (major, minor, patch)
    static bool is_at_least(int major, int minor, int patch);

    //! return true if the named feature is available in this version
    static bool has_feature(const std::string &name);
};
```

其中由于该library处于开发阶段，`has_feature`函数，并没有明显作用。

