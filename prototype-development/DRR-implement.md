# python基于modernGL实现DRR

## 参考资料

github地址：https://github.com/moderngl/moderngl

doc地址：https://moderngl.readthedocs.io/

volume rendering using glsl：https://github.com/toolchainX/Volume_Rendering_Using_GLSL

moderngl volume raycasting example: https://github.com/ulricheck/ModernGL-Volume-Raycasting-Example

pyglm:https://github.com/Zuzu-Typ/PyGLM

## PyGLM简介

pyglm wiki page：https://github.com/Zuzu-Typ/PyGLM/wiki

### PyGLM 类型

主要的数据类型有：

- vector，表示向量，可以有1，2，3或4个元素，可以理解成1xn的向量，也可以理解成nx1的向量，具体在矩阵乘法介绍的时候进行说明；
- matrix，二维矩阵。矩阵的类型表示为`glm.matNxM`，其中N表示列的数目，M表示行的数目，因此数据上的2x3的矩阵，2行3列，那么用glm的表示方法即为`glm.mat3x2`。**这一点在矩阵乘法的时候特别需要注意**
- quaternion，四元数。
- array（`glm.array`）。是存储上述类型或编号之一的多个实例的副本的简单方法。

### Using Vectors

https://github.com/Zuzu-Typ/PyGLM/wiki/Using-vectors

直接看到文档酒醒，没什么需要注意的。

### Using Matrix

https://github.com/Zuzu-Typ/PyGLM/wiki/Using-matrices



将数据传递给别的lib：

```python
# a very important identity matrix
m      = glm.mat4()

# a ctypes pointer to m's data
ptr    = glm.value_ptr(m) 

# size of m's data in bytes (here 4 * 4 * 4 = 64 bytes)
nbytes = glm.sizeof(m)

glBufferData(GL_UNIFORM_BUFFER, nbytes, ptr, GL_STATIC_DRAW)
```



## ModernGL简介

modernGL中最重要的类型和对象：

- Buffer，opengl buffer，比如可以用来写入vertex data，与之对应的是`GL_ARRAY_BUFFER`，支不支持`GL_ELEMENT_ARRAY_BUFFER`？即VBO。
- Program，着色器程序，由GLSL源代码构建；
- VertexArray，职责负责Buffer和Program之间的通信，可以理解如何访问buffer，和进行渲染；即VAO。
- Texture，TextureArray，Texture3D，TextureCube，不同的纹理类型；
- Framebuffer，离屏渲染的render对象。支持不同的attachments，比如texture，depth texture或buffer。

上面的对象只能通过context创建。简单的示例如下：

```python
import struct
import moderngl

ctx = moderngl.create_context(standalone=True)

program = ctx.program(
    vertex_shader="""
    #version 330

    // Output values for the shader. They end up in the buffer.
    out float value;
    out float product;

    void main() {
        // Implicit type conversion from int to float will happen here
        value = gl_VertexID;
        product = gl_VertexID * gl_VertexID;
    }
    """,
    # What out varyings to capture in our buffer!
    varyings=["value", "product"],
)

NUM_VERTICES = 10

# We always need a vertex array in order to execute a shader program.
# Our shader doesn't have any buffer inputs, so we give it an empty array.
vao = ctx.vertex_array(program, [])

# Create a buffer allocating room for 20 32 bit floats
buffer = ctx.buffer(reserve=NUM_VERTICES * 8)

# Start a transform with buffer as the destination.
# We force the vertex shader to run 10 times
vao.transform(buffer, vertices=NUM_VERTICES)

# Unpack the 20 float values from the buffer (copy from graphics memory to system memory).
# Reading from the buffer will cause a sync (the python program stalls until the shader is done)
data = struct.unpack("20f", buffer.read())
for i in range(0, 20, 2):
    print("value = {}, product = {}".format(*data[i:i+2]))
```

## volume rendering 流程

