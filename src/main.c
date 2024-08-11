#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "particle.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int window_width = 800;
const int window_height = 600;
const int MAX_PARTICLES = 2000;
const float TIME_STEP = 10.0 / 60.0f;
const float G = 6.67430e-11f;

void drawParticles(Particle* particles, int numParticles) {
    glBegin(GL_POINTS);
    for (int i = 0; i < numParticles; i++) {
        glColor3f(particles[i].r, particles[i].g, particles[i].b);
        glVertex2f(particles[i].x, particles[i].y);
    }
    glEnd();
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Gravitational Particle Simulation", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Initialize particles
    Particle particles[MAX_PARTICLES];
    initParticles(particles, MAX_PARTICLES);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Compute forces and update particles
        computeForces(particles, MAX_PARTICLES, G);
        updateParticles(particles, MAX_PARTICLES, TIME_STEP);

        // Draw particles
        drawParticles(particles, MAX_PARTICLES);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
