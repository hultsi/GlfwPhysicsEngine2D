#include <iostream>
#include "./headers/GlfwHeaders.h"

int main(void)
{
    GLFWwindow *window;
    GlfwGameControl gameControl;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /* Define callbacks */
    glfwSetKeyCallback(window, GlfwGameControl::keyCallback);

    // Define shapes
    gameControl.createObject(GlfwSquare(1, 1, W_WIDTH - 1, 80));
    gameControl.createObject(GlfwSquare(500, 400, 200, 60, false, 20), true);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw objects */
        gameControl.drawAll();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
