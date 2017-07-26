//
//  solarsystem.cpp
//  solarsystem
//
//  likejiao 2017/7/26
//  

/*
A x = By ，AB是矩阵，xy是向量，可以看出矩阵的本质是：描述运动。
于是在 OpenGL 内部有一个负责绘制变换的矩阵，这就是 OpenGL 中的矩阵模式。

矩阵既能够描述运动的变换，也能够描述某个物体所处的坐标系，
因此在处理不同的操作时，我们要给 OpenGL 设置不同的矩阵模式，这就需要用到
glMatrixMode(),接受三个不同的模式：

1 GL_PROJECTION 投影, 
向 OpenGL 声明将进行投影操作，会把物体头银到一个平面上。
开启这个模式后要使用glLoadIdentity()把矩阵设置为单位矩阵，
而后的操作如可以通过 gluPerspective 设置视景

2 GL_MODELVIEW 模型视图,
会向 OpenGL 声明接下来的语句将描绘一个以模型为基础的操作，
比如设置摄像机视角，开启后同样需要设置 OpenGL 的矩阵模式为单位矩阵。

3 GL_TEXTURE 纹理, 暂时用不到。
 */



#include "solarsystem.hpp"
#include <cstdlib>

// 我们约定本次实验的（摄像机）初始视角在 (x, -x, x) 处，则即有：
#define REST 700
#define REST_Y (-REST)
#define REST_Z (REST)

#define maxDepth 1.0
#define nearNormDepth 0.3
#define farNormalDepth 0.7

void SolarSystem::onDisplay() {

    glClear(GL_COLOR_BUFFER_BIT  |  GL_DEPTH_BUFFER_BIT); // 清除 viewport 缓冲区 //清楚深度缓存
    //glClearColor(.7f, .7f, .7f, .1f); // 清空并设置颜色缓存 灰色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);// 黑色背景
    // gluPerspective(GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar)。四个参数：
    // 1.第一个参数为视角的大小
    // 2.第二个参数为实际窗口的横纵比，如图中 aspect=w/h
    // 3.第三个参数为近平面距离
    // 4.第四个参数则为远平面距离
    // 这个函数会创建一个对称的透视型视景体，在使用这个函数前需要将 OpenGL 的矩阵模式设置为 GL_PROJECTION
    glMatrixMode(GL_PROJECTION); // 指定当前矩阵为投影矩阵
    glLoadIdentity(); // 将指定的矩阵指定为单位矩阵
    gluPerspective(75.0f, 1.0f, 1.0f, 40000000); // 指定当前的观察视景体


    glMatrixMode(GL_MODELVIEW); // 指定当前矩阵为视景矩阵堆栈应用术后的矩阵操作
    glLoadIdentity(); // 指定当前的矩阵为单位矩阵
    gluLookAt(viewX, viewY, viewZ, centerX, centerY, centerZ, upX, upY, upZ); // 设置视角的九个参数 // 定义视图矩阵，并与当前矩阵相乘

    glEnable(GL_LIGHT0); //启用一个光源(0号光源)
    glEnable(GL_LIGHTING); // 启用光源
    //glEnable(GL_DEPTH_TEST); // 启用深度测试，根据坐标的远近自动隐藏被遮住的图形
    //glDepthFunc(GL_LESS); //基准设置为 1.0，那么GL_LESS 则深度小余 1.0 的通过测试 

    //------------------------------------------------
    glEnable(GL_DEPTH_TEST); // 启用深度测试，http://blog.sina.com.cn/s/blog_4017fe890102v6rm.html
    // glClear(GL_DEPTH_BUFFER_BIT); //清楚深度缓存  第一行已经包含
    glDepthRange(nearNormDepth, farNormalDepth);
    glClearDepth(maxDepth);
    glDepthFunc(GL_LEQUAL); //基准设置为 1.0，那么GL_LESS 则深度小余 1.0 的通过测试 

    //先绘制不透明的太阳
    stars[0]->draw();

    // 接下来要绘制半透明的行星，所谓半透明是由有阴影变化，所以将深度缓冲设置为只读
    glDepthMask(GL_FALSE);
    // 实际绘制
    // for (int i=0; i<STARS_NUM; i++)//这种绘制顺序对于前后覆盖效果不好
    //     stars[i]->draw();
    
    for (int i=STARS_NUM-1; i>0; i--){
        // 太阳的问题解决了，接下来就要单独解决月亮轨迹的遮挡问题了
        if(i == 4){ // 在月亮前面绘制的是地球，序号是4，这样就可以实现地球对月亮的遮挡
            glDepthMask(GL_TRUE);
            stars[i]->draw();
            glDepthMask(GL_FALSE);
            continue;
        }
        // 正常的绘制
        stars[i]->draw();
    }

    // 完成半透明物体的绘制，将深度缓冲区恢复为可读可写的形式
    glDepthMask(GL_TRUE);
    // 以上的设置来源--  http://blog.163.com/jackie_howe/blog/static/199491347201231925236498/  
    // 成功了！！！绘制三维的半透明和不透明混合图形
    // ---------------------------------------------------
    
    // 我们在 main 函数中初始化显示模式时使用了 GLUT_DOUBLE
    // 需要使用 glutSwapBuffers 在绘制结束后实现双缓冲的缓冲区交换
    glutSwapBuffers();
}


#define TIMEPAST 1 // 假设每次更新都经过了一天
void SolarSystem::onUpdate() {
    for (int i=0; i<STARS_NUM; i++)
        stars[i]->update(TIMEPAST); // 更新星球的位置

    this->onDisplay();
}

// 按键操作变化视角
// w(+y方向)
// a(-x方向)
// d(+x方向)
// x(-y方向)
// s(+z方向)
// S(-z方向)
// r(reset)

