#ifndef CONTROLS_H
#define CONTROLS_H

#include <GLFW/glfw3.h>
#include "particle.h"

// Function to initialize controls
void initControls(GLFWwindow* window);

// Function to handle input, including resetting the simulation
void handleInput(GLFWwindow* window, Particles* particles, int numParticles);

#endif
