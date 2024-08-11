#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "particle.h"
#include "shader_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX_PARTICLES 5000
#define GRAVITY_CONST 0.00001f
#define ATTRACTION_STRENGTH 0.05f
#define PARTICLE_SIZE 100.0f

Particle particles[MAX_PARTICLES];

float centerX = 0.0f, centerY = 0.0f;
int attract = 0;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        attract = 1;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        centerX = (float)xpos / 400.0f - 1.0f;
        centerY = 1.0f - (float)ypos / 300.0f;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        attract = 0;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (attract) {
        centerX = (float)xpos / 400.0f - 1.0f;
        centerY = 1.0f - (float)ypos / 300.0f;
    }
}

void updateParticlesAndApplyAttraction(float dt) {
    computeForces(particles, MAX_PARTICLES, GRAVITY_CONST);
    if (attract) {
        applyAttraction(particles, MAX_PARTICLES, centerX, centerY, ATTRACTION_STRENGTH * 3.0f);
    }
    updateParticles(particles, MAX_PARTICLES, dt);
    handleBoundaryCollisions(particles, MAX_PARTICLES, -1.0f, 1.0f, -1.0f, 1.0f);
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

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

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

        updateParticlesAndApplyAttraction(dt);
        drawParticles(particles, MAX_PARTICLES);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
