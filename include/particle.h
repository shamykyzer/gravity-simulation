#ifndef PARTICLE_H
#define PARTICLE_H

#include "particle_struct.h"  // Include the Particle struct definition


void initParticles(Particle* particles, int numParticles);
void computeForces(Particle* particles, int numParticles, float G, float repulsionStrength);
void applyAttraction(Particle* particles, int numParticles, float centerX, float centerY, float attractionStrength, float repulsionStrength);
void handleBoundaryCollisions(Particle* particles, int numParticles, float minX, float maxX, float minY, float maxY);
void updateParticles(Particle* particles, int numParticles, float dt);
void drawParticles(Particle* particles, int numParticles);
void cleanupSimulation();

#endif
