#include <GL/glew.h>  // Move this to the top
#include "particle.h"
#include "quadtree.h"
#include "constants.h"
#include <GLFW/glfw3.h>
#include "controls.h"  // Include controls to access simulation speed
#include <math.h>
#include <stdlib.h>

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
    particles->prev_x = (float*)malloc(numParticles * sizeof(float));
    particles->prev_y = (float*)malloc(numParticles * sizeof(float));

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
        particles->prev_x[i] = particles->x[i];
        particles->prev_y[i] = particles->y[i];
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
    float speed = getSimulationSpeed();  // Get the current simulation speed
    dt *= speed;  // Adjust the time step by the simulation speed

    for (int i = 0; i < numParticles; ++i) {
        float disp_x = particles->x[i] - particles->prev_x[i];
        float disp_y = particles->y[i] - particles->prev_y[i];

        // Update previous positions
        particles->prev_x[i] = particles->x[i];
        particles->prev_y[i] = particles->y[i];

        // Verlet integration
        particles->x[i] += disp_x + particles->ax[i] * dt * dt;
        particles->y[i] += disp_y + particles->ay[i] * dt * dt;

        // Reset acceleration
        particles->ax[i] = 0.0f;
        particles->ay[i] = 0.0f;

        // Calculate velocity (for visualization or other purposes)
        particles->vx[i] = (particles->x[i] - particles->prev_x[i]) / dt;
        particles->vy[i] = (particles->y[i] - particles->prev_y[i]) / dt;

        // Update momentum
        particles->momentum_x[i] = particles->mass[i] * particles->vx[i];
        particles->momentum_y[i] = particles->mass[i] * particles->vy[i];
    }
}

// Handle boundary collisions with reduced momentum
void handleBoundaryCollisions(Particles* particles, int numParticles, float minX, float maxX, float minY, float maxY) {
    const float dampingFactor = 0.8f;

    for (int i = 0; i < numParticles; ++i) {
        if (particles->x[i] < minX) {
            particles->x[i] = minX;
            particles->prev_x[i] = particles->x[i] + (particles->x[i] - particles->prev_x[i]) * dampingFactor;
        } else if (particles->x[i] > maxX) {
            particles->x[i] = maxX;
            particles->prev_x[i] = particles->x[i] + (particles->x[i] - particles->prev_x[i]) * dampingFactor;
        }

        if (particles->y[i] < minY) {
            particles->y[i] = minY;
            particles->prev_y[i] = particles->y[i] + (particles->y[i] - particles->prev_y[i]) * dampingFactor;
        } else if (particles->y[i] > maxY) {
            particles->y[i] = maxY;
            particles->prev_y[i] = particles->y[i] + (particles->y[i] - particles->prev_y[i]) * dampingFactor;
        }
    }
}

// Apply attraction force towards a center point
void applyAttraction(Particles* particles, int numParticles, float centerX, float centerY, float strength) {
    for (int i = 0; i < numParticles; ++i) {
        float dx = centerX - particles->x[i];
        float dy = centerY - particles->y[i];
        float distSq = dx * dx + dy * dy + 1e-4f;  // Prevent division by zero
        float dist = sqrtf(distSq);

        // Compute the attraction force
        float force = strength / distSq;

        // Calculate the components of the force
        float forceX = force * dx / dist;
        float forceY = force * dy / dist;

        // Apply the force to the particle's acceleration
        particles->ax[i] += forceX;
        particles->ay[i] += forceY;

        // Update velocity to make particles follow the cursor
        particles->vx[i] += particles->ax[i];
        particles->vy[i] += particles->ay[i];
    }
}

// Apply centripetal force towards a center point for orbiting
void applyCentripetalForce(Particles* particles, int numParticles, float centerX, float centerY, float strength) {
    for (int i = 0; i < numParticles; ++i) {
        float dx = centerX - particles->x[i];
        float dy = centerY - particles->y[i];
        float distSq = dx * dx + dy * dy + 1e-4f;  // Prevent division by zero
        float dist = sqrtf(distSq);

        // Compute the centripetal force
        float force = strength / distSq;

        // Calculate the components of the force
        float forceX = force * dx / dist;
        float forceY = force * dy / dist;

        // Apply the force to the particle's acceleration
        particles->ax[i] += forceX;
        particles->ay[i] += forceY;

        // Update velocity to make particles orbit around the center
        float tangentX = -dy / dist;
        float tangentY = dx / dist;
        particles->vx[i] += tangentX * force;
        particles->vy[i] += tangentY * force;
    }
}

// Apply global repulsion between all particles
void applyGlobalRepulsion(Particles* particles, int numParticles, float repulsionStrength) {
    for (int i = 0; i < numParticles; ++i) {
        for (int j = i + 1; j < numParticles; ++j) {
            float dx = particles->x[j] - particles->x[i];
            float dy = particles->y[j] - particles->y[i];
            float distSq = dx * dx + dy * dy + 1e-4f;  // Prevent division by zero
            float dist = sqrtf(distSq);

            if (dist < 0.05f) {  // Adjust this threshold as needed
                float force = repulsionStrength / distSq;
                float forceX = force * dx / dist;
                float forceY = force * dy / dist;

                particles->vx[i] -= forceX;
                particles->vy[i] -= forceY;

                particles->vx[j] += forceX;
                particles->vy[j] += forceY;
            }
        }
    }
}

// Draw particles on the screen
void drawParticles(Particles* particles, int numParticles) {
    glBegin(GL_POINTS);
    for (int i = 0; i < numParticles; i++) {
        // Calculate the speed of the particle
        float speed = sqrtf(particles->vx[i] * particles->vx[i] + particles->vy[i] * particles->vy[i]);

        // Normalize the speed to a value between 0 and 1 for color mapping
        float normalizedSpeed = fminf(speed / 0.5f, 1.0f);  // Assuming 0.5f is the max speed

        // Set color based on speed: Cyan -> Yellow -> White
        float r = 0.0f, g = 1.0f, b = 1.0f;  // Default Cyan color

        if (normalizedSpeed > 0.8f) {  // High speed, make particle white
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
        } else if (normalizedSpeed > 0.4f) {  // Medium speed, make particle yellow
            r = 1.0f;
            g = 1.0f;
            b = 0.0f;
        }

        glColor3f(r, g, b);  // Set the particle color based on speed
        glVertex2f(particles->x[i], particles->y[i]);
    }
    glEnd();
}

void resetSimulation(Particles* particles, int numParticles) {
    initParticles(particles, numParticles);
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
    free(particles->prev_x);
    free(particles->prev_y);
}
