#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
    float x, y;       // Position
    float vx, vy;     // Velocity
    float ax, ay;     // Acceleration (added these to handle the forces)
    float mass;       // Mass of the particle
    float r, g, b;    // Color of the particle
} Particle;

void initParticles(Particle* particles, int numParticles);
void computeForces(Particle* particles, int numParticles, float G);
void updateParticles(Particle* particles, int numParticles, float dt);

#endif
