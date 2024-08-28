#include "particle.h"
#include "quadtree.h"
#include "constants.h"
#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Initialize particles with slight perturbations
void initParticles(Particles* particles, int numParticles) {
    particles->x = (float*)malloc(numParticles * sizeof(float));
    particles->y = (float*)malloc(numParticles * sizeof(float));
    particles->prev_x = (float*)malloc(numParticles * sizeof(float));
    particles->prev_y = (float*)malloc(numParticles * sizeof(float));
    particles->vx = (float*)malloc(numParticles * sizeof(float));
    particles->vy = (float*)malloc(numParticles * sizeof(float));
    particles->ax = (float*)malloc(numParticles * sizeof(float));
    particles->ay = (float*)malloc(numParticles * sizeof(float));
    particles->mass = (float*)malloc(numParticles * sizeof(float));
    particles->r = (float*)malloc(numParticles * sizeof(float));
    particles->g = (float*)malloc(numParticles * sizeof(float));
    particles->b = (float*)malloc(numParticles * sizeof(float));
    particles->momentum_x = (float*)malloc(numParticles * sizeof(float));
    particles->momentum_y = (float*)malloc(numParticles * sizeof(float));

    for (int i = 0; i < numParticles; ++i) {
        particles->x[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        particles->y[i] = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        particles->prev_x[i] = particles->x[i];
        particles->prev_y[i] = particles->y[i];
        particles->vx[i] = 0.0f;
        particles->vy[i] = 0.0f;
        particles->ax[i] = 0.0f;
        particles->ay[i] = 0.0f;
        particles->mass[i] = 1.0f;
        particles->r[i] = 1.0f;  // Set red component to 1.0 (white)
        particles->g[i] = 1.0f;  // Set green component to 1.0 (white)
        particles->b[i] = 1.0f;  // Set blue component to 1.0 (white)
        particles->momentum_x[i] = 0.0f;
        particles->momentum_y[i] = 0.0f;
    }
}

// Compute forces for central orbit
void computeForces(Particles* particles, int numParticles, float G) {
    float centerX = 0.0f;
    float centerY = 0.0f;

    for (int i = 0; i < numParticles; ++i) {
        float dx = centerX - particles->x[i];
        float dy = centerY - particles->y[i];
        float distSq = dx * dx + dy * dy + 1e-4f;
        float dist = sqrtf(distSq);
        float force = G / distSq;

        particles->ax[i] = force * dx / dist;
        particles->ay[i] = force * dy / dist;
    }
}

void updateParticles(Particles* particles, int numParticles, float dt) {
    for (int i = 0; i < numParticles; ++i) {
        particles->vx[i] += particles->ax[i] * dt;
        particles->vy[i] += particles->ay[i] * dt;

        particles->x[i] += particles->vx[i] * dt;
        particles->y[i] += particles->vy[i] * dt;
    }
}

void handleBoundaryCollisions(Particles* particles, int numParticles, float minX, float maxX, float minY, float maxY) {
    for (int i = 0; i < numParticles; ++i) {
        if (particles->x[i] < minX || particles->x[i] > maxX) {
            particles->vx[i] *= -0.9f;
            particles->x[i] = particles->x[i] < minX ? minX : maxX;
        }

        if (particles->y[i] < minY || particles->y[i] > maxY) {
            particles->vy[i] *= -0.9f;
            particles->y[i] = particles->y[i] < minY ? minY : maxY;
        }
    }
}

void drawParticles(Particles* particles, int numParticles) {
    glBegin(GL_POINTS);
    for (int i = 0; i < numParticles; i++) {
        glColor3f(particles->r[i], particles->g[i], particles->b[i]);
        glVertex2f(particles->x[i], particles->y[i]);
    }
    glEnd();
}

void freeParticles(Particles* particles) {
    free(particles->x);
    free(particles->y);
    free(particles->prev_x);
    free(particles->prev_y);
    free(particles->vx);
    free(particles->vy);
    free(particles->ax);
    free(particles->ay);
    free(particles->mass);
    free(particles->r);
    free(particles->g);
    free(particles->b);
    free(particles->momentum_x);
    free(particles->momentum_y);
}
