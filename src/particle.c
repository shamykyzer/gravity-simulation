#include "particle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void initParticles(Particle* particles, int numParticles) {
    for (int i = 0; i < numParticles; ++i) {
        particles[i].x = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].y = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].vx = 0.0f;
        particles[i].vy = 0.0f;
    }
}

void updateParticles(Particle* particles, int numParticles) {
    for (int i = 0; i < numParticles; ++i) {
        float fx = 0.0f;
        float fy = 0.0f;
        for (int j = 0; j < numParticles; ++j) {
            if (i != j) {
                float dx = particles[j].x - particles[i].x;
                float dy = particles[j].y - particles[i].y;
                float distance = sqrtf(dx * dx + dy * dy);
                if (distance > 0.01f) { // Avoid division by zero and very close distances
                    float force = 1.0f / (distance * distance);
                    fx += force * dx / distance;
                    fy += force * dy / distance;
                }
            }
        }
        particles[i].vx += fx;
        particles[i].vy += fy;
    }
    for (int i = 0; i < numParticles; ++i) {
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
    }
}
