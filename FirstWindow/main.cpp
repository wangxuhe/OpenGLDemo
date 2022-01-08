/*
LearnOpenGL CN: https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/
Example: 窗口
Date: 2022.01.07
Other: 其他的示例窗口示例与其类似，不再添加注释
*/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// 改变视口大小回调
void ChangeViewCallBack(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 输入回调
void ProcessInput(GLFWwindow* window)
{
    // 检测ESC按键是否被按下
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // 设置关闭GLFW窗口
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, const char * argv[]) {
    // 初始化GLFW
    glfwInit();
    // 配置GLFW的主版本号和次版本号均为3，也就是使用OpenGL3.3版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /*
     配置GLFW使用的模式为核心模式(Core-profile)
     模式主要有两种：
     1. 立即渲染模式(Immediate mode), 也就是固定渲染管线
     2. 核心模式(Core-profile)，也就是可编程渲染管线
     */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Mac OSX 需要使用(其他系统可注释掉)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "First Window", NULL, NULL);
    if (NULL == window)
    {
        cout << "Error: create GLFW window Failed" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 设置窗口大小改变回调
    glfwSetFramebufferSizeCallback(window, ChangeViewCallBack);
    
    // GLAD主要用于管理OpenGL的函数指针，所以调用任何OpenGL的函数之前首先要初始化GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Error: Init GLAD Failed" << endl;
        return -1;
    }
    
    // 渲染循环，避免程序立即退出并关闭窗口
    // 检测下GLFW是否要求被退出
    while(!glfwWindowShouldClose(window))
    {
        // 检测输入按键相关
        ProcessInput(window);
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        /*
         屏幕是按照从左到右，由上而下的逐像素绘制而成的。
         最终的图像不是瞬间显示给用户， 而是一步步的生成的。
         如果采用单缓冲绘图就会存在图像闪烁的问题，因为使用双缓冲来渲染窗口。
         所有的绘制指定在后缓冲中绘制，前缓冲用于保存最终的输出的图像。
         通过交换前后缓冲，图像可以立即呈现出来。
         */
        glfwSwapBuffers(window);
        // 检测是否存在事件相关，用于更新窗口状态等
        glfwPollEvents();
    }
    // 循环结束后，用于释放分配所有的资源相关
    glfwTerminate();
    
    return 0;
}
