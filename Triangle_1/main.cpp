#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

void Render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(0.0f, 0.5f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0.5f, -0.5f);
    }
    glEnd();
}

int main(int argc, const char * argv[])
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(480, 320, "Triganle", NULL, NULL);
    if(NULL == window)
    {
        cout << "Error: Create GLFW window Failed" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    while(!glfwWindowShouldClose(window)){
        Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}
