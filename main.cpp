#include "./headers/GlfwHeaders.h"
#include <iostream>

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

    /* Define view size as W_WIDTH and W_HEIGHT instead of -1 to 1 (also scaling and rotation) */
    glViewport(0.0f, 0.0f, W_WIDTH, W_HEIGHT);
    glLoadIdentity();
    glOrtho(0, W_WIDTH, 0, W_HEIGHT, 0, 1); //Screen size, rotation and scaling

    /* Define keycallbacks */
    glfwSetWindowUserPointer(window, (void *)&gameControl);
    glfwSetKeyCallback(window, GlfwGameControl::keyCallback);

    // Define shapes
    GlfwSquare *sq = gameControl.createObject(GlfwSquare(430, 440, 200, 60, 20, false, 30));
    sq->velocity.x = 4;
    sq->velocity.y = -1;
    GlfwSquare *sq2 = gameControl.createObject(GlfwSquare(720, 280, 280, 60, M_PI / 2, false, 70));

    GlfwSquare *sq3 = gameControl.createObject(GlfwSquare(90, 50, W_WIDTH - 91, 80, 0, false, 170));
    sq3->velocity.x = 1;
    sq3->velocity.y = 1;
    GlfwSquare *sq4 = gameControl.createObject(GlfwSquare(100, 700, 600, 100, 0, false, 60));
    sq4->velocity.y = -.5;
    //gameControl.createObject(GlfwSquare(500, 240, 200, 60, 0, false, 20));

    //gameControl.createObject(GlfwSquare(60, 650, 100, 30, 0, false, 120));

    // Reset performance calc
    gameControl.resetPerformance();
    double msPerFrame;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Calculate performance
        msPerFrame = gameControl.getPerformance(false);
        // std::cout << msPerFrame << "\n";

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

/*
FPS LIMITER

double lasttime = glfwGetTime();
while (!glfwWindowShouldClose(window)) {
    myGameUpdate();
    while (glfwGetTime() < lasttime + 1.0/TARGET_FPS) {
        // TODO: Put the thread to sleep, yield, or simply do nothing
    }
    lasttime += 1.0/TARGET_FPS;
}
*/
