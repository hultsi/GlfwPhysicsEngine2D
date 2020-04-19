#include "./headers/GlfwHeaders.h"
#include <iostream>

GLFWwindow *window;

int initWindow();

int main(void)
{
    GlfwGameControl gameControl;

    // Define shapes
    GlfwSquare *sq = gameControl.createObject(GlfwSquare(430, 440, 200, 60, 20, false, "sq1", 30));
    sq->velocity.x = 5;
    sq->velocity.y = -2;
    GlfwSquare *sq2 = gameControl.createObject(GlfwSquare(720, 280, 280, 60, M_PI / 2, false, "sq2", 70));
    GlfwSquare *sq3 = gameControl.createObject(GlfwSquare(330, 440, 100, 60, 20, false, "sq3", 30));
    sq3->velocity.x = 5;
    sq3->velocity.y = -2;
    GlfwSquare *sq4 = gameControl.createObject(GlfwSquare(980, 380, 50, 60, M_PI / 2, false, "sq4", 30));
    sq4->velocity.x = -3;

    if (initWindow() == -1)
        return -1;

    /* Define keycallbacks */
    glfwSetWindowUserPointer(window, (void *)&gameControl);
    glfwSetKeyCallback(window, GlfwGameControl::keyCallback);

    // Reset performance calculation
    gameControl.resetPerformance();
    double msPerFrame;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Calculate performance
        msPerFrame = gameControl.getPerformance(false);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Update objects */
        gameControl.updateAll(msPerFrame);

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

//TODO: Move this elsewhere
int initWindow()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Physics engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /* Define view size as W_WIDTH and W_HEIGHT instead of -1 to 1 (also scaling and rotation) */
    glViewport(0.0f, 0.0f, W_WIDTH, W_HEIGHT);
    glLoadIdentity();
    glOrtho(0, W_WIDTH, 0, W_HEIGHT, 0, 1); //Screen size, rotation and scaling

    return 0;
}
