import numpy as np

import matplotlib.pyplot as plt

def show_example2_1():
    """
    一个半径为a的圆，参数方程为：
     r(t) = (a*cos(t), a*sin(t)), t in (0, 2pi), 假设a=2
    绘制：
    - 时间参数方程曲线
    - 弧长参数方程曲线
    - 单位正切向量曲线
    - 单位正法向量曲线
    """
    
    print("====时间参数方程曲线")
    a = 2
    t = np.arange(0, 2*np.pi+0.1, 0.1)
    curve_x = a * np.cos(t)
    curve_y = a * np.sin(t)
    plt.figure()
    nimages = 6
    plt.subplot(nimages,1,1)
    plt.plot(curve_x, curve_y)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.title("Curve")
    plt.axis("equal")
    plt.xlim(0,15)
   
    plt.subplot(nimages,1,2)
    plt.plot(t, curve_x)
    plt.xlabel("t")
    plt.ylabel("x")
    plt.title("x on t")
    plt.axis("equal")
    plt.xlim(0,15)
    
    plt.subplot(nimages,1,3)
    plt.plot(t, curve_y)
    plt.xlabel("t")
    plt.ylabel("y")
    plt.title("y on t")
    plt.axis("equal")
    plt.xlim(0,15)
    
    print("====弧长参数方程曲线")
    s = np.arange(0, 2*np.pi*a+0.1, 0.1)
    length_x = a*np.cos(s/a)
    length_y = a*np.sin(s/a)
    plt.subplot(nimages,1,4)
    plt.plot(s, length_x)
    plt.xlabel("s")
    plt.ylabel("x")
    plt.axis("equal")
    plt.xlim(0,15)
  
    plt.subplot(nimages,1,5)
    plt.plot(s, length_y)
    plt.xlabel("s")
    plt.ylabel("y")
    plt.axis("equal")
    plt.xlim(0,15)
    
    plt.subplot(nimages,1,6)
    plt.plot(length_x, length_y)
    plt.xlabel("x")
    plt.ylabel("y")
    plt.axis("equal")
    plt.xlim(0,15)
    
    plt.show()
