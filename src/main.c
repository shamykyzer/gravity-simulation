#include "particle.h"
#include "quadtree.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

const int window_width = 1200;
const int window_height = 900;
const int MAX_PARTICLES = 10000;

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
    initParticles(particles, MAX_PARTICLES);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        QuadNode* root = createNode(-1.0f, -1.0f, 1.0f, 1.0f);
        for (int i = 0; i < MAX_PARTICLES; i++) {
            insertParticle(root, &particles[i]);
        }

        for (int i = 0; i < MAX_PARTICLES; i++) {
            computeForce(root, &particles[i], 0.5f, 1.0f);
        }

        updateParticles(particles, MAX_PARTICLES, 0.01f);
        drawParticles(particles, MAX_PARTICLES);

        freeQuadtree(root);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
