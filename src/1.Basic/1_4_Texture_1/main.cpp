/*
LearnOpenGL CN: https://learnopengl-cn.github.io/01%20Getting%20started/05%20Shaders/
Example: 读取着色器文件实现效果，添加纹理
Date: 2022.01.09
Other:
* 在Product -> Scheme -> Edit Scheme -> Options 勾选Use Custom working directory, 添加了Resources/shader的路径相关
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "learnopengl/shader.h"
#include <stb_image.h>

using namespace std;
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
 
void changeSizeFunc(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
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
    glfwSetFramebufferSizeCallback(window, changeSizeFunc);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    /*
     纹理坐标在XY(ST)轴间， 范围在[0,1]
     使用纹理坐标获取颜色叫做采样器(sample)
     */
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom left
        0.0f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f,   0.5f, 1.0f  // top left

    };
    unsigned int indices[] = {
        0, 1, 2,
    };
    
    unsigned int VAO, VBO, EBO;
    // 生成指定的对象
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // 绑定对象
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 设置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // 生成纹理
    unsigned int texture1;
    glGenBuffers(1, &texture1);
    glBindBuffer(GL_TEXTURE_2D, texture1);
    // 设置ST轴也就是XY轴的纹理环绕方式
    // 有四种样式：GL_REPEAT，GL_MIRRORED_REPEAT，GL_CLAMP_TO_EDGE，GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 设置缩放纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载图片数据
    int width, height, nrChannels;
    unsigned char *data = stbi_load("texture/container.jpg", &width, &height, &nrChannels, 0);
    if(data)
    {
        // 生成的纹理对象加载纹理数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // 生成多级纹理，多级纹理主要用于纹理被缩小的情况下
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Error: load texture failed" << endl;
    }
    // 在生成纹理后注意释放图像内存
    stbi_image_free(data);

    Shader shader("shader/texture_1.vs", "shader/texture_1.fs");
    shader.useProgram();
    
    glUniform1i(glGetUniformLocation(shader.ID, "ourTexture"), 0);

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        // render
        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1, &EBO);
    shader.deleteProgram();
    
    glfwTerminate();
    return 0;
}
