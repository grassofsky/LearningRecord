# QFramework 学习记录

## 关于作者的建议

**关于模块**

QF 主要支持了三种模块化，如下：

- 基于单例的模块化
- Manager Of Managers
- 基于 IOC 的模块化

文中作者首推用 IOC 的方式做模块化，但是它有一定的学习门槛。所以对于初学者，或者初学团队，还是建议用比较稳定的 Manager Of Managers 方式去做模块化。而基于单例的模块化建议新手开发者使用，因为比较容易掌握。

文中作者推荐：

- 如果一个人独立负责一个项目，并且是初学者，那么建议用基于单例的模块化、或者 Manager Of Managers 。
- 如果是一个团队负责一个项目，项目规模比较大，但是团队成员的平均水平在初学阶段，推荐用 Manager Of Managers。
- 如果成员的能力都不错，那么建议选择此篇之后的方式，QF 提供了一些别的选择。

**关于事件**

文中作者建议

- 如果还没有使用 UI Kit 中的 消息机制，并且不打算使用 Manager Of Managers 架构，笔者建议使用 TypeEventSystem 作为消息通信的工具
- 如果打算使用 Manager Of Managers 架构，或者 UI Kit 中的消息机制，那么就用 UI Kit 提供的消息机制就好，具体怎么用会在下一篇进行介绍
- 全局的 QEventSystem 与 UI Kit、Manager Of Managers 的 QEventSystem  不是同一个示例，所以在某些情况下如果用到 全局 QEventSystem 那么不要担心与 UI Kit 冲突的问题。不过这里要注意一下 全局的  QEventSystem 事件是发送不到 Manager Of Mananger 和 UI Kit 中的。

## 小结

基于上述，考虑到刚开始使用Unity，

如果项目涉及到团队协作：

选择使用**Manager Of Managers模式**，并且同QEventSystem一起使用。

Manager Of Managers（一）模块设计介绍：https://qframework.cn/doc/detail/3e066f8c-1aca-4412-a56d-41fa753406b7#

QEventSystem介绍：https://qframework.cn/doc/detail/78842527-dac2-41e5-b4a7-46d644778af8

如果项目功能简单，单人可完成：

那么可以选择单例的模块化和TypeEventSystem进行开发。

## 使用Manager Of Managers需要考虑的事项

**需要包含哪些模块**

而刘钢老师的在视频中列出了以下模块（- 后面跟着的是QFramework中对应的实现）：

- MainManager
- EventManager
- AudioManager - AudioKit下面的AudioManager
- GUIManager - UIKit下面的UIManager
- PoolManager
- LevelManager
- GameManager
- SaveManager
- MenuManager

基于QMgrBehaviour实现的类，仅有：AudioManager，以及UIManager，想要借用Manager of Managers开发模式，可以构建自己的Manager，具体的构建方式参见现有的AudioManager的实现。

**事件如何管理**

QEventSystem

## 利用Unity开发考虑的事项

- 开发模式选择-Manager of Managers
- 工具 - QLog，QApp，QConsole
- 模块划分