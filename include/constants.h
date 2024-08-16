#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_PARTICLES 10000
#define THRESHOLD_DISTANCE 0.2f
#define GRAVITY_CONST 0.0001f
#define ATTRACTION_STRENGTH 0.05f
#define PARTICLE_SIZE 1000.0f
#define REPULSION_STRENGTH 0.02f  // Adjust this value as needed

void applyOrbit(Particles* particles, int numParticles, float centerX, float centerY, float strength);

#endif
