# IMGUI介绍

原文链接：http://www.johno.se/book/imgui.html

## 动机

UI编程是一件比较困难的事情。这可能很大程度上是因为用户界面工具包往往是大型和复杂的软件系统。掌握他们会有一个陡峭的学习曲线，为了集成，通常涉及大量特定于应用程序的实现。在使用软件的时候，留意UI的表现形式，可以发现很多相同功能的UI，会有很多表现形式。通常涉及一个ui toolkit仍然是一件很困难的事情，特别针对游戏行业。通常原先设计的通用的UI，到最后会发现不在那么通用了。

随着GPU的发展，现在出现了一种新的方式来处理UI。即时模式图形用户界面（IMGUI）代表了一种范例，其中用户界面的创建（对于客户端应用程序）和实现更简单（对于工具箱设计器）。

## 缓存状态的悲哀

我认为，传统用户界面系统设计和使用的复杂性是由此类系统保持了各种状态引起的。程序员通常被要求在应用程序和用户界面之间来回主动地复制状态，以便用户界面反映应用程序的状态，反之，为了使用户界面中发生的更改影响应用程序的状态。

这是一个基本的问题，这样的状态（来自UI交互系统），是实际状态的一份COPY/CACHE。

从客户端应用的角度看，UI更像是一堆对象的集合（通常称为widget），这些对象分装了需要和应用交互的各种状态。这个同步过程是双向的，为了让用户了解到当前应用的状态，状态需要从应用端转移到UI端；为了让应用知道当前用户的操作，状态需要从UI端转移到应用端。 有时，根据所使用的工具包，用户界面工具包为这种“数据交换”提供了一定程度的自动化，但同步本身（更不用说重复状态）仍然是现实。 

 此外，通知应用程序用户与接口交互的方式（这反过来表明需要重新同步状态）通常采用回调的形式。这需要应用程序为感兴趣的任何低级交互实现“事件处理程序”，通常是通过手动或通过各种代码生成技巧将某些工具箱基类子类化；在任何一种情况下，都会使客户机应用程序的生命周期更加复杂。 

## 应用立即模式（Immediate Mode applied)

IMGUI通过要求应用程序实时显式传递可视化和交互所需的所有状态来消除这种状态同步。 用户界面只保留了为方便系统支持的每种小部件所需的功能所需的最小状态量。

使用IMGUI，概念发生了变化。Widget不再是对象，甚至可以说是不存在的。它们采用过程方法调用的形式，用户界面本身从有状态的对象集合变成了方法调用的实时序列。

这种实现方式的基础是实时应用程序循环的概念，也就是说，应用以实时的帧率处理逻辑和绘制（30 frames per second或者更高）。在游戏场景，这个是很常见的。

乍看起来，通过传递参数的方式实现看上去很冗余，但实际上并不是这样的。看上去会影响绘制性能。但是，对于现代的CPU和GPU而言，这个不是问题。

其优点在于简单和灵活。删除用户界面系统中的隐式状态缓存可以减少与缓存相关的bug的可能性，也完全消除了工具箱将小部件作为对象公开给客户机应用程序的需要。在逻辑上，小部件从对象变为方法调用。我们将看到，这从根本上改变了客户机应用程序处理用户界面实现的方式。

## Issues of acceptance

在深入内部的详细实现之前，我想要讨论下“Why”。

我意识到使用现有的用户界面工具包的一个重要原因是存在可供使用的工具箱；您不必自己编写代码，可以专注于自己的细节应用程序。这里我将要解释如何从头开始实现一个用户界面工具箱；为什么这很有趣？

这里可能不太明显，但使用IMGUI的主要好处和原因是，实际的特定于应用程序的客户端代码变得更少（更少的LOC）和更简单。对于小型应用程序来说，这可能没有什么好处，但是任何非常复杂的用户界面通常都是非常不容易实现和维护的，即使是给定了一个用户界面工具包。

