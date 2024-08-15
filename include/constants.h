#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_PARTICLES 7000
#define GRAVITY_CONST 0.0001f
#define ATTRACTION_STRENGTH 0.05f
#define PARTICLE_SIZE 100.0f

void applyOrbit(Particles* particles, int numParticles, float centerX, float centerY, float strength);

#endif
