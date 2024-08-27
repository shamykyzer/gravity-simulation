#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "particle.h"
#include "shader_utils.h"
#include "controls.h"
#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Particles particles;  // Use the SoA structure

void updateParticlesAndForces(Particles* particles, int numParticles, float dt) {
    computeForces(particles, numParticles, GRAVITY_CONST);
    updateParticles(particles, numParticles, dt);
    handleBoundaryCollisions(particles, numParticles, -1.0f, 1.0f, -1.0f, 1.0f);
}

int main() {
    srand(time(NULL));

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1200, 900, "Particle Simulation", NULL, NULL);
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

    const char* vertexSource = "#version 330 core\n"
                               "layout(location = 0) in vec2 aPos;\n"
                               "layout(location = 1) in vec3 aColor;\n"
                               "out vec3 color;\n"
                               "void main()\n"
                               "{\n"
                               "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
                               "    color = aColor;\n"
                               "}";

    const char* fragmentSource = "#version 330 core\n"
                                 "in vec3 color;\n"
                                 "out vec4 FragColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "    FragColor = vec4(color, 1.0);\n"
                                 "}";

    GLuint shaderProgram = createShaderProgram(vertexSource, fragmentSource);
    glUseProgram(shaderProgram);

    initParticles(&particles, MAX_PARTICLES);

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        float dt = 0.016f;  // Simulate a fixed timestep (approx. 60 FPS)

        handleInput(window, &particles, MAX_PARTICLES);  // Handle input, including reset and attraction
        updateParticlesAndForces(&particles, MAX_PARTICLES, dt);

        // Prepare data to send to GPU
        float* particleData = (float*)malloc(MAX_PARTICLES * 5 * sizeof(float));  // 2 for position, 3 for color
        for (int i = 0; i < MAX_PARTICLES; ++i) {
            particleData[i * 5 + 0] = particles.x[i];
            particleData[i * 5 + 1] = particles.y[i];
            particleData[i * 5 + 2] = particles.r[i];
            particleData[i * 5 + 3] = particles.g[i];
            particleData[i * 5 + 4] = particles.b[i];
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 5 * sizeof(float), particleData, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

        free(particleData);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    freeParticles(&particles);  // Free the allocated memory

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
