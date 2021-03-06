# python基于modernGL实现DRR

## 参考资料

github地址：https://github.com/moderngl/moderngl

doc地址：https://moderngl.readthedocs.io/

volume rendering using glsl：https://github.com/toolchainX/Volume_Rendering_Using_GLSL

moderngl volume raycasting example: https://github.com/ulricheck/ModernGL-Volume-Raycasting-Example

pyglm:https://github.com/Zuzu-Typ/PyGLM

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

