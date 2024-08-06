#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
    float x, y;     // Position
    float vx, vy;   // Velocity
    float r, g, b;  // Color components
    float mass;     // Mass of the particle
} Particle;

// Function to initialize particles
void initParticles(Particle* particles, int numParticles);

// Function to update particle positions and velocities using the quadtree
void updateParticles(Particle* particles, int numParticles);

#endif
