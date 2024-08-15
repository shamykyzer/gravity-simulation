#include "controls.h"
#include "constants.h"
#include "particle.h"
#include <GLFW/glfw3.h>

static int attract = 0;
static int repel = 0;
static float centerX = 0.0f, centerY = 0.0f;
static float orbitStrength = ATTRACTION_STRENGTH;
static float repulsionStrength = ATTRACTION_STRENGTH; // Base repulsion strength

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
        orbitStrength = ATTRACTION_STRENGTH; // Reset strength
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        repel = 1;
        repulsionStrength = ATTRACTION_STRENGTH; // Start with base repulsion strength
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        repel = 0;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (attract || repel) {
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

    if (attract) {
        orbitStrength += 0.001f; // Increase orbit strength the longer left-click is held
        applyOrbit(particles, numParticles, centerX, centerY, orbitStrength);
    }

    if (repel) {
        repulsionStrength += 0.001f; // Increase repulsion strength the longer right-click is held
        //applyBounceBack(particles, numParticles, centerX, centerY, repulsionStrength);
    }
}

void initControls(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
}
