/*
LearnOpenGL CN: https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle
Example: 三角形着色器
Date: 2022.01.07
Other:
 * 着色器有两种： 顶点着色器(VertexShader)和片段着色器(FragmentShader)
 * 顶点着色器主要用于坐标转换； 片段着色器主要逐像素点绘制颜色
 * 着色器的语言使用的是GLSL语言，语言构成类C
 * 创建角色器有两种方式：
    1. 将源代码相关编写为C风格的字符串
    2. 生成指定格式的文件，顶点着色器后缀名为.vs 片段着色器后缀名为.fs
    方式1比方式2效率更高，它避免了程序读取文件的时间相关；但是改写不如方式2直观，易于修改
 * 着色器的创建步骤：
    1. 通过glCreateShader创建指定类型的着色器对象Shader
    2. 通过glShaderSource将着色器源码附加到着色器对象上
    3. 通过glCompileShader编译着色器对象(注意检测是否编译成功)
    4. 通过glCreateProgram创建着色器程序对象Program, 它的主要作用是将所有多个着色器合并并链接到一起，用于绘制
    5. 通过glAttachShader将着色器对象附加到程序对象中，
    6. 通过glLinkProgram对着色器对象链接(注意检测是否链接成功)
    6. 通过glDeleteShader删除指定的着色器对象
    7. 通过glUseProgram使用程序对象进行渲染
 * 调用着色器顶点属性相关
    1. 生成顶点数组对象(VAO)， 顶点缓冲对象(VBO)， VAO会将VBO的数据存储起来
    2. 绑定对应的对象
    3. 将顶点数据复制到缓冲的内存中， 并设置顶点属性的指针，用于解析
    4. 启用顶点属性
    注意：
    1. 关于VBO，VAO相关，优先创建VAO，绑定VAO
    2. VBO在储存到VAO后，可以解除绑定
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
 
//顶点着色器
const char *vertextSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

//片段着色器
const char *fragmentSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
 
void ChangeSizeFunc(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void pressInput(GLFWwindow* window){
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        // 点击ESC退出
        glfwSetWindowShouldClose(window, true);
    }
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        // 点击LEFT切换为线段绘制
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        // 点击RIGHT切换为填充绘制
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
 
int main()
{
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
    glfwSetFramebufferSizeCallback(window, ChangeSizeFunc);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
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
    int shaderProgram = glCreateProgram();
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
    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,     // 左下角
        0.5f, -0.5f, 0.0f,      // 右下角
        -0.5f, 0.5f, 0.0f,      // 左上角
        0.5f, 0.5f, 0.0f        // 右上角
    };
    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };
    unsigned int VAO;       // 顶点数组对象(Vertex Array Object VAO)
    unsigned int VBO;       // 顶点缓冲对象(Vertex Buffer Object VBO)
    unsigned int EBO;       // 索引缓冲对象
    // 生成指定的对象
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 绑定对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    /*
    将数据复制到缓冲的内存中
    参数1: 目标缓冲类型
    参数2: 指定传输数据的大小，以字节为单位
    参数3: 发送的实际数据
    参数4: 显卡管理给定数据的类型，有三种形式：
       GL_STATIC_DRAW: 数据几乎不会发生改变
       GL_DYNAMIC_DRAW: 数据会改变很多
       GL_STREAM_DRAW: 数据每次绘制都会改变
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 设定顶点属性指针, 告知OpenGL如何解析数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 启用顶点属性
    glEnableVertexAttribArray(0);
    
    // 解除VBO的绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
 
    while(!glfwWindowShouldClose(window))
    {
        pressInput(window);
        
        // render
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 激活程序对象
        glUseProgram(shaderProgram);
        // 绘制图形
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}

