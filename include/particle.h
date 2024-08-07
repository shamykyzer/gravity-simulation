#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
    float x, y;    // Position
    float vx, vy;  // Velocity
    float mass;    // Mass
    float r, g, b; // Color components
} Particle;

// Function declarations
void initParticles(Particle* particles, int numParticles);
void updateParticles(Particle* particles, int numParticles);

#endif // PARTICLE_H
