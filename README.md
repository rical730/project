# project
some interesting projects

# 1 solarsystem 太阳系仿真模拟系统
## 1.1介绍
 * 使用 OpenGL GLUT 编写一个简单的太阳系运行系统
## 1.2背景
 * OpenGL 包含了很多渲染函数，但是他们的设计目的是独立于任何窗口系统或操作系统的。因此，它自身并没有包含创建打开窗口或者从键盘或鼠标读取时间的函数，甚至连最基本的显示窗口的功能都没有，所以单纯只使用 OpenGL 是完全不可能创建一个完整的图形程序的。并且绝大多数程序都需要与用户进行交互（响应键盘鼠标等操作）。GLUT 则提供了这一便利。
 * GLUT 其实是 OpenGL Utility Toolkit 的缩写，它是一个处理 OpenGL 程序的工具库，主要负责处理与底层操作系统的调用及 I/O操作。使用 GLUT 可以屏蔽掉底层操作系统 GUI 实现上的一些细节，仅使用 GLUT 的 API 即可跨平台的创建应用程序窗口、处理鼠标键盘事件等等。

## 1.3运行
```
git clone https://www.github.com/rical730/project.git
cd project/solarsystem/
./solarsystem
```
详见`project/solarsystem/`里的readme.txt

## 1.4特点
 * 基本的 Makefile
 * 基本的 OOP 编程思想
 * OpenGL GLUT 的结构基本使用
 * OpenGL 三维编程中摄像机视角的概念
 * 使用双缓冲
 * 修复了景深前后的问题
 * 修复了星球半透明问题
 * 使得仿真系统更加逼真

## 1.5 可进一步优化
 * 给不同星期绘制纹理

## V2.0程序运行仿真图
![](https://github.com/rical730/project/blob/master/solarsystem/view-solarsystem-v2.0.png)