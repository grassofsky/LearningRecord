# unity - 从child中获取TextMeshPro以及设置text

**问题一、无法直接调用`GetComponentInChildren`从孙子后代的`gameobject`中获取`TextMeshPro`组件。具体原因未知。**

通常情况下GetComponentInChildren能够自行进行深度遍历查找（如：https://blog.csdn.net/qq_39108767/article/details/101757494中介绍的），不知道为什么此时不行。

**解决方法：**

通过显式的深度优先遍历，获取对应的TextMeshPro组件，具体代码类似如下：

```c#
private T GetComponentInChildrenExt<T>(Transform trans) where T : class
{
    if (trans == null)
    {
        return default(T);
    }

    var component = trans.gameObject.GetComponentInChildren<T>();
    if (component != null)
    {
        return component;
    }

    for (int i = 0; i < trans.childCount; i++)
    {
        component = GetComponentInChildrenExt<T>(trans.GetChild(i));
        if (component != null)
        {
            return component;
        }
    }
    return default(T);
}
```

**问题二、在Start中通过下面的方式设置TextMeshPro的text属性无效**

```c#
private TextMeshPro _textMesh;
void Start()
{
    _textMesh = GetComponentInChildrenExt<TextMeshPro>(transform);
    _textMesh.text = "any other text";
}
```

**解决办法：**

尝试后发现，需要在update中多次设置text：

```c#
private int _textSetMaxCount = 3;  // 具体次数尝试后确定
private int _textSetCount = 0;

private void Update()
{
    if (_textSetCount < _textSetMaxCount)
    {
    	_textMesh.text = "any other text";
        _textSetCount++;
    }
}
```

## 补充

如果有谁知道具体原因，或有更好的解决方法，请留言，谢谢。

