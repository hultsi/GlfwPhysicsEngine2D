#include "./headers/GlfwHeaders.h"
#include <iostream>

GLFWwindow *window;

int initWindow();

int main(void)
{
    GlfwGameControl gameControl;

    // Fix the weird corner case !!!
    // Define shapes
    GlfwSquare *sq = gameControl.createObject(GlfwSquare(430, 440, 200, 60, 1, false, "sq1", 230));
    sq->velocity.x = 8;
    sq->velocity.y = -6;
    GlfwSquare *sq2 = gameControl.createObject(GlfwSquare(720, 280, 280, 60, 0, false, "sq2", 230));
    GlfwSquare *sq3 = gameControl.createObject(GlfwSquare(230, 440, 100, 60, 20, false, "sq3", 30));
    sq3->velocity.x = 5;
    sq3->velocity.y = -2;
    GlfwSquare *sq4 = gameControl.createObject(GlfwSquare(980, 380, 50, 60, M_PI / 2, false, "sq4", 30));
    sq4->velocity.x = -2;
    GlfwSquare *sq5 = gameControl.createObject(GlfwSquare(680, 380, 70, 60, M_PI, false, "sq5", 30));
    sq5->velocity.y = -3;
    GlfwSquare *sq1 = gameControl.createObject(GlfwSquare(300, 380, 50, 60, M_PI / 2, false, "sq6", 30));
    sq1->velocity.x = 12;
    // GlfwSquare *sq2 = gameControl.createObject(GlfwSquare(500, 380, 50, 60, M_PI / 2, false, "sq2", 130));
    // sq2->velocity.x = 0;
    // GlfwSquare *sq3 = gameControl.createObject(GlfwSquare(700, 380, 50, 60, M_PI / 2, false, "sq3", 130));
    // sq3->velocity.x = -17;
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
        gameControl.updateAllBegin();
        gameControl.updateAll();
        gameControl.updateAllEnd();

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
