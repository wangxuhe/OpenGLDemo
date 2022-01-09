/*
LearnOpenGL CN: https://learnopengl-cn.github.io/01%20Getting%20started/05%20Shaders/
Example: 三角形变色
Date: 2022.01.09
Other:
* 着色器(Shader)是运行在GPU上的小程序， 他只是一种把输入转换为输出，是非常独立的小程序
  他们之间不能相互通信，唯一的沟通就是输入和输出。
* 着色器的语言是GLSL类C语言编写， 主要特点是，开头声明版本， 然手输入输出变量， main函数
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

using namespace std;
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
 
//顶点着色器
const char *vertextSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

//片段着色器
/*
 添加uniform, 他是CPU想GPU着色器发送数据的方式， 全局
 */
const char *fragmentSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";
 
void changeSizeFunc(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // 点击ESC退出
        glfwSetWindowShouldClose(window, true);
    }
}

int getProgramId()
{
    int success;
    char infoLog[512];
    // 创建着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 将源码附加到着色器对象上
    glShaderSource(vertexShader, 1, &vertextSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // 编译着色器
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    // 检测着色器程序是否编译成功
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR: VertexShader Compile Fail:\n" << infoLog << endl;
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR: FragmentShader Compile Fail:\n" << infoLog << endl;
    }
    
    // 创建着色程序对象
    int shaderProgram  = glCreateProgram();
    // 将着色器附加到程序对象中，并对通过程序对象将这些着色器链接
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 检测链接是否成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR: ShaderProgram Link Fail" << infoLog << endl;
        return -1;
    }
    // 着色器对象链接到程序对象后，可以删除着色器对象，因为不再需要他们了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

void updateColor(int shaderProgram)
{
    // 获取运行的秒数
    double timeValue = glfwGetTime();
    float redValue = sin(timeValue)/5.0f + 0.5f;
    float greenValue = tan(timeValue)/2.0f + 0.5f;
    float blueValue = cos(timeValue)/2.0f + 0.5f;
    /*
     通过着色器程序ID获取着色器中uniform ourColor的位置值
     如果没有获取到，会返回-1
     注意，使用该接口，一定要保证glUseProgram已经使用
     */
    int location = glGetUniformLocation(shaderProgram, "ourColor");
    if(location == -1)
    {
        cout << "Error: not Get ourColor Location" << endl;
        return;
    }
    // 设置uniform值
    glUniform4f(location, redValue, greenValue, blueValue, 1.0f);
}
 
int main()
{
    // --------------------- 初始化 ---------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triange Shader", NULL, NULL);
    if(NULL == window){
        cout<<"Fail to Create Window"<<endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, changeSizeFunc);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // 测试，获取硬件支持顶点属性的最大数量上限
    int nAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nAttributes);
    cout << "Maxnum of vertex attribute supported: " << nAttributes << endl;
    
    // --------------------- 三角形 ---------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,

    };
    unsigned int VAO, VBO;
    // 生成指定的对象
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 绑定对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
 
    int shaderProgram = getProgramId();
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        // render
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        
        // 更新属性
        updateColor(shaderProgram);
        
        // 绘制图形
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}
