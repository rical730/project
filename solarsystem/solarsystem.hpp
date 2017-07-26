//
//  solarsystem.hpp
//  solarsystem
//
//  likejiao 2017/7/26
//  
#ifndef solarsystem_hpp
#define solarsystem_hpp


#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include "stars.hpp"

#define STARS_NUM 10

/*
我们还需要考虑太阳系类的设计。在太阳系中，太阳系显然是由各个行星组成的；
并且，对于太阳系而言，太阳系中行星运动后的视图刷新应该由太阳系来完成。
据此太阳系成员变量应为包含行星的变量，成员函数应作为处理太阳系内的视图刷新及键盘响应等事件
 */ 

class SolarSystem {

public:

    SolarSystem();
    ~SolarSystem();

    void onDisplay();
    void onUpdate();
    void onKeyboard(unsigned char key, int x, int y);

private:
    Star *stars[STARS_NUM];

    // 定义观察视角的参数, OpenGL 三维编程中摄像机视角的概念,把摄像机想象成我们自己的头
    GLdouble viewX, viewY, viewZ; // 相当于头（摄像机）在 OpenGL 世界坐标中的坐标位置
    GLdouble centerX, centerY, centerZ; // 相当于头所看（摄像机所拍）物体的坐标位置
    GLdouble upX, upY, upZ; // 相当于头顶（摄像机顶部）朝上的方向向量（因为我们可以歪着头观察一个物体）。

};

#endif /* solarsystem_hpp */


