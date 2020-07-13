# 源代码实现 - octrees（2）- 碰撞检测

在前面相关的文章中介绍了，[Octrees的一些概念](https://www.cnblogs.com/grass-and-moon/p/13264962.html)，以及[Octrees插入对象的实现](https://www.cnblogs.com/grass-and-moon/p/13266715.html)。

下面需要实现以下八叉树内的对象碰撞检测的操作。基于八叉树的碰撞检测的基本原理为：遍历八叉树，将子节点的对象和所有祖先节点中的对象进行碰撞检测。

## 球的碰撞检测

球的碰撞检测很简单，只要两个球的球心之间的距离小于半径距离之和，说明两个球发生了碰撞。完整代码如下：

```c++
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <cmath>

const int MAX_DEPTH = 2;

struct Point {
    Point() {}
    Point(float xi, float yi, float zi) : x(xi), y(yi), z(zi) {}
    float x{ 0.0f };
    float y{ 0.0f };
    float z{ 0.0f };

    float operator[](int i) {
        float b[3] = { x,y,z };
        return b[i];
    }
};

Point operator+(const Point& a, const Point& b) {
    return Point{ a.x + b.x, a.y + b.y, a.z + b.z };
}

std::ostream& operator<<(std::ostream& os, const Point& pt) {
    os << "{" << pt.x << ", " << pt.y << ", " << pt.z << "}";
    return os;
}

struct Object {
    Point center{ 0.0f, 0.0f, 0.0f };
    float radius{ 0.0f };
    Object* pNextObject{ nullptr };
};

std::ostream& operator<<(std::ostream& os, const Object& obj) {
    os << "Obj Center: " << obj.center << " radius: " << obj.radius;
    return os;
}

struct Node {
    Point center{ 0.0f, 0.0f, 0.0f };   //< The center of the node
    float halfWidth{ 0.0f };            //< The half width of the node
    Node* pChild[8]{ nullptr };         //< The child of the node
    Object* pObjList{ nullptr };        //< The objects in the node
};

std::ostream& operator<<(std::ostream& os, const Node& node) {
    os << "Node Center: " << node.center << " halfWidth: " << node.halfWidth;
    return os;
}

// Preallocates an octree down to a specific depth
Node* BuildOctree(Point center, float halfWidth, int stopDepth)
{
    if (stopDepth < 0) return NULL;
    else {
        // Construct and fill in 'root' of this subtree
        Node* pNode = new Node;
        pNode->center = center;
        pNode->halfWidth = halfWidth;
        pNode->pObjList = NULL;

        // Recursively construct the eight children of the subtree
        Point offset;
        float step = halfWidth * 0.5f;
        for (int i = 0; i < 8; ++i) {
            // 赞
            offset.x = ((i & 1) ? step : -step);
            offset.y = ((i & 2) ? step : -step);
            offset.z = ((i & 4) ? step : -step);
            pNode->pChild[i] = BuildOctree(center + offset, step, stopDepth - 1);
        }
        return pNode;
    }
}

// Insert object
void InsertObject(Node* pTree, Object* pObject)
{
    int index = 0, straddle = 0;
    // Compute the octant number [0..7] the object sphere center is in
    // If straddling any of the dividing x, y, or z planes, exit directly
    for (int i = 0; i < 3; ++i) {
        float delta = pObject->center[i] - pTree->center[i];
        if (std::abs((double)delta) < pTree->halfWidth + pObject->radius) { // TODO CHECK
            straddle = 1;
            break;
        }
        if (delta > 0.0f) index |= (1 << i);
    }

    if (!straddle && pTree->pChild[index]) {
        InsertObject(pTree->pChild[index], pObject);
    }
    else {
        pObject->pNextObject = pTree->pObjList;
        pTree->pObjList = pObject;
    }
}

std::string GenerateKey(Object* pA, Object* pB)
{
    Object* lower = pA < pB ? pA : pB;
    Object* higher = pA > pB ? pA : pB;

    std::ostringstream address;
    address << lower << higher;
    return address.str();
}

bool TestCollision(Object* pA, Object* pB)
{
    float x = pA->center[0] - pB->center[0];
    float y = pA->center[1] - pB->center[1];
    float z = pA->center[2] - pB->center[2];

    float length = std::sqrt(x * x + y * y + z * z);
    return length < pA->radius + pB->radius;
}

// Test all objects that could possibly overlap due to cell ancestry and coexistence
// in the same cell. Assumes objects exist in a single cell only, and fully inside it
std::map<std::string, std::pair<Object*, Object*>> TestAllCollisions(Node* pTree)
{
    // Keep track of all ancestor object lists in a stack
    static Node* ancestorStack[MAX_DEPTH];
    static int depth = 0;


    // Check collision between all objects on this level and all
    // all ancestor objects. The current level is included as its
    // ancestor so all nessary pairwise tests are done
    std::map<std::string, std::pair<Object*, Object*>> result;
    ancestorStack[depth++] = pTree;
    std::string key;
    for (int n = 0; n < depth; n++) {
        Object* pA, * pB;
        for (pA = ancestorStack[n]->pObjList; pA; pA = pA->pNextObject) {
            for (pB = pTree->pObjList; pB; pB = pB->pNextObject) {
                if (pA == pB) break;
                key = GenerateKey(pA, pB);
                if (result.count(key) == 0 && TestCollision(pA, pB)) {
                    result[key] = std::make_pair(pA, pB);
                }
            }
        }
    }

    std::map<std::string, std::pair<Object*, Object*>> childResult;
    // Recursively visit all existing children
    for (int i = 0; i < 8; ++i) {
        if (pTree->pChild[i]) {
            childResult = TestAllCollisions(pTree->pChild[i]);
            result.insert(childResult.begin(), childResult.end());
        }
    }

    // Remove current node from ancestor stack before returning
    depth--;
    return result;
}

void PrintCollisionInfo(const std::map<std::string, std::pair<Object*, Object*>>& mapInterObjects)
{
    for (auto iter = mapInterObjects.begin(); iter != mapInterObjects.end(); ++iter) {
        std::cout << "Object: " << *(iter->second.first) << " Intersects Object: " << *(iter->second.second) << std::endl;
    }
}

int main()
{
    // Assuming the depth of octree is 1
    // The root center is (0.0, 0.0, 0.0)
    // The root width is 2.0
    Node* pRoot = BuildOctree(Point{ 0.0, 0.0, 0.0 }, 1.0, MAX_DEPTH-1);

    Object objs[3];
    objs[0].radius = 0.9;
    objs[0].center = Point{ 0.0, 0.0, 0.0 };
    objs[1].radius = 0.1;
    objs[1].center = Point{ -0.5, -0.5, -0.5 };
    objs[2].radius = 0.3;
    objs[2].center = Point{0.1, 0.2, 0.3};

    InsertObject(pRoot, &objs[0]);
    InsertObject(pRoot, &objs[1]);
    InsertObject(pRoot, &objs[2]);

    std::cout << "=================================" << std::endl;
    auto result = TestAllCollisions(pRoot);
    PrintCollisionInfo(result);

    return 0;
}
```

输出结果为：

```
=================================
Object: Obj Center: {0, 0, 0} radius: 0.9 Intersects Object: Obj Center: {-0.5, -0.5, -0.5} radius: 0.1
Object: Obj Center: {0, 0, 0} radius: 0.9 Intersects Object: Obj Center: {0.1, 0.2, 0.3} radius: 0.3
```

经过test确定，之前怀疑`TestAllCollisions`有问题的地方是没有问题的。

## 三角面片之间的碰撞检测

要实现三角形碰撞检测，最关键的地方就是需要判断两个三角形是不是相交。
