//
//  main.cpp
//  solarsystem
//
//  likejiao 2017/7/26
//  

/*
知识储备
OpenGL 包含了很多渲染函数，但是他们的设计目的是独立于任何窗口系统或操作系统的。
因此，它自身并没有包含创建打开窗口或者从键盘或鼠标读取时间的函数，甚至连最基本的显示窗口的功能都没有，
所以单纯只使用 OpenGL 是完全不可能创建一个完整的图形程序的。
并且绝大多数程序都需要与用户进行交互（响应键盘鼠标等操作）。GLUT 则提供了这一便利。

GLUT 其实是 OpenGL Utility Toolkit 的缩写，它是一个处理 OpenGL 程序的工具库


 */



// 使用 GLUT 的基本头文件
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// 工程的头文件，包含太阳系绘制的一系列函数
#include "solarsystem.hpp"

// 创建图形窗口的基本宏
#define WINDOW_X_POS 50
#define WINDOW_Y_POS 50
#define WIDTH 700
#define HEIGHT 700

SolarSystem solarsystem;

// 用于注册 GLUT 的回调
void onDisplay(void) {
    solarsystem.onDisplay();
}
void onUpdate(void) {
    solarsystem.onUpdate();
}
void onKeyboard(unsigned char key, int x, int y) {
    solarsystem.onKeyboard(key, x, y);
}

int main(int argc, char*  argv[]) {
    glutInit(&argc, argv); // 对 GLUT 进行初始化，并处理所有的命令行参数
    // 这个函数指定了使用 RGBA 模式韩式颜色索引模式。另外还可以指定是使用单缓冲还是双缓冲窗口。
    glutInitDisplayMode(GLUT_RGBA |  GLUT_DOUBLE | GLUT_DEPTH); // 这里我们使用 RGBA 和 双缓冲窗口
    glutInitWindowPosition(WINDOW_X_POS, WINDOW_Y_POS); // 设置窗口被创建时左上角位于屏幕上的位置
    glutInitWindowSize(WIDTH, HEIGHT); // 设置窗口被创建时的宽高, 为了简便起见
    glutCreateWindow("SolarSystem mb Likejiao"); // 创建一个窗口，输入的字符串为窗口的标题

    // glutDisplayFunc 的函数原型为 glutDisplayFunc(void (*func)(void))
    // 这是一个回调函数，每当 GLUT 确定一个窗口的内容需要更新显示的时候，glutDisplayFunc 注册的回调函数就会被执行.
    glutDisplayFunc(onDisplay);

    // glutIdleFunc(void (*func)(void)) 将指定一个函数，用于处理当事件循环
    // 处于空闲的时候，就执行这个函数。这个回调函数接受一个函数指针作为它的唯一参数
    glutIdleFunc(onUpdate);

    // glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
    // 会将键盘上的键与一个函数关联，当这个键被按下或者释放时，函数就会调用
    glutKeyboardFunc(onKeyboard);

    
    glutMainLoop();
    return 0;
}


