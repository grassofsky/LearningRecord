代码位于：https://gitee.com/grassofsky/learnopengl-vs2010

使用visual studio 2010的时候如果使用glad，发现`gladLoadGLLoad((GLADloadproc)glfwGetProAddress))`返回一直为false，因此此处使用glew。

使用了glm的版本为0.9.5.4.

在使用glew的时候需要注意的有：`#include <gl/glew.h>`需要放到最前面。

`glewInit()`要放到`glfwMakeContextCurrent`之后，并且在调用glewInit之前需要将glewExperimental变量设置成`GL_TRUE`。

源代码自带的相机实在是太难操作了。