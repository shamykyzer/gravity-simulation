#include "controls.h"
#include "particle.h"
#include <GLFW/glfw3.h>

static int attract = 0;
static int followCursor = 0;
static float centerX = 0.0f, centerY = 0.0f;

// Callback for mouse button input
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

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        followCursor = 1;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        followCursor = 0;
    }
}

// Callback for cursor position input
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (attract || followCursor) {
        centerX = (float)xpos / 400.0f - 1.0f;
        centerY = 1.0f - (float)ypos / 300.0f;
    }
}

// Function to reset the simulation
void resetSimulation(Particle* particles, int numParticles) {
    initParticles(particles, numParticles);
}

// Function to handle key input and apply attraction if necessary
void handleInput(GLFWwindow* window, Particle* particles, int numParticles) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        resetSimulation(particles, numParticles);
    }

    // Apply attraction if the left mouse button is pressed
    if (attract) {
        applyAttraction(particles, numParticles, centerX, centerY, ATTRACTION_STRENGTH * 3.0f, 0.1f);  // Adjust repulsion strength as needed
    }

    // Make particles follow the cursor if the right mouse button is pressed
    if (followCursor) {
        applyAttraction(particles, numParticles, centerX, centerY, ATTRACTION_STRENGTH, 0.0f);  // Only attraction, no repulsion
    }
}

// Function to initialize controls
void initControls(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}
