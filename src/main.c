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
                               "        force += G * aMass * masses[i] * dir * invDist * invDist * invDist;\n"
                               "    }\n"
                               "    vec2 newVel = aVel + deltaTime * force / aMass;\n"
                               "    vec2 newPos = aPos + deltaTime * newVel;\n"
                               "    fragColor = aColor;\n"
                               "    gl_Position = vec4(newPos, 0.0, 1.0);\n"
                               "}\n";

    const char* fragmentSource = "#version 330 core\n"
                                 "in vec3 fragColor;\n"
                                 "out vec4 color;\n"
                                 "void main() {\n"
                                 "    color = vec4(fragColor, 1.0);\n"
                                 "}\n";

    GLuint shaderProgram = createShaderProgram(vertexSource, fragmentSource);
    glUseProgram(shaderProgram);

    // Initialize particles
    initParticles(particles, MAX_PARTICLES);

    // Print particle data for debugging
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        printf("Particle %d - Position: (%f, %f), Mass: %f\n", i, particles[i].x, particles[i].y, particles[i].mass);
    }

    // Vertex Array and Buffer setup
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * MAX_PARTICLES, particles, GL_DYNAMIC_DRAW);
    checkGLError("Buffer Data");

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
    glEnableVertexAttribArray(0);

    // Velocity attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Mass attribute
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Color attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(3);
    checkGLError("Vertex Attrib Setup");

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Update particles
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Update uniforms
        glUniform1f(glGetUniformLocation(shaderProgram, "deltaTime"), 0.01f);
        glUniform1f(glGetUniformLocation(shaderProgram, "G"), 0.0001f);
        glUniform1i(glGetUniformLocation(shaderProgram, "numParticles"), MAX_PARTICLES);

        // Copy particle positions and masses to uniforms
        for (int i = 0; i < MAX_PARTICLES; ++i) {
            char uniformName[32];
            snprintf(uniformName, sizeof(uniformName), "positions[%d]", i);
            glUniform2f(glGetUniformLocation(shaderProgram, uniformName), particles[i].x, particles[i].y);

            snprintf(uniformName, sizeof(uniformName), "masses[%d]", i);
            glUniform1f(glGetUniformLocation(shaderProgram, uniformName), particles[i].mass);
        }
        checkGLError("Uniform Setup");

        glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);
        checkGLError("Drawing");

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
