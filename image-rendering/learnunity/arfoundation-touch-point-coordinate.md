# ARFoundation - touch point坐标点测试

本文目的是为了看一下Android手机上touch之后，对应的点相关信息，主要包括：

- 点的屏幕坐标；
- 点的viewport坐标系下的坐标；
- 点的世界坐标系下的坐标；

代码如下：

```c#
void Update()
{
    if (Input.touchCount > 0)
    {
        Debug.Log("TouchTest touch number is: " + Input.touchCount);
        for (int i=0; i<Input.touchCount; ++i)
        {
            Touch touch = Input.GetTouch(i);
            Debug.Log("TouchTest touch" + Convert.ToString(i) + 
                      " screen position is: " + touch.position);
            Debug.Log("TouchTest touch" + Convert.ToString(i) + 
                      " viewport position is: " + Camera.main.ScreenToViewportPoint(touch.position));
            Debug.Log("TouchTest touch" + Convert.ToString(i) + 
                      " world position is: " + Camera.main.ScreenToWorldPoint(touch.position));
        }
    }
}
```

| 手机屏幕的位置描述 | Screen Position  | Viewport Position | World Position   |
| ------------------ | ---------------- | ----------------- | ---------------- |
| 中间               | (743.0, 1654.0)  | (0.5, 0.5, 0.0)   | (0.1, 0.0, 0.0)  |
| 左下角             | (69.0, 2906.0)   | (0.0, 1.0, 0.0)   | (0.0, 0.0, -0.1) |
| 右下角             | (1388.0, 2960.0) | (1.0, 1.0, 0.0)   | (0.0, 0.0, 0.0)  |
| 右上角             | (1390.0, 190.0)  | (1.0, 0.1, 0.0)   | (0.0, 0.0, 0.0)  |
| 左上角             | (89.0, 217.0)    | (0.1, 0.1, 0.0)   | (0.0, 0.0, 0.0)  |

**所谓的转换得到的世界坐标系的位置是，相机位于世界坐标系中的位置**。