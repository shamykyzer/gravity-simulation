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
        particles[i].mass = ((float)rand() / RAND_MAX) * 0.01f + 0.01f; // Random mass
        particles[i].r = 0.0f;
        particles[i].g = 0.0f;
        particles[i].b = 1.0f;
    }
}

void updateParticles(Particle* particles, int numParticles, float speedFactor, int mousePressed, double mouseX, double mouseY) {
    const float G = 0.0001f;  // Gravitational constant

    for (int i = 0; i < numParticles; ++i) {
        float fx = 0.0f, fy = 0.0f;

        for (int j = 0; j < numParticles; ++j) {
            if (i != j) {
                float dx = particles[j].x - particles[i].x;
                float dy = particles[j].y - particles[i].y;
                float distSquared = dx * dx + dy * dy + 0.01f; // Prevent division by zero
                float force = G * particles[i].mass * particles[j].mass / distSquared;
                fx += force * dx / sqrt(distSquared); // Normalize by distance
                fy += force * dy / sqrt(distSquared);
            }
        }

        // Update velocity based on computed forces
        particles[i].vx += fx / particles[i].mass;
        particles[i].vy += fy / particles[i].mass;

        // Update position
        particles[i].x += particles[i].vx * speedFactor;
        particles[i].y += particles[i].vy * speedFactor;

        // Wrap around the screen edges
        if (particles[i].x > 1.0f) particles[i].x = -1.0f;
        if (particles[i].x < -1.0f) particles[i].x = 1.0f;
        if (particles[i].y > 1.0f) particles[i].y = -1.0f;
        if (particles[i].y < -1.0f) particles[i].y = 1.0f;
    }
}