### An example of simplification

在我的一个游戏中，UfoPilot II : The Phadt Menace, 整个前端一开始用传统的方法实现的。和MFC对话框有点类似。在这里，我为每个特定的“screen”设置了一个class，并在用户浏览界面时实例化了每个类的一个对象。

每个screen类有多个widget成员，布局结构。通常会运行一下程序，看一下layout的正确性，然后编辑代码，然后在运行看看，重复如上操作。此时丰富的编辑器在一定程度上会帮助到我。

将这个用户界面移植到IMGUI后，在移植过程中根据需要实现了工具箱方法（我在开发过程中构建了Gui类，将代码从Widget类移到Gui类），我获得了以下几点好处：

 首先，在每一个“screen”都有一个类的情况下，它从一个类分解为一个Menu类中的单个方法（它代表前端屏幕和代码的整个集合）。这样从10-15个类，缩减到了一个类。

所有的widgets类都被分解成Gui类的方法，同样，我以前有几个类，现在有一个。

前端的进一步变化和迭代从涉及小部件实例化、布局、回调等的痛苦经历转变为以“if（doButton（））do something…”的形式添加或删除几行代码。

Layout仍然直接编码到应用程序中，但是由于我已经掌握了这些信息，所以只需移动代码就可以了。

### How to encourage acceptance

有人将不得不实现一个参考tookit和一个或多个使用它的应用程序。此外，这些应用程序必须强调这样一个事实：这是一个范式的转变，而不仅仅是一个漂亮的把戏。事实上，纵观WPF，整个思维模式仍极为保留。

到目前为止，由于使用GDI或GDI+绘图的开销，它在win32上还不可行（除非人们转而使用DirectX）。然而，在Windows7上，新的directxapi Direct2D和DirectWrite可能是高分辨率/高性能IMGUI应用程序的一个很好的解决方案。

这个参考工具包需要“看起来不错”。这意味着小部件看起来“现代”；让艺术家参与其中可能是个好主意。。。:)

 “布局问题”需要解决，因为对于许多应用程序，我怀疑要求程序员处理每个“小部件”的布局是不可行的。

## Basic implementation (Object Oriented)

客户看到单个Gui实例（IMGUI 上下文）。单个的实例封装了完整的GUI系统或框架。对于每个widget类型会有一个或多个methods与之对应。下面是一个Gui类的示例：

```c++
class Gui
{
public:
    void label(const int aX, const int aY, const char* aText);
    const bool button(const int aX, const int aY,
                      const int aWidth, const int aHeight,
                      const char* aText);
    const bool radio(const bool anActive,
                     const int aX, const int aY,
                     const int aWidth, const int aHeight,
                     const char* aText);
    const bool check(const bool anActive,
                     const int aX, const int aY,
                     const int aWidth, const int aHeight,
                     const char* aText);
    const bool tab(const bool anActive,
                     const int aX, const int aY,
                     const int aWidth, const int aHeight,
                     const char* aText);
    void edit(const int aX, const int aY, String& aString);
};
```

那么应用的时候，想要放一个widget，只需要调用对应的方法即可。

在任何情况下，都要遵守基本前提；客户机应用程序在任何给定时间以帧为基础传递给定小部件运行所需的所有状态 

### Buttons

 基于这个前提，对一个按钮的点击作出反应很简单： 

```c++
Gui myGui;
void doSomeUserInterface()
{
    if (myGui.button(64,64,32,16))
    {
        // do something as a result of the button being clicked
    }
}
```

### Radio buttons, check boxes, and tabs

对于上述控件的使用示例如下：

```c++
Gui myGui;
int myChoice(0);

void doSomeUserInterface()
{
    int i;
    for (i = 0; i < 5; ++i)
    {
        if (myGui.radio(myChoice == i, 
                        64, 64 + i*20,
                        32, 16,
                        String::format("choice %d", i+1)))
        {
            myChoice = i;
        }
    }
}
```

