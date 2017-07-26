#
#  Makefile
#
#  likejiao 2017/7/26
#

CXX = g++
EXEC = solarsystem
SOURCES = main.cpp stars.cpp solarsystem.cpp
OBJECTS = main.o stars.o solarsystem.o
LDFLAGS = -lglut -lGL -lGLU -std=c++11

all :
	$(CXX) $(SOURCES) $(LDFLAGS) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o

 # 书写编译命令时注意 -lglut -lGLU -lGL 的位置
 # 这是因为 g++ 编译器中 -l 选项用法有点特殊：
 # 所以 -l 选项的库应该存在于所有编译文件的右边