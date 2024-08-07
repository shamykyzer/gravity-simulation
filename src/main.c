#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "particle.h"
#include "quadtree.h"

const int window_width = 800;
const int window_height = 800;

void drawParticles(Particle* particles, int numParticles) {
    glBegin(GL_POINTS);
    for (int i = 0; i < numParticles; ++i) {
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

    int numParticles = 100;
    Particle* particles = (Particle*)malloc(numParticles * sizeof(Particle));
    if (!particles) {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    initParticles(particles, numParticles);

    Node root;
    root.width = 2.0f; // Assuming a width for the quadtree root node
    root.centerX = 0.0f;
    root.centerY = 0.0f;
    root.mass = 0.0f;
    root.hasParticle = 0;
    root.isLeaf = 1;
    for (int i = 0; i < 4; ++i) {
        root.children[i] = NULL;
    }

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Insert particles into the quadtree
        for (int i = 0; i < numParticles; ++i) {
            insertParticle(&root, &particles[i]);
        }

        // Compute forces on each particle
        for (int i = 0; i < numParticles; ++i) {
            computeForce(&root, &particles[i], 0.5f, 1.0f, 0.1f);
        }

        // Update particle positions
        updateParticles(particles, numParticles, 0.01f); // Correctly pass the time step

        // Draw particles
        drawParticles(particles, numParticles);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Free allocated memory
    free(particles);
    for (int i = 0; i < 4; ++i) {
        if (root.children[i]) {
            free(root.children[i]);
        }
    }

    glfwTerminate();
    return 0;
}