如您所见，由上述代码生成的用户界面基于应用程序的实际状态，而不是任何“小部件对象”的状态。同样，IMGUI的中心主题；没有必要明确地将应用程序状态同步到gui状态，因为只存在一个状态副本，即应用程序本身的副本。

PS: 如上所示，当点击之后的下一帧，显示的状态才会发生变化。

### Edit boxes

使用编辑框也很简单，在这种情况下，您可以传递一个字符串引用，即编辑。又一次，其思想是传递一个字符串实例，该实例是应用程序状态的一部分，由gui直接编辑

```c++
Gui myGui;
String myString("hello");
void doSomeUserInterface()
{
    myGui.edit(64,64,myString);
}
```

### Hey, where's the list box?

大多数用户界面工具包都支持列表框/列表控件的概念。有趣的是，这种小部件类型在IMGUI中基本上被忽略了（除非您明确要求滚动支持；请参阅高级特性部分）。因为列表通常只是一堆文本标签，您可以通过简单地执行以下操作来支持它：

```c++
Gui myGui;
String myStrings[5] = {""hello"", ""how"", ""are"", ""you"", ""doing""};
void doSomeUserInterface()
{
    int i;
    for(i = 0; i < 5; i++)        
        myGui.label(64, 64 + i * 16, myStrings[i]);
}
```

如果您需要选择支持（与许多列表框支持相同），您可以执行类似于以下操作的操作，这是单选按钮或属性表/选项卡的典型方法（见上文）:

```c++
Gui myGui;
String myStrings[5] = {""hello"", ""how"", ""are"", ""you"", ""doing""};
int mySelection(0);

void doSomeUserInterface()
{
    int i;
    for (i = 0; i < 5; ++i)
    {
        if (myGui.radio(mySelection == i, 
                        64, 64 + i*20,
                        32, 16,
                        myStrings[i]))
        {
            mySelection = i;
        }
    }
}
```

## How it works

### Widgets as methods instead of objects

Gui类中的widget 方法封装了widget相关的存在，显示，交互的逻辑。再次注意，从客户端的角度来看，小部件只能说是以方法调用的形式“存在”；小部件从对象变为方法调用。

这里的主要好处之一是完全集中控制调用代码。“widgets”和响应用户与这些widget交互的代码都在同一个地方。

此外，看一下下面的例子：

```c++
Gui myGui;
bool myEnableChoices(false);
int myChoice(0);
void doSomeUserInterface()
{
    if (myGui.button(64, 64, 32, 16))
    {
        myEnableChoices = !myEnableChoices;
    }
    if (myEnableChoices)
    {
        for (int i=0; i<5; i++)
        {
            if (myGui.radio(myChoice == i,
                            64, 64 + i * 20,
                            32, 16,
                            String::format("choice %d", i+1)))
            {
                myChoice = i;
            }
        }
    }
}
```

正如您所看到的，“启用或禁用”用户界面的某些方面非常简单，这是因为用户界面根本就不以要启用或禁用的对象的形式存在，因此可以轻松地在每帧的基础上更改，而不需要任何开销.

当然，可以使用任意复杂的表达式来代替简单的布尔变量myEnableChoices；这是IMGUI强大功能和灵活性的重要组成部分。在传统的用户界面系统中，这种功能通常需要大量启用/禁用小部件对象。

### Implementing basic interactions

在上述例子中，最关键的交互是鼠标左键点击widget。为了实现这么个功能，需要获取鼠标点击的事件，和当前点击的位置。一份简单实现如下：

```c++
const bool Gui::button(const int aX, const int aY,
                      const int aWidth, const int aHeight,
                      const char* aText)
{
    drawRect(aX, aY, aWidth, aHeight);
    drawText(aX, aY, aText);
    
    return mouse::leftButtonPressed() &&
        mouse::cursorX() >= aX &&
        mouse::cursorY() >= aY &&
        mouse::cursorX() < (aX + aWidth) &&
        mouse::cursorY() < (aY + aHeight);
}
```

