#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
    float *x, *y;       // Positions
    float *vx, *vy;     // Velocities
    float *ax, *ay;     // Accelerations
    float *mass;        // Masses
    float *r, *g, *b;   // Colors
    float *momentum_x, *momentum_y; // Momenta
} Particles;

// Function prototypes for operations on the SoA structure
void initParticles(Particles* particles, int numParticles);
void computeForces(Particles* particles, int numParticles, float G);
void updateParticles(Particles* particles, int numParticles, float dt);
void handleBoundaryCollisions(Particles* particles, int numParticles, float minX, float maxX, float minY, float maxY);
void applyAttraction(Particles* particles, int numParticles, float centerX, float centerY, float strength);
void drawParticles(Particles* particles, int numParticles);
void freeParticles(Particles* particles);

#endif
