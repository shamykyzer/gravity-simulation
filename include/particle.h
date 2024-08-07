#ifndef PARTICLE_H
#define PARTICLE_H

// Define the particle structure
typedef struct {
    float x, y;     // Position
    float vx, vy;   // Velocity
    float r, g, b;  // Color components
    float mass;     // Mass of the particle
} Particle;

// Function to initialize particles
void initParticles(Particle* particles, int numParticles);

// Correct declaration for updating particles
void updateParticles(Particle* particles, int numParticles);

#endif // PARTICLE_H