### Implementing edit boxes

编辑框比button复杂点，主要是要知道输入聚焦的位置。

```c++
void Gui::edit(const int aX, const int aY, String& aString)
{
    if (&aString == myEditInstance)
        activeEdit(aX, aY, aString);
    else
        passiveEdit(aX, aY, aString);
}
```

Gui维护一个指向当前正在编辑的字符串的指针，以便在帧之间保持焦点。这本质上是一个简单的“盲数据句柄”（const void*），它只被Gui用作外部（应用程序）上下文的标识符.

根据这个焦点信息，每个编辑框要么是活动的（在任何给定的时间只有一个）或者是被动的。为了简洁起见，这里省略了activeEdit（）的实现，但是可以在direct3d示例中看到完整的实现.

```c++
void Gui::passiveEdit(const int aX, const int aY, const String& aString)
{
    if (radio(false, aX, aY, aString))
        myEditInstance = &aString;
}
```

一个被动的编辑框在这里被实现为一个非活动的单选按钮，点击它将使它成为焦点，使其成为活动的。

编辑框的实现突出了imgui的一个重要细节；它们并不完全是无状态的。但是，它们只需保留足够的状态，一次只需处理单个交互。这是imgui如何在没有widget对象的情况下摆脱困境的关键细节；它们在逻辑上有一些内部“widget state”，但是对于每种支持的widget类型，它们只需要一个副本，因为用户在任何给定的时间内只能与单个widget交互。

### Implementing display

当谈到小部件的实际显示时，需要考虑一些不同的方法，这取决于应用程序需求的几个方面。

在上面的交互示例中，内部方法drawRect（）和drawText（）是在计算并返回实际交互之前调用的。这些方法的实际作用取决于许多因素，如下面所示。在任何情况下，调用代码都必须小心，不要创建重叠的情况（这些基本示例无法以任何方式处理重叠的窗口小部件/窗口，但请参阅更高级的示例以了解如何实现这一点）

#### Direct display

如果调用代码打算在一个过程中完成所有的用户界面交互和显示，并且考虑到平台本身支持它，那么基本上可以让drawRect（）和drawText（）直接呈现到底层的“画布”中。

大多数基于软件的2d绘图库（例如Win32上的GDI）将支持这种类型的实现，因为它们通常具有“绘图画布”的概念，并且通常本质上对绘图顺序或绘图调用的数量不敏感

#### Deferred display（延迟显示）

在需要更多控制显示的情况下，可以使用延迟显示。这基本上可以归结为让drawRect（）和drawText（）将“绘图事件”记录到某个列表中，然后让应用程序遍历该列表并适当地绘制它，比如调用Gui::draw，执行具体的绘制事件。请注意，此缓存仅在一个帧的持续时间内持续，并有助于有效地批处理预处理。

这种情况适用于大多数硬件加速的应用程序，其中底层API针对类似原语的批处理进行了优化。

使用延迟显示的另一个原因是如果您的应用程序像许多游戏一样将输入和输出分开。在input（） /update（） /output（）的典型循环中，“widget calls”和对结果交互的相应反应将进入input（），而缓冲窗口小部件的实际显示将进入output（），通常最后显示（在您正在显示的任何3d内容之上）。**意思就是说，游戏处理的时候，通常先处理widget中的一些交互，然后将内容传递给游戏绘制，绘制结束后，再绘制这些widget。**

## Implications and tradeoffs（影响和权衡）

### The Style and/or Layout issue

正如上文中交代的，layout问题是整个代码的调用相关的。在某些情况下，这是完全合适的，例如当用户界面需要非常动态和/或动画时。

在其他情况下，需要更好的审美控制，例如当艺术家或其他非程序员需要控制小部件的放置和外观时。考虑以下因素：

