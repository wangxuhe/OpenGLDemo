// 从《OpenGL 超级宝典》(第5版)移植而来

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLUT/GLUT.h>
#include <iostream>

GLBatch triangleBatch;
GLShaderManager shaderManager;

void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
}

void SetupRC()
{
    // blue backaground
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    
    // load up a triangle
    GLfloat vVerts[] = {
        -0.5f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    triangleBatch.Begin(GL_TRIANGLES, 3);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}

// called to draw scene
void RenderScene(void)
{
    // clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    GLfloat vRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
    triangleBatch.Draw();
    
    // perform the buffer swap to display the back buffer
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    gltSetWorkingDirectory(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(480, 320);
    glutCreateWindow("Triangle");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        return 1;
    }
    
    SetupRC();
    
    glutMainLoop();
    return 0;
}

