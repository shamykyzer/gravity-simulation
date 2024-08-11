#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
    float x, y;       // Position
    float vx, vy;     // Velocity
    float ax, ay;     // Acceleration
    float mass;       // Mass
    float r, g, b;    // Color
    float momentum_x, momentum_y; // Momentum
} Particle;

void initParticles(Particle* particles, int numParticles);
void computeForces(Particle* particles, int numParticles, float G);
void updateParticles(Particle* particles, int numParticles, float dt);
void handleBoundaryCollisions(Particle* particles, int numParticles, float minX, float maxX, float minY, float maxY);
void applyAttraction(Particle* particles, int numParticles, float centerX, float centerY, float strength);
void drawParticles(Particle* particles, int numParticles);

#endif
