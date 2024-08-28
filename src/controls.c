#include "controls.h"
#include "constants.h"
#include "particle.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>

static int attract = 0;
static int repel = 0;
static float centerX = 0.0f, centerY = 0.0f;
static float attractionStrength = ATTRACTION_STRENGTH;
static float repulsionStrength = 0.0f;

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        attract = 1;
        attractionStrength = ATTRACTION_STRENGTH;  // Reset to initial strength
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        centerX = (float)xpos / 400.0f - 1.0f;
        centerY = 1.0f - (float)ypos / 300.0f;
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
        centerX = (float)xpos / 400.0f - 1.0f;
        centerY = 1.0f - (float)ypos / 300.0f;
    }
}

void handleInput(GLFWwindow* window, Particles* particles, int numParticles) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        resetSimulation(particles, numParticles);
    }

    if (attract) {
        applyAttraction(particles, numParticles, centerX, centerY, attractionStrength);
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
}

void resetSimulation(Particles* particles, int numParticles) {
    // Re-initialize particles
    initParticles(particles, numParticles);
}

void applyAttraction(Particles* particles, int numParticles, float mouseX, float mouseY, float strength) {
    for (int i = 0; i < numParticles; ++i) {
        float dx = mouseX - particles->x[i];
        float dy = mouseY - particles->y[i];
        float distSq = dx * dx + dy * dy + 1e-4f;
        float dist = sqrtf(distSq);
        float force = strength / distSq;

        particles->vx[i] += force * dx / dist;
        particles->vy[i] += force * dy / dist;
    }
}

void applyGlobalRepulsion(Particles* particles, int numParticles, float strength) {
    for (int i = 0; i < numParticles; ++i) {
        float angle = ((float)rand() / RAND_MAX) * 2 * M_PI;
        particles->vx[i] += strength * cosf(angle);
        particles->vy[i] += strength * sinf(angle);
    }
}
