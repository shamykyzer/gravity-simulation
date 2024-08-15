#ifndef INTEGRAL_IMAGE_H
#define INTEGRAL_IMAGE_H

#include "particle_struct.h"  // Include the Particle struct definition

// Function to allocate memory for the integral image
float** allocateIntegralImage(int width, int height);

// Function to initialize the integral image (set all values to 0)
void initializeIntegralImage(float** integralImage, int width, int height);

// Function to update the integral image based on the current positions and masses of particles
void updateIntegralImage(float** integralImage, Particle* particles, int numParticles, int width, int height);

// Function to compute force acting on a particle using the integral image
void computeForceFromIntegralImage(Particle* particle, float** integralImage, int width, int height);

// Function to free the memory used by the integral image
void freeIntegralImage(float** integralImage, int width);

#endif
