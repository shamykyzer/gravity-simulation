#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
    float x, y;     // Position
    float vx, vy;   // Velocity
    float r, g, b;  // Color components
    float mass;     // Mass of the particle
} Particle;

// Function to initialize particles with random positions and velocities
void initParticles(Particle* particles, int numParticles);

// Function to update particle positions and velocities based on gravitational interactions
void updateParticles(Particle* particles, int numParticles, float speedFactor, int mousePressed, double mouseX, double mouseY);

#endif
