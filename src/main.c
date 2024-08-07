#include "particle.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_PARTICLES 3000

Particle particles[MAX_PARTICLES];

int main(int argc, char** argv) {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Gravitational Particle Simulation", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Initialize particles
    initParticles(particles, MAX_PARTICLES);

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Update particles using the quadtree
        updateParticles(particles, MAX_PARTICLES);

        // Render particles
        glBegin(GL_POINTS);
        for (int i = 0; i < MAX_PARTICLES; ++i) {
            glColor3f(particles[i].r, particles[i].g, particles[i].b);
            glVertex2f(particles[i].x, particles[i].y);
        }
        glEnd();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glfwTerminate();
    return 0;
}