```c++
class Gui
{
public:
    Gui(const LookAndFeel& aLookAndFeel);
    const bool button(const Layout& aLayout, const char* aText);
}
```

通过用封装小部件外观细节的“look and feel”参数化Gui本身，可以支持任何可想象的数据驱动可视化方法。

另外，通过将每个小部件调用的布局信息对象化，可以使用数据驱动的布局对象，或者由外部编辑器控制，并在应用程序启动时加载

```c++
Gui myGui;
int myChoice(0);
Layout myLayouts[5];
void doSomeUserInterface()
{
    int i;
    for (i=0; i<5; i++)
    {
        if (myGui.radio(myChoice==i, myLayouts[i]))
            myChoice = i;
    }
}
```

如您所见，这样可以显著地清理代码，同时仍然允许代码在任何给定时间控制widget的“存在”。需要注意的是，这与完全对象化的小部件并不相同；我们只是将它们的布局信息对象化. 

这种方法可以在真正的即时模式风格下进行扩展，直接在Gui类中构建布局编辑。通过允许用户切换“布局编辑模式”，或者只需按住SHIFT键，就可以支持布局的点击和拖动样式编辑（有关实现的详细信息，请参阅高级功能部分）。 

### Frame shearing

IMGUI在实时应用程序的上下文中需要注意的一个方面（每秒不断地渲染新的帧很多次）是，用户交互总是响应于在前一帧上绘制的内容。这是因为用户界面必须至少绘制一次，这样用户才能意识到有一些小部件要与之交互。大多数情况下，如果帧速率足够高，这不会引起任何问题，但这是值得注意的.

任何给定的小部件交互的结果都有可能改变某些控制用户界面本身外观的应用程序状态，这种差异可能导致用户界面的某些部分反映“旧”状态，而另一些则反映“新”状态。我称之为“Frame shearing”，因为显示的图像同时代表两个不同逻辑图像的部分。

同样，在实时情况下（30 fps或更高），这通常对用户来说并不明显，但是，如果您不想/不能以交互速率显示gui（可能您有一些复杂的缓存方案），您需要采取一定的预防措施（这方面的一个例子是在web应用程序中使用IMGUI技术，有关更多详细信息，请参阅该部分)

要利用的主要技术是让任何改变用户界面外观的代码生成一个“Frame shearing异常”，该异常将打破为当前帧生成gui的方法，并重新启动当前帧的整个进程。理论上，在每个用户界面上，这个界面只会出现一次“Frame shearing”的情况。由于应用程序拥有所有状态，因此在更改此类状态时需要显式地抛出剪切异常。

## Advanced features

有些应用程序将通过概述的基本小部件，而其他应用程序则需要更先进的小部件和特性。组合框、树控件、滑块和编辑框等小部件类型，这些框比（例如）按钮复杂，有时是有用的，并且确实可以在imgui中实现，而不需要太多麻烦。

### How advanced do we want to be?

在深入了解这些小部件的细节之前，值得一提的是，这些更复杂的小部件类型中有许多都与保存屏幕实际属性有关（组合框、树控件、滑块和滚动功能）。我推测，这种功能存在的最初原因与动态更改和/或创建和销毁用户界面的难度/开销直接相关。

 使用IMGUI，小部件的存在由方法调用控制，并且布局可以完全动态和过程化。这意味着整个屏幕的内容可以在每帧的基础上发生变化，从而有效地使屏幕的实际容量无限大。这意味着不需要重叠窗口、滚动视图等。滚动的替代方法包括项目的“页面”，与使用选项卡/属性表代替多个窗口密切相关。 

从可用性的角度来看，我个人觉得不太依赖滚动或多个重叠窗口的用户界面更有效率。然而，用户期望和已建立的用户界面标准对于大量的应用程序仍然很重要。因此，我们现在来看一些更高级的widgettype。

### Tree controls

