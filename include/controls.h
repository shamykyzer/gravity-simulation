#ifndef CONTROLS_H
#define CONTROLS_H

#include <GL/glew.h>  // Add this line
#include <GLFW/glfw3.h>
#include "particle.h"

void initControls(GLFWwindow* window);
void handleInput(GLFWwindow* window, Particles* particles, int numParticles);
float getSimulationSpeed();  // Add this line

#endif // CONTROLS_H
