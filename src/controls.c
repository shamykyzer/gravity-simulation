#include "controls.h"
#include <stdlib.h>

// constants.h
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define GRAVITY_CONST 0.00001f
#define ATTRACTION_STRENGTH 0.05f
#define PARTICLE_SIZE 100.0f
#define MAX_PARTICLES 5000

// Global variables for control state
static int attract = 0;
static float centerX = 0.0f, centerY = 0.0f;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
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

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (attract) {
        centerX = (float)xpos / 400.0f - 1.0f;
        centerY = 1.0f - (float)ypos / 300.0f;
    }
}

void resetSimulation(Particles* particles, int numParticles) {
    initParticles(particles, numParticles);
}

void handleInput(GLFWwindow* window, Particles* particles, int numParticles) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        resetSimulation(particles, numParticles);
    }
    
    // Apply attraction if mouse is pressed
    if (attract) {
        applyAttraction(particles, numParticles, centerX, centerY, ATTRACTION_STRENGTH * 3.0f);
    }
}

void initControls(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}

#endif