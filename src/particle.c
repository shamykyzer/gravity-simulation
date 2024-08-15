#include "integral_image.h"
#include "particle.h"
#include "quadtree.h"
#include <GL/glew.h>
#include <math.h>
#include <stdlib.h>

#define GRAVITY_CONST 0.0001f // Increase the gravitational pull
#define REPULSION_CONST 0.00001f // Decrease repulsion slightly
#define MAX_FORCE 0.05f
#define BOUNCE_DAMPING 0.8f
#define DAMPING 0.99f
#define ATTRACTION_STRENGTH 0.0005f // Stronger attraction to the cursor

float** integralImage;
int integralImageWidth = 800;
int integralImageHeight = 600;
float softening = 0.01f;

void initializeSimulation() {
    integralImage = allocateIntegralImage(integralImageWidth, integralImageHeight);
    initializeIntegralImage(integralImage, integralImageWidth, integralImageHeight);
}

void updateParticlesAndForces(Particle* particles, int numParticles, float dt) {
    computeForces(particles, numParticles, GRAVITY_CONST, REPULSION_CONST);
    updateParticles(particles, numParticles, dt);
    handleBoundaryCollisions(particles, numParticles, -1.0f, 1.0f, -1.0f, 1.0f);
}

void initParticles(Particle* particles, int numParticles) {
    for (int i = 0; i < numParticles; ++i) {
        particles[i].x = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].y = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].vx = ((float)rand() / RAND_MAX - 0.5f) * 0.02f;
        particles[i].vy = ((float)rand() / RAND_MAX - 0.5f) * 0.02f;
        particles[i].mass = 1.0f;
        particles[i].r = (float)rand() / RAND_MAX;
        particles[i].g = (float)rand() / RAND_MAX;
        particles[i].b = (float)rand() / RAND_MAX;
        particles[i].ax = 0.0f;
        particles[i].ay = 0.0f;
    }
}

void computeForces(Particle* particles, int numParticles, float G, float repulsionStrength) {
    for (int i = 0; i < numParticles; ++i) {
        float fx = 0.0f;
        float fy = 0.0f;

        for (int j = 0; j < numParticles; ++j) {
            if (i == j) continue;

            float dx = particles[j].x - particles[i].x;
            float dy = particles[j].y - particles[i].y;
            float distSq = dx * dx + dy * dy + softening * softening;
            float dist = sqrtf(distSq);

            float attractionForce = (G * particles[i].mass * particles[j].mass) / distSq;
            float repulsionForce = (repulsionStrength * particles[i].mass * particles[j].mass) / distSq;
            float netForce = attractionForce - repulsionForce;
            netForce = fminf(netForce, MAX_FORCE);

            fx += netForce * dx / dist;
            fy += netForce * dy / dist;
        }

        particles[i].ax = fx / particles[i].mass;
        particles[i].ay = fy / particles[i].mass;
    }
}

void applyAttraction(Particle* particles, int numParticles, float centerX, float centerY, float attractionStrength, float repulsionStrength) {
    for (int i = 0; i < numParticles; ++i) {
        float dx = centerX - particles[i].x;
        float dy = centerY - particles[i].y;
        float distSq = dx * dx + dy * dy + 1e-4f;  // Add small value to prevent division by zero
        float dist = sqrtf(distSq);

        // Compute the attraction force
        float attractionForce = attractionStrength / distSq;

        // Apply the attraction force
        particles[i].vx += attractionForce * dx / dist;
        particles[i].vy += attractionForce * dy / dist;

        // Compute and apply the repulsion force between particles
        for (int j = 0; j < numParticles; ++j) {
            if (i != j) {
                float dx_p = particles[j].x - particles[i].x;
                float dy_p = particles[j].y - particles[i].y;
                float distSq_p = dx_p * dx_p + dy_p * dy_p + 1e-4f; // Add small value to prevent division by zero
                float dist_p = sqrtf(distSq_p);

                // Compute the repulsion force
                float repulsionForce = repulsionStrength / (distSq_p * dist_p);

                // Apply the repulsion force
                particles[i].vx -= repulsionForce * dx_p / dist_p;
                particles[i].vy -= repulsionForce * dy_p / dist_p;
            }
        }
    }
}

void updateParticles(Particle* particles, int numParticles, float dt) {
    for (int i = 0; i < numParticles; ++i) {
        particles[i].vx += particles[i].ax * dt;
        particles[i].vy += particles[i].ay * dt;

        particles[i].vx *= DAMPING;
        particles[i].vy *= DAMPING;

        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;
    }
}

void handleBoundaryCollisions(Particle* particles, int numParticles, float minX, float maxX, float minY, float maxY) {
    for (int i = 0; i < numParticles; ++i) {
        if (particles[i].x < minX || particles[i].x > maxX) {
            particles[i].vx *= -BOUNCE_DAMPING;
            particles[i].x = particles[i].x < minX ? minX : maxX;
        }

        if (particles[i].y < minY || particles[i].y > maxY) {
            particles[i].vy *= -BOUNCE_DAMPING;
            particles[i].y = particles[i].y < minY ? minY : maxY;
        }
    }
}

void drawParticles(Particle* particles, int numParticles) {
    glBegin(GL_POINTS);
    for (int i = 0; i < numParticles; i++) {
        glColor3f(particles[i].r, particles[i].g, particles[i].b);
        glVertex2f(particles[i].x, particles[i].y);
    }
    glEnd();
}

void cleanupSimulation() {
    freeIntegralImage(integralImage, integralImageWidth);
}
