//
//  star.cpp
//  solarsystem
//
//  likejiao 2017/7/26
//  
#include "stars.hpp"
#include <cmath>

#define PI 3.1415926535



Star::Star(GLfloat radius, GLfloat distance,
           GLfloat speed,  GLfloat selfSpeed,
           Star* parent) {
    this->radius = radius;
    this->selfSpeed = selfSpeed;
    this->alphaSelf = this->alpha = 0;
    this->distance = distance;

    for (int i = 0; i < 4; i++)
        this->rgbaColor[i] = 1.0f;

    this->parentStar = parent;
    if (speed > 0)
        this->speed = 360.0f / speed;
    else
        this->speed = 0.0f;
}


void Star::drawStar() {

    glEnable(GL_LINE_SMOOTH);
    
    glEnable(GL_BLEND); // 透明模式，不会出现景深被覆盖
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int n = 1440;

    //注意：我们在绘制、旋转对象之前，一定要通过 glPushMatrix 保存当前的矩阵环境，否则会出现莫名其妙的绘制错误。
    glPushMatrix();
    {
        // 公转

        // 如果是行星，且距离不为0，那么 且向原点平移一个半径
        // 这部分用于处理卫星
        if (parentStar != 0 && parentStar->distance > 0) {
            glRotatef(parentStar->alpha, 0, 0, 1); //将绘制的图形沿 z 轴旋转 alpha
            glTranslatef(parentStar->distance, 0.0, 0.0); // x 轴方向上平移 distance , y,z 方向不变
        }

        // 绘制运行轨道
        glBegin(GL_LINES); //GL_LINES 表示绘制依次画出的点及他们之间的连线, 用很多边的多边形来模拟圆形
        for(int i=0; i<n; ++i)
            glVertex2f(distance * cos(2 * PI * i / n),
                       distance * sin(2 * PI * i / n));
        glEnd();

        glRotatef(alpha, 0, 0, 1); // 绕 z 轴旋转 alpha
        glTranslatef(distance, 0.0, 0.0); // x 轴方向平移 distance, y,z 方向不变

        // 自转
        glRotatef(alphaSelf, 0, 0, 1);

        // 绘制行星颜色
        glColor3f(rgbaColor[0], rgbaColor[1], rgbaColor[2]);
        glutSolidSphere(radius, 40, 32);

    }
    glPopMatrix(); // 恢复绘制前的矩阵环境


}

// 行星在绘制时，首先要考虑自身的公转角度和自转角度
void Star::update(long timeSpan) {
    alpha += timeSpan * speed;  // 更新角度
    alphaSelf += selfSpeed;     // 更新自转角度
}


Planet::Planet(GLfloat radius, GLfloat distance,
               GLfloat speed,  GLfloat selfSpeed,
               Star* parent, GLfloat rgbColor[3]) :
Star(radius, distance, speed, selfSpeed, parent) {
    rgbaColor[0] = rgbColor[0];
    rgbaColor[1] = rgbColor[1];
    rgbaColor[2] = rgbColor[2];
    rgbaColor[3] = 1.0f;
}

// 对于 Planet 类而言，属于不发光的星球，我们要绘制它的光照效果：
void Planet::drawPlanet() {
    GLfloat mat_ambient[]  = {0.0f, 0.0f, 0.5f, 1.0f};
    GLfloat mat_diffuse[]  = {0.0f, 0.0f, 0.5f, 1.0f};
    GLfloat mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};
    GLfloat mat_emission[] = {rgbaColor[0], rgbaColor[1], rgbaColor[2], rgbaColor[3]};
    GLfloat mat_shininess  = 90.0f; // 发光

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION,  mat_emission); // 其他的材质设置都需要设置多个值，这是需要使用数组进行设置
    glMaterialf (GL_FRONT, GL_SHININESS, mat_shininess); // 镜面指数只需要设置一个数值
}


/*
在 OpenGL 里要实现光照系统，我们首先需要做的就是设置光源。
OpenGL 内只支持有限数量的光源（八个）分别使用 GL_LIGHT0 至 GL_LIGHT7 这八个宏来表示。
通过 glEnable 函数来启用， glDisable 函数来禁用。例如：glEnable(GL_LIGHT0);

设置光源位置则需要使用 glLightfv glMaterialfv 进行设置

GLfloat 数组的四个参数：
(x,y,z,w) 其中当 w为0时，表示该光源位于无限远，而 x,y,z 便指定了这个无限远光源的方向；
当 w 不为0时，表示 位置性光源，其位置为 (x/w, y/w, z/w) 。

 */

LightPlanet::LightPlanet(GLfloat radius,    GLfloat distance, GLfloat speed,
                         GLfloat selfSpeed, Star* parent,   GLfloat rgbColor[3]) :
Planet(radius, distance, speed, selfSpeed, parent, rgbColor) {
    ;
}

// 对于 LightPlanet 类来说，属于发光的星球，所以我们不但要设置其光照材质，还要设置其光源位置：
void LightPlanet::drawLight() {

    GLfloat light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_ambient[]  = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat light_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // 指定零号光源的位置
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);  // 表示各种光线照射到该材质上，经过很多次反射后追踪遗留在环境中的光线强度
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);  // 漫反射后的光照强度
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); // 镜面反射后的光照强度

}
