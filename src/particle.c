#include "particle_struct.h"  // Include the Particle struct definition
#include "particle.h"
#include "quadtree.h"
#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Initialize particles
void initParticles(Particles* particles, int numParticles) {
    particles->x = (float*)malloc(numParticles * sizeof(float));
    particles->y = (float*)malloc(numParticles * sizeof(float));
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
        particles->x[i] = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles->y[i] = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles->vx[i] = (float)rand() / RAND_MAX * 0.1f - 0.05f;
        particles->vy[i] = (float)rand() / RAND_MAX * 0.1f - 0.05f;
        particles->ax[i] = 0.0f;
        particles->ay[i] = 0.0f;
        particles->mass[i] = (float)rand() / RAND_MAX * 0.01f + 0.01f;
        particles->momentum_x[i] = particles->mass[i] * particles->vx[i];
        particles->momentum_y[i] = particles->mass[i] * particles->vy[i];
        particles->r[i] = (float)rand() / RAND_MAX;
        particles->g[i] = (float)rand() / RAND_MAX;
        particles->b[i] = (float)rand() / RAND_MAX;
    }
}

// Compute forces using quadtree
void computeForces(Particles* particles, int numParticles, float G) {
    float minX = -1.0f, minY = -1.0f, maxX = 1.0f, maxY = 1.0f;

    QuadNode* root = createNode(minX, minY, maxX, maxY);

    for (int i = 0; i < numParticles; ++i) {
        Particle p = {
            .x = particles->x[i],
            .y = particles->y[i],
            .vx = particles->vx[i],
            .vy = particles->vy[i],
            .ax = particles->ax[i],
            .ay = particles->ay[i],
            .mass = particles->mass[i],
            .r = particles->r[i],
            .g = particles->g[i],
            .b = particles->b[i],
            .momentum_x = particles->momentum_x[i],
            .momentum_y = particles->momentum_y[i]
        };
        insertParticle(root, &p);
    }

    float theta = 0.5f;  // Barnes-Hut approximation threshold
    for (int i = 0; i < numParticles; ++i) {
        Particle p = {
            .x = particles->x[i],
            .y = particles->y[i],
            .vx = particles->vx[i],
            .vy = particles->vy[i],
            .ax = particles->ax[i],
            .ay = particles->ay[i],
            .mass = particles->mass[i],
            .r = particles->r[i],
            .g = particles->g[i],
            .b = particles->b[i],
            .momentum_x = particles->momentum_x[i],
            .momentum_y = particles->momentum_y[i]
        };
        computeForce(root, &p, theta, G);

        particles->vx[i] = p.vx;
        particles->vy[i] = p.vy;
    }

    freeQuadtree(root);
}

// Update particle positions
void updateParticles(Particles* particles, int numParticles, float dt) {
    const float maxVelocity = 0.5f;  // Increased maximum velocity

    for (int i = 0; i < numParticles; ++i) {
        particles->vx[i] += particles->ax[i] * dt;
        particles->vy[i] += particles->ay[i] * dt;

        // Clamp the velocity
        float speed = sqrtf(particles->vx[i] * particles->vx[i] + particles->vy[i] * particles->vy[i]);
        if (speed > maxVelocity) {
            float scale = maxVelocity / speed;
            particles->vx[i] *= scale;
            particles->vy[i] *= scale;
        }

        particles->momentum_x[i] = particles->mass[i] * particles->vx[i];
        particles->momentum_y[i] = particles->mass[i] * particles->vy[i];

        particles->x[i] += particles->vx[i] * dt;
        particles->y[i] += particles->vy[i] * dt;
    }
}

// Handle boundary collisions
void handleBoundaryCollisions(Particles* particles, int numParticles, float minX, float maxX, float minY, float maxY) {
    for (int i = 0; i < numParticles; ++i) {
        if (particles->x[i] < minX) {
            particles->x[i] = minX;
            particles->vx[i] *= -1.0f;
        } else if (particles->x[i] > maxX) {
            particles->x[i] = maxX;
            particles->vx[i] *= -1.0f;
        }

        if (particles->y[i] < minY) {
            particles->y[i] = minY;
            particles->vy[i] *= -1.0f;
        } else if (particles->y[i] > maxY) {
            particles->y[i] = maxY;
            particles->vy[i] *= -1.0f;
        }
    }
}

// Apply attraction force
void applyAttraction(Particles* particles, int numParticles, float centerX, float centerY, float strength) {
    for (int i = 0; i < numParticles; ++i) {
        float dx = centerX - particles->x[i];
        float dy = centerY - particles->y[i];
        float distSq = dx * dx + dy * dy + 1e-4f;  // Avoid division by zero
        float dist = sqrtf(distSq);
        
        // Compute the attraction force
        float force = strength / distSq;
        
        // Apply the force to the particle's acceleration
        particles->ax[i] += force * dx / dist;
        particles->ay[i] += force * dy / dist;
    }
}

// Draw particles on the screen
void drawParticles(Particles* particles, int numParticles) {
    glBegin(GL_POINTS);
    for (int i = 0; i < numParticles; i++) {
        glColor3f(particles->r[i], particles->g[i], particles->b[i]);
        glVertex2f(particles->x[i], particles->y[i]);
    }
    glEnd();
}

// Free allocated memory for particles
void freeParticles(Particles* particles) {
    free(particles->x);
    free(particles->y);
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
