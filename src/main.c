#include "particle.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_PARTICLES 3000

int main(int argc, char** argv) {
    // Declare a pointer for particles
    Particle* particles = (Particle*)malloc(MAX_PARTICLES * sizeof(Particle));
    if (!particles) {
        fprintf(stderr, "Failed to allocate memory for particles\n");
        return -1;
    }

    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        free(particles); // Free the allocated memory before returning
        return -1;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Gravitational Particle Simulation", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        free(particles); // Free the allocated memory before returning
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        free(particles); // Free the allocated memory before returning
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
    free(particles);
    glfwDestroyWindow(window); // Properly destroy the window before termination
    glfwTerminate();
    return 0;
}
