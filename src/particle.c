#include "particle.h"
#include <stdlib.h>
#include <math.h>

void initParticles(Particle* particles, int numParticles) {
    for (int i = 0; i < numParticles; ++i) {
        particles[i].x = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].y = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].vx = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].vy = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].ax = 0.0f;
        particles[i].ay = 0.0f;
        particles[i].mass = 1.0f;  // Assume equal mass for simplicity
        particles[i].r = (float)rand() / RAND_MAX;
        particles[i].g = (float)rand() / RAND_MAX;
        particles[i].b = (float)rand() / RAND_MAX;
    }
}

void computeForces(Particle* particles, int numParticles, float G) {
    for (int i = 0; i < numParticles; ++i) {
        float fx = 0.0f;
        float fy = 0.0f;

        for (int j = 0; j < numParticles; ++j) {
            if (i != j) {
                float dx = particles[j].x - particles[i].x;
                float dy = particles[j].y - particles[i].y;
                float distSq = dx * dx + dy * dy + 1e-5f; // Softening factor to avoid singularity
                float invDist = 1.0f / sqrtf(distSq);
                float invDistCube = invDist * invDist * invDist;
                float force = G * particles[i].mass * particles[j].mass * invDistCube;

                fx += force * dx;
                fy += force * dy;
            }
        }

        particles[i].ax = fx / particles[i].mass;
        particles[i].ay = fy / particles[i].mass;
    }
}

void updateParticles(Particle* particles, int numParticles, float dt) {
    for (int i = 0; i < numParticles; ++i) {
        particles[i].vx += particles[i].ax * dt;
        particles[i].vy += particles[i].ay * dt;
        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;

        // Boundary collision detection and response
        if (particles[i].x <= -1.0f || particles[i].x >= 1.0f) {
            particles[i].vx *= -1.0f; // Reverse velocity on x-axis
            if (particles[i].x <= -1.0f) particles[i].x = -1.0f;
            if (particles[i].x >= 1.0f) particles[i].x = 1.0f;
        }

        if (particles[i].y <= -1.0f || particles[i].y >= 1.0f) {
            particles[i].vy *= -1.0f; // Reverse velocity on y-axis
            if (particles[i].y <= -1.0f) particles[i].y = -1.0f;
            if (particles[i].y >= 1.0f) particles[i].y = 1.0f;
        }
    }
}