// 每次按键移动的距离
#define OFFSET 20
void SolarSystem::onKeyboard(unsigned char key, int x, int y) {

    switch (key)    {
        case 'w': viewY += OFFSET; break; // 摄像机Y 轴位置增加 OFFSET
        case 's': viewZ += OFFSET; break;
        case 'S': viewZ -= OFFSET; break;
        case 'a': viewX -= OFFSET; break;
        case 'd': viewX += OFFSET; break;
        case 'x': viewY -= OFFSET; break;
        case 'r':
            viewX = 0; viewY = REST_Y; viewZ = REST_Z;
            centerX = centerY = centerZ = 0;
            upX = upY = 0; upZ = 1;
            break;
        case 27: exit(0); break;
        default: break;
    }

}

// 对于solarsystem.cpp的构造函数，我们需要对所有的星球进行初始化，
// 这里为了方便起见我们先给出适当的行星之间的参数：

// 公转半径
#define SUN_RADIUS 48.74
#define MER_RADIUS  7.32
#define VEN_RADIUS 18.15
#define EAR_RADIUS 19.13
#define MOO_RADIUS  6.15
#define MAR_RADIUS 10.19
#define JUP_RADIUS 42.90
#define SAT_RADIUS 36.16
#define URA_RADIUS 25.56
#define NEP_RADIUS 24.78

// 距太阳的距离
#define MER_DIS    62.06
#define VEN_DIS   115.56
#define EAR_DIS   168.00
#define MOO_DIS    26.01
#define MAR_DIS   228.00
#define JUP_DIS   333.40
#define SAT_DIS   428.10
#define URA_DIS   848.00
#define NEP_DIS   949.10

// 运动速度
#define MER_SPEED   87.0
#define VEN_SPEED  225.0
#define EAR_SPEED  365.0
#define MOO_SPEED   30.0
#define MAR_SPEED  687.0
#define JUP_SPEED 1298.4
#define SAT_SPEED 3225.6
#define URA_SPEED 3066.4
#define NEP_SPEED 6014.8

// 设置所有行星自转速度相同
#define SELFROTATE 3


/*
在 SolarSystem 中，包括太阳在内一共有九课星球（不包括冥王星），
但是在我们所设计的 Star 类中，每一个 Star 对象都具有 parentStar 的属性，
因此我们还可以额外实现这些星球的卫星，
比如围绕地球运行的月球，据此我们一共考虑实现十个星球。
于是我们可以设置如下枚举，用于索引一个数组中的星球：
*/


// 定义了星球的枚举
enum STARS {
    Sun,        // 太阳
    Mercury,    // 水星
    Venus,      // 金星
    Moon,       // 月球
    Earth,      // 地球
    Mars,       // 火星
    Jupiter,    // 木星
    Saturn,     // 土星
    Uranus,     // 天王星
    Neptune     // 海王星
};

// 为了方便操作数组，定义一个设置多为数组的宏
// 对太阳系星球的参数进行调整用的宏
#define SET_VALUE_3(name, value0, value1, value2) \
                   ((name)[0])=(value0), ((name)[1])=(value1), ((name)[2])=(value2)


// 实现SolarSystem类的构造函数，其中星球的颜色是随机选取的
SolarSystem::SolarSystem() {

    // 定义视角 
    // viewXYZ是（摄像机）初始视角在 (x, -x, x) 处
    // centerXYZ是所观察物体（太阳）的位置在 (0,0,0)
    // upXYZ是摄像机顶部向量
    viewX = 0;
    viewY = REST_Y;
    viewZ = REST_Z;
    centerX = centerY = centerZ = 0;
    upX = upY = 0;
    upZ = 1;

    // 太阳
    GLfloat rgbColor[3] = {1, 0, 0};
    stars[Sun]     = new LightPlanet(SUN_RADIUS, 0, 0, SELFROTATE, 0, rgbColor);
    // 水星
    SET_VALUE_3(rgbColor, .2, .2, .5);
    stars[Mercury] = new Planet(MER_RADIUS, MER_DIS, MER_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 金星
    SET_VALUE_3(rgbColor, 1, .7, 0);
    stars[Venus]   = new Planet(VEN_RADIUS, VEN_DIS, VEN_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 地球
    SET_VALUE_3(rgbColor, 0, 1, 0);
    stars[Earth]   = new Planet(EAR_RADIUS, EAR_DIS, EAR_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 月亮
    SET_VALUE_3(rgbColor, 1, 1, 0);
    stars[Moon]    = new Planet(MOO_RADIUS, MOO_DIS, MOO_SPEED, SELFROTATE, stars[Earth], rgbColor);
    // 火星
    SET_VALUE_3(rgbColor, 1, .5, .5);
    stars[Mars]    = new Planet(MAR_RADIUS, MAR_DIS, MAR_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 木星
    SET_VALUE_3(rgbColor, 1, 1, .5);
    stars[Jupiter] = new Planet(JUP_RADIUS, JUP_DIS, JUP_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 土星
    SET_VALUE_3(rgbColor, .5, 1, .5);
    stars[Saturn]  = new Planet(SAT_RADIUS, SAT_DIS, SAT_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 天王星
    SET_VALUE_3(rgbColor, .4, .4, .4);
    stars[Uranus]  = new Planet(URA_RADIUS, URA_DIS, URA_SPEED, SELFROTATE, stars[Sun], rgbColor);
    // 海王星
    SET_VALUE_3(rgbColor, .5, .5, 1);
    stars[Neptune] = new Planet(NEP_RADIUS, NEP_DIS, NEP_SPEED, SELFROTATE, stars[Sun], rgbColor);

}

// 在析构函数中释放申请的内存
SolarSystem::~SolarSystem() {
    for(int i = 0; i<STARS_NUM; i++)
        delete stars[i];
}
