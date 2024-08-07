#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
    float x, y;
    float vx, vy;
    float mass;
} Particle;

void initParticles(Particle* particles, int numParticles);
void updateParticles(Particle* particles, int numParticles, float dt);

#endif