树控件基本上可以简化为一个可以展开和折叠的“节点”小部件。为了使客户端应用程序友好，需要向调用传递应用程序句柄/标识符

```c++
for (c = 0; c < NUM_CATEGORIES; c++)
{
    // return true if node is expand
    // application passes a const void* to identify the node
    // across frames (first param, can be anything which is 
    // unique and const across frame boundaries)
    // actual exapnd/collapse state is stored inside the gui
    // in a map (const void* <-> bool)
    if (myGui.node(CATEGORY_NAMES[c], x, y, CATEGORY_NAMES[c]))
    {
        // do gui for expanded node
    }
}
```

Gui类通过维护constvoid*和bool之间映射的内部集合来支持这一点，bool封装了每个逻辑节点的展开/折叠状态。这使客户机应用程序不再需要保持与应用程序数据项关联的gui相关状态。

```c++
bool Gui::node(const void* aHandle, const int aX, const int aY, const char* aLabel)
{
    bool& h(handleState(aHandle));
    String s;
    s.format(h ? "-%s" : "+%s", aLabel);
    if (doRadio(h, aX, aY, s))
        h = !h;
    return h;
}
```

### Combo boxes

组合框/下拉列表与树控件非常相似；它们可以使用相同的句柄概念实现。Gui管理展开/折叠功能，并且可以另外支持在组合框展开时禁用分配的小部件交互，以支持重叠（扩展列表相对于其他小部件）。

```c++
static const char* COMBO_CHOICES[] = 
{
    "orange",
    "pink",
    "red",
    "blue",
    NULL,
};
int myChoice(0);
myGui.combo(myChoice, 64, 64, COMBO_CHOICES);

// implementation
void Gui::combo(unsigned int& aChoice, const int aX, const int aY, const char** someChoices)
{
    bool& h(handleState(&aChoice));
    // expand
    if (h)
    {
        if (doButton(aX, aY, someChoices[aChoice]))
            h = false;
        // list
        unsigned int c(0);
        int y(aY);
        while(someChoices[c])
        {
            if (doRadio(c == aChoice, aX, y += buttonHeight(), someChoices[a]))
            {
                aChoice = c;
                h = false;
            }
            c++;
        }
    }
    // collapsed
    else
    {
        if (doRadio(h, aX, y += buttonHeight(), someChoices[a]))
            h = true;
    }
}
```

### Sliders / scrollbars

当滑块/滚动条被分解时，实际上只是一个背景矩形，一个显示当前值的可选文本，以及一个充当“拖动手柄”的矩形，定位在适当的位置，并具有适当的大小来表示当前值。图形用户界面将一个float*保留为当前值，通过单击拖动手柄内部进行设置，并通过向下移动鼠标来更新，以便跟踪要更新的值。

```c++
void Gui::horizontalSlider(const float aMax, const float aRange, float& aValue, 
                           const int aX, const int aY,
                           const int aWidth, const int aHeight)
{
    const int SIZE((int)(aWidth * aRange / aMax));
    // if this is the current one, update the value
    if (&value == myScrollValue)
    {
        aValue += sliderX() / (float)(aWidth - SIZE) * aMax;
        if (aValue < 0.f)
            aValue = 0.f;
        else if (aValue > aMax)
            aValue = aMax;
    }
    
    // display the value as text
    text(aX+aWidth/2, aY, String::format("%.1f", aValue));
    
    // draw a background
    rect(aX, aY, aWidth, aHeight, true);
    
    // do the dragabble thing (the slider)
    if (rect(aX + (int)((aWidth - SIZE) * aValue / aMax), aY, SIZE, aHeight, true) &&
        !myScrollValue && isButtonDown(VK_LBUTTON))
    {
        myScrollValue = &aValue;
    }
}
```

## Others

更多示例参见原文。

**此处主要目的是为了了解到什么是IMGUI，以及基于它，如何实现一些简单或复杂的Widget。**