#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "particle.h"
#include "shader_utils.h"
#include "controls.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_PARTICLES 2000
#define GRAVITY_CONST 0.00001f
#define THETA 0.5f  // Barnes-Hut approximation parameter

Particle particles[MAX_PARTICLES];

void updateParticlesAndApplyForces(float dt) {
    computeForces(particles, MAX_PARTICLES, GRAVITY_CONST, THETA);
    handleBoundaryCollisions(particles, MAX_PARTICLES, -1.0f, 1.0f, -1.0f, 1.0f);
    updateParticles(particles, MAX_PARTICLES, dt);
}

int main() {
    srand(time(NULL));

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Particle Simulation", NULL, NULL);
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

    initControls(window);

    const char* vertexSource = "#version 330 core\n"
                               "layout(location = 0) in vec2 aPos;\n"
                               "void main()\n"
                               "{\n"
                               "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
                               "}";

    const char* fragmentSource = "#version 330 core\n"
                                 "out vec4 FragColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "    FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
                                 "}";

    GLuint shaderProgram = createShaderProgram(vertexSource, fragmentSource);
    glUseProgram(shaderProgram);

    initParticles(particles, MAX_PARTICLES);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float dt = 0.016f;  // Simulate a fixed timestep (approx. 60 FPS)

        handleInput(window, particles, MAX_PARTICLES);
        updateParticlesAndApplyForces(dt);
        drawParticles(particles, MAX_PARTICLES);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
