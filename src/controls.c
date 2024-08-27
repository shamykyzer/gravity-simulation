#include "controls.h"
#include "constants.h"
#include "particle.h"
#include <GLFW/glfw3.h>
#include <math.h>  // Add this line to include the math header

// Add this line at the top of the file
void applyCentripetalForce(Particles* particles, int numParticles, float centerX, float centerY, float strength);

static int attract = 0;
static int repel = 0;
static float centerX = 0.0f, centerY = 0.0f;
static float attractionStrength = ATTRACTION_STRENGTH;
static float repulsionStrength = 0.0f;
static float orbitStrength = 0.01f;  // Reduced strength of the centripetal force
static float simulationSpeed = 1.0f; // Simulation speed

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        attract = 1;
        attractionStrength = ATTRACTION_STRENGTH;  // Reset to initial strength
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Get the window size
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Convert screen coordinates to normalized device coordinates (NDC)
        float ndcX = (float)xpos / (float)width * 2.0f - 1.0f;
        float ndcY = 1.0f - (float)ypos / (float)height * 2.0f;

        // Map NDC to world coordinates
        centerX = ndcX;
        centerY = ndcY;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        attract = 0;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        repel = 1;
        repulsionStrength = REPULSION_STRENGTH;  // Reset to initial strength
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        repel = 0;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (attract || repel) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Convert screen coordinates to normalized device coordinates (NDC)
        float ndcX = (float)xpos / (float)width * 2.0f - 1.0f;
        float ndcY = 1.0f - (float)ypos / (float)height * 2.0f;

        // Map NDC to world coordinates
        centerX = ndcX;
        centerY = ndcY;
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_UP) {
            simulationSpeed += 0.1f;  // Increase simulation speed
        } else if (key == GLFW_KEY_DOWN) {
            simulationSpeed = fmaxf(0.1f, simulationSpeed - 0.1f);  // Decrease simulation speed, but not below 0.1
        } else if (key == GLFW_KEY_RIGHT) {
            orbitStrength += 0.01f;  // Increase force
        } else if (key == GLFW_KEY_LEFT) {
            orbitStrength = fmaxf(0.01f, orbitStrength - 0.01f);  // Decrease force, but not below 0.01
        }
    }
}

void handleInput(GLFWwindow* window, Particles* particles, int numParticles) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        resetSimulation(particles, numParticles);
    }

    if (attract) {
        applyCentripetalForce(particles, numParticles, centerX, centerY, orbitStrength);
        attractionStrength += 0.01f;  // Increase attraction strength the longer the button is held
    }

    if (repel) {
        applyGlobalRepulsion(particles, numParticles, repulsionStrength);
        repulsionStrength += 0.01f;  // Increase repulsion strength the longer the button is held
    }
}

void initControls(GLFWwindow* window) {
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);
}

float getSimulationSpeed() {
    return simulationSpeed;
}
