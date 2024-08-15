// In controls.h
#ifndef CONTROLS_H
#define CONTROLS_H

#define ATTRACTION_STRENGTH 0.05f  // Adjust the value as needed

// Other includes and function prototypes
#include <GLFW/glfw3.h>
#include "particle.h"

void initControls(GLFWwindow* window);
void handleInput(GLFWwindow* window, Particle* particles, int numParticles);

#endif
