#ifndef CONTROLS_H
#define CONTROLS_H

#include <GLFW/glfw3.h>
#include "particle.h"

// Declare centerX and centerY as extern
extern float centerX;
extern float centerY;

// Function to initialize controls
void initControls(GLFWwindow* window);

// Function to handle input, including resetting the simulation
void handleInput(GLFWwindow* window, Particles* particles, int numParticles);

// Function to handle mouse scroll input for adjusting gravitational pull
void handleScroll(GLFWwindow* window, double xoffset, double yoffset);

#endif
