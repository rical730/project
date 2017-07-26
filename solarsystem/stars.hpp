//
//  star.hpp
//  solarsystem
//
//  likejiao 2017/7/26
//  
#ifndef star_hpp
#define star_hpp

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

/*
我们将星球分为：
1 普通能够自转并绕某个点公转的星球(Star), 
2 具有特殊材质的星球(Planet), 
3 能够发光的星球(LightPlanet)

为了编程实现上的方便，我们要对现实世界的实际编程模型做一些前提假设：
1 星球的运行轨道为圆形；
2 自转速度保持相同；
3 每次刷新绘制的时候假设时间经过了一天。

首先，我们可以考虑按下面的思路整个实现逻辑：
初始化星球对象;
1 初始化 OpenGL 引擎, 实现 onDraw 和 onUpdate;
2 星球应该自己负责处理自己的属性、绕行关系、变换相关绘制，因此在设计星球的类时应该提供一个绘制 draw() 方法;
3 星球也应该自己处理自己自转公转等更新显示的绘制，因此在设计星球类时候也应该提供一个更新方法 update();
4 在 onDraw() 中应调 用星球的 draw() 方法;
5 在 onUpdate() 中调用星球的 update() 方法;
6 在 onKeyboard() 键盘调整整个太阳系的显示.

进一步，对于每个星球而言，都具有如下的属性：
1 颜色 color
2 公转半径 radius
3 自转速度 selfSpeed
4 公转速度 speed
5 距离太阳中心的距离 distance
6 绕行的星球 parentStar
7 当前的自转的角度 alphaSelf
8 当前的公转角度 alpha
 */

// 普通能够自转并绕某个点公转的星球(Star)
class Star {
public:
    GLfloat radius; // 星球的运行半径
    GLfloat speed, selfSpeed; // 星球的公转、自传速度
    GLfloat distance; // 星球的中心与父节点星球中心的距离
    GLfloat rgbaColor[4]; // 星球的颜色

    Star* parentStar; // 父节点星球

    // 构造函数，构造一颗星球时必须提供
    // 旋转半径、旋转速度、自转速度、绕行(父节点)星球
    Star(GLfloat radius, GLfloat distance,
         GLfloat speed,  GLfloat selfSpeed,
         Star* parent);

    void drawStar(); // 对一般的星球的移动、旋转等活动进行绘制
    virtual void draw() { drawStar(); } // 提供默认实现，负责调用 drawStar()
    virtual void update(long timeSpan); // 参数为每次刷新画面时的时间跨度

protected:
    GLfloat alphaSelf, alpha;  // 当前的自转的角度,当前的公转角度
};


// 具有特殊材质的星球(Planet)
class Planet : public Star {
public:
    // 构造函数
    Planet(GLfloat radius, GLfloat distance,
           GLfloat speed,  GLfloat selfSpeed,
           Star* parent, GLfloat rgbColor[3]);
    void drawPlanet(); // 增加对具备自身材质的行星绘制材质
    virtual void draw() { drawPlanet(); drawStar(); } // 继续向其子类开放重写功能
};

// 能够发光的星球(LightPlanet)
class LightPlanet : public Planet {
public:
    LightPlanet(GLfloat Radius, GLfloat Distance,
                GLfloat Speed,  GLfloat SelfSpeed,
                Star* Parent, GLfloat rgbColor[]);
    void drawLight();
    virtual void draw() { drawLight(); drawPlanet(); drawStar(); } // 增加对提供光源的恒星绘制光照
};

#endif /* star_hpp */
