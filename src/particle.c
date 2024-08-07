#include "particle.h"
#include <stdlib.h>
#include <math.h>
#define MAX_VELOCITY 10.0f

void initParticles(Particle* particles, int numParticles) {
    for (int i = 0; i < numParticles; i++) {
        particles[i].x = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].y = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].vx = 0.0f;
        particles[i].vy = 0.0f;
        particles[i].mass = (float)rand() / RAND_MAX + 0.1f;
        particles[i].r = (float)rand() / RAND_MAX;
        particles[i].g = (float)rand() / RAND_MAX;
        particles[i].b = (float)rand() / RAND_MAX;
    }
}

void updateParticles(Particle* particles, int numParticles, float dt) {
    for (int i = 0; i < numParticles; i++) {
        particles[i].vx = fminf(fmaxf(particles[i].vx, -MAX_VELOCITY), MAX_VELOCITY);
        particles[i].vy = fminf(fmaxf(particles[i].vy, -MAX_VELOCITY), MAX_VELOCITY);

        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;

        if (particles[i].x < -1.0f || particles[i].x > 1.0f) {
            particles[i].vx *= -0.5f;
        }

        if (particles[i].y < -1.0f || particles[i].y > 1.0f) {
            particles[i].vy *= -0.5f;
        }
    }
}
