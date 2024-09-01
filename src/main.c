#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "particle.h"
#include "shader_utils.h"
#include "controls.h"
#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Particles particles;  // Declare particles globally

void updateParticlesAndForces(Particles* particles, int numParticles, float dt) {
    computeForces(particles, numParticles, GRAVITY_CONST);
    updateParticles(particles, numParticles, dt);
    handleBoundaryCollisions(particles, numParticles, -1.0f, 1.0f, -1.0f, 1.0f);
}

int main(int argc, char* argv[]) {
    srand(time(NULL));  // Ensure different seed for each process

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 960, "Particle Simulation", NULL, NULL);
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

    initControls(window);  // Initialize controls

    initParticles(&particles, MAX_PARTICLES);  // Initialize particles

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float dt = 0.016f;  // Simulate a fixed timestep (approx. 60 FPS)

        handleInput(window, &particles, MAX_PARTICLES);  // Handle input
        applyAttraction(&particles, MAX_PARTICLES, centerX, centerY, ATTRACTION_STRENGTH); // Apply attraction
        updateParticlesAndForces(&particles, MAX_PARTICLES, dt);

        drawParticles(&particles, MAX_PARTICLES);  // Draw particles

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    freeParticles(&particles);  // Free the allocated memory
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}