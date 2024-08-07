#include "particle.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

void initParticles(Particle* particles, int numParticles) {
    srand(time(NULL));
    for (int i = 0; i < numParticles; ++i) {
        particles[i].x = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        particles[i].y = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        particles[i].vx = 0.0f;
        particles[i].vy = 0.0f;
        particles[i].mass = ((float)rand() / RAND_MAX) * 5.0f + 1.0f; // Random mass between 1 and 6
    }
}

void updateParticles(Particle* particles, int numParticles, float dt) {
    for (int i = 0; i < numParticles; ++i) {
        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;
    }
}
