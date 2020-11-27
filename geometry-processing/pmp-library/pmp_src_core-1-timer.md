## PMP SRC Core - 1 - 计时器

对于任何一个库而言，对于某个执行过程耗时的监测是很重要的。pmp library也不例外，增加了timer计时的辅助类。内部是采用c++11 std::chrono库实现的。

## 关于std::chrono

chrono主要涉及到的三个概念是：

1. **Durations**。用来测量时间的跨度，比如：1分钟，2小时，10毫秒。用`duration`模板类实现， 将计数表示与周期精度相结合（比如，10毫秒，其中10是计数表示，毫秒是周期精度）；
2. **Time Points**。用来表示具体的时间点，如一个人的生日，下一趟火车抵达的时间。在这个库中，`time_point`类模板的对象通过使用相对于epoch的`duration`来表达这一点；
3. **Clocks**。一种将时间点与实际物理时间联系起来的框架。库中这种联系的方式有：`system_clock`，`steady_clock`，`high_resolution_clock`。

## 关于计时器

通常的使用场景是：

```c++
Timer time;
time.start();
Calculating();
time.stop();
std::cout << "Calculating cost: " << time.elapsed() << "ms" << std::endl;
```

相对于开始、结束，以及获取时间段，该实现种还多了，中断后继续的功能，即：

```
Timer time;
time.start();
Calculating1();
time.stop();
std::cout << "Calculating1 cost: " << time.elapsed() << "ms" << std::endl;
time.cont();
Calculating2();
time.stop();
std::cout << "Calculating1 and Calculating2 cost: " << time.elapsed() << "ms" << std::endl;
```

