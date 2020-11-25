# Unity MVC Development

## 引入

要学习Unity MVC的开发模式，最简单的方法是，要有代码，又要有文章介绍，并且对于入门而言又不能太难。

基于此，检索到如下资料：

- 文章：[Unity with MVC: How to Level Up Your Game Development](https://www.toptal.com/unity-unity3d/unity-with-mvc-how-to-level-up-your-game-development)
- 代码：
  - [Application-Model-View-Controller-Component](https://github.com/JOESIDinc/Application-Model-View-Controller-Component)

在文章中提出了：AMVCC

![](https://uploads.toptal.io/blog/image/127708/toptal-blog-image-1544131359145-702b3fb0ebea491ebdcf82d218b19edc.png)

建议的分类方法：

**Models**

- Hold the application’s core data and state, such as player `health` or gun `ammo`.
- Serialize, deserialize, and/or convert between types.
- Load/save data (locally or on the web).
- Notify Controllers of the progress of operations.
- Store the Game State for the Game’s [Finite State Machine](http://gamedevelopment.tutsplus.com/tutorials/finite-state-machines-theory-and-implementation--gamedev-11867).
- Never access Views.

**Views**

- Can get data from Models in order to represent up-to-date game state to the user. For example, a View method `player.Run()` can internally use `model.speed` to manifest the player abilities.
- Should never mutate Models.
- Strictly implements the functionalities of its class. For example:    
  - A `PlayerView` should not implement input detection or modify the Game State.
  - A View should act as a black box that has an interface and notifies of important events.
  - Does not store core data (like speed, health, lives,…).

**Controllers**

- Do not store core data.
- Can sometimes filter notifications from undesired Views.
- Update and use the Model’s data.
- Manages Unity’s scene workflow.