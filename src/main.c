#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "particle.h"
#include "quadtree.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h> // For memcpy

const int window_width = 900;
const int window_height = 600;
const int MAX_PARTICLES = 8000;
const float MAX_VELOCITY = 1e1; // Cap the maximum velocity
const float TIME_STEP = 1;   // Fixed time step

void drawInterpolatedParticles(Particle* particles, Particle* prevParticles, int numParticles, float alpha) {
    glBegin(GL_POINTS);
    for (int i = 0; i < numParticles; i++) {
        glColor3f(particles[i].r, particles[i].g, particles[i].b);
        float interpolatedX = particles[i].x * alpha + prevParticles[i].x * (1.0f - alpha);
        float interpolatedY = particles[i].y * alpha + prevParticles[i].y * (1.0f - alpha);
        glVertex2f(interpolatedX, interpolatedY);
    }
    glEnd();
}

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Gravity Simulation", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Particle particles[MAX_PARTICLES];
    Particle prevParticles[MAX_PARTICLES];
    initParticles(particles, MAX_PARTICLES);

    double previousTime = glfwGetTime();
    double accumulator = 0.0;

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double frameTime = currentTime - previousTime;
        previousTime = currentTime;
        accumulator += frameTime;

        while (accumulator >= TIME_STEP) {
            memcpy(prevParticles, particles, sizeof(Particle) * MAX_PARTICLES);

            QuadNode* root = createNode(-1.0f, -1.0f, 1.0f, 1.0f);
            for (int i = 0; i < MAX_PARTICLES; i++) {
                insertParticle(root, &particles[i]);
            }

            for (int i = 0; i < MAX_PARTICLES; i++) {
                computeForce(root, &particles[i], 0.5f, 1.0f);
            }

            updateParticles(particles, MAX_PARTICLES, TIME_STEP);
            freeQuadtree(root);

            accumulator -= TIME_STEP;
        }

        float alpha = accumulator / TIME_STEP;
        glClear(GL_COLOR_BUFFER_BIT);
        drawInterpolatedParticles(particles, prevParticles, MAX_PARTICLES, alpha);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
