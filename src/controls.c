#include "controls.h"
#include "constants.h"
#include "particle.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>  // Include for debugging

// Define centerX and centerY here
float centerX = 0.0f, centerY = 0.0f;
float attractionStrength = ATTRACTION_STRENGTH;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    centerX = (float)xpos / (width / 2.0f) - 1.0f;
    centerY = 1.0f - (float)ypos / (height / 2.0f);
    printf("Cursor Position: (%f, %f)\n", centerX, centerY);  // Debugging information
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            // Update center of gravity to cursor position
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            centerX = (float)xpos / (width / 2.0f) - 1.0f;
            centerY = 1.0f - (float)ypos / (height / 2.0f);
            printf("Mouse Click Position: (%f, %f)\n", centerX, centerY);  // Debugging information
        }
    }
}

void handleScroll(GLFWwindow* window, double xoffset, double yoffset) {
    (void)window; // unused
    (void)xoffset;
    attractionStrength += (float)yoffset * 0.01f;
    if (attractionStrength < 0.0f) {
        attractionStrength = 0.0f;
    }
    printf("Attraction Strength: %f\n", attractionStrength);
}

void handleInput(GLFWwindow* window, Particles* particles, int numParticles) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        resetSimulation(particles, numParticles);  // Ensure this function is defined
    }
}

void initControls(GLFWwindow* window) {
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback); // Set mouse button callback
    glfwSetScrollCallback(window, handleScroll);
}

// Ensure the function signature matches the declaration
void applyAttraction(Particles* particles, int numParticles, float centerX, float centerY, float strength) {
    for (int i = 0; i < numParticles; ++i) {
        float dx = centerX - particles->x[i];
        float dy = centerY - particles->y[i];
        float distSq = dx * dx + dy * dy + 1e-4f; // Avoid division by zero
        float dist = sqrtf(distSq);
        float force = strength / distSq; // Adjust strength as needed

        particles->ax[i] += force * dx / dist;
        particles->ay[i] += force * dy / dist;
    }
}

void applyGlobalRepulsion(Particles* particles, int numParticles, float strength) {
    for (int i = 0; i < numParticles; ++i) {
        float angle = ((float)rand() / RAND_MAX) * 2 * M_PI;
        particles->vx[i] += strength * cosf(angle);
        particles->vy[i] += strength * sinf(angle);
    }
}
