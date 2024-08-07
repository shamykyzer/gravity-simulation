#include "particle.h"
#include "quadtree.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PARTICLES 1000

Particle particles[MAX_PARTICLES];

// Function to compile shaders
GLuint compileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    } else {
        printf("Shader compiled successfully.\n");
    }

    return shader;
}

// Function to create shader program
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    } else {
        printf("Shader program linked successfully.\n");
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void checkGLError(const char* location) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "OpenGL error at %s: %d\n", location, err);
    }
}

int main() {
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

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }
    checkGLError("GLFW/GLEW Initialization");

    // Compile shaders
    const char* vertexSource = "#version 330 core\n"
                               "layout(location = 0) in vec2 aPos;\n"
                               "layout(location = 1) in vec2 aVel;\n"
                               "layout(location = 2) in float aMass;\n"
                               "layout(location = 3) in vec3 aColor;\n"
                               "out vec3 fragColor;\n"
                               "uniform float deltaTime;\n"
                               "uniform float G;\n"
                               "uniform int numParticles;\n"
                               "uniform vec2 positions[1000];\n"
                               "uniform float masses[1000];\n"
                               "void main() {\n"
                               "    vec2 force = vec2(0.0);\n"
                               "    for (int i = 0; i < numParticles; ++i) {\n"
                               "        vec2 dir = positions[i] - aPos;\n"
                               "        float distSquared = dot(dir, dir) + 0.01;\n"
                               "        float invDist = inversesqrt(distSquared);\n"
                               "        float invDistCube = invDist * invDist * invDist;\n"
                               "        force += G * masses[i] * aMass * invDistCube * dir;\n"
                               "    }\n"
                               "    vec2 newVel = aVel + deltaTime * force / aMass;\n"
                               "    vec2 newPos = aPos + deltaTime * newVel;\n"
                               "    gl_Position = vec4(newPos, 0.0, 1.0);\n"
                               "    fragColor = aColor;\n"
                               "}";
    const char* fragmentSource = "#version 330 core\n"
                                 "in vec3 fragColor;\n"
                                 "out vec4 color;\n"
                                 "void main() {\n"
                                 "    color = vec4(fragColor, 1.0);\n"
                                 "}";

    GLuint shaderProgram = createShaderProgram(vertexSource, fragmentSource);

    // Generate and bind VAO and VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);

    // Vertex positions
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
    glEnableVertexAttribArray(0);
    // Velocities
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Masses
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // Colors
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Main loop
    float deltaTime = 0.01f;
    float G = 1.0f;
    initParticles(particles, MAX_PARTICLES);

    while (!glfwWindowShouldClose(window)) {
        // Update particles
        updateParticles(particles, MAX_PARTICLES, deltaTime);

        // Render particles
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glUniform1f(glGetUniformLocation(shaderProgram, "deltaTime"), deltaTime);
        glUniform1f(glGetUniformLocation(shaderProgram, "G"), G);
        glUniform1i(glGetUniformLocation(shaderProgram, "numParticles"), MAX_PARTICLES);

        // Update positions and masses
        for (int i = 0; i < MAX_PARTICLES; ++i) {
            glUniform2f(glGetUniformLocation(shaderProgram, "positions[i]"), particles[i].x, particles[i].y);
            glUniform1f(glGetUniformLocation(shaderProgram, "masses[i]"), particles[i].mass);
        }

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(particles), particles);
        checkGLError("Buffer Setup");

        glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
