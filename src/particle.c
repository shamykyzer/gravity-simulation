#include "particle_struct.h"
#include "particle.h"
#include "quadtree.h"
#include "constants.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>  // Include this for printf
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
        particles->vx[i] = (float)rand() / RAND_MAX * 0.005f - 0.0025f;  // Much slower initial velocity
        particles->vy[i] = (float)rand() / RAND_MAX * 0.005f - 0.0025f;  // Much slower initial velocity
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

// Update particle positions
void updateParticles(Particles* particles, int numParticles, float dt) {
    const float maxVelocity = 0.2f;  // Lower maximum velocity
    const float dampingFactor = 0.98f;  // Higher damping factor for more reduction in speed

    for (int i = 0; i < numParticles; ++i) {
        particles->vx[i] += particles->ax[i] * dt;
        particles->vy[i] += particles->ay[i] * dt;

        // Apply increased damping to reduce velocity more significantly
        particles->vx[i] *= dampingFactor;
        particles->vy[i] *= dampingFactor;

        // Clamp the velocity to a lower max velocity
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

void applyAttraction(Particles* particles, int numParticles, float centerX, float centerY, float strength) {
    for (int i = 0; i < numParticles; ++i) {
        float dx = centerX - particles->x[i];
        float dy = centerY - particles->y[i];
        float distSq = dx * dx + dy * dy + 1e-4f;  // Avoid division by zero
        float dist = sqrtf(distSq);

        // Compute the gravitational force
        float force = strength / distSq;

        // Calculate the components of the force
        float forceX = (force * dx / dist);
        float forceY = (force * dy / dist);

        // Apply the force to the particle's acceleration
        particles->ax[i] += forceX;
        particles->ay[i] += forceY;

        // Ensure the velocity is updated correctly to create a stable orbit
        particles->vx[i] += particles->ax[i];
        particles->vy[i] += particles->ay[i];
    }
}

// Draw particles on the screen
void drawParticles(Particles* particles, int numParticles) {
    glBegin(GL_POINTS);
    for (int i = 0; i < numParticles; i++) {
        float speed = sqrtf(particles->vx[i] * particles->vx[i] + particles->vy[i] * particles->vy[i]);

        float r = 0.0f, g = 1.0f, b = 1.0f;  // Cyan color by default

        if (speed > 0.2f && speed < 0.4f) {  // Medium speed, make it yellow
            r = 1.0f;
            g = 1.0f;
            b = 0.0f;
        } else if (speed >= 0.4f) {  // High speed, make it white
            r = 1.0f;
            g = 1.0f;
            b = 1.0f;
        }

        glColor3f(r, g, b);  // Set the particle color based on speed
        glVertex2f(particles->x[i], particles->y[i]);
    }
    glEnd();
}

void applyOrbit(Particles* particles, int numParticles, float centerX, float centerY, float strength) {
    for (int i = 0; i < numParticles; ++i) {
        float dx = centerX - particles->x[i];
        float dy = centerY - particles->y[i];
        float distSq = dx * dx + dy * dy + 1e-4f;
        float dist = sqrtf(distSq);

        float force = strength / distSq;

        particles->ax[i] += force * dx / dist;
        particles->ay[i] += force * dy / dist;

        particles->vx[i] += particles->ax[i];
        particles->vy[i] += particles->ay[i];
    }
}

// //void applyBorderRepulsion(Particles* particles, int numParticles, float minX, float maxX, float minY, float maxY, float repulsionStrength, float thresholdDistance) {
//     for (int i = 0; i < numParticles; ++i) {
//         // Check left and right borders
//         if (particles->x[i] < minX + thresholdDistance) {
//             float distance = minX + thresholdDistance - particles->x[i];
//             particles->x[i] += repulsionStrength * distance;
//             printf("Particle %d at left border, repelled by %f\n", i, repulsionStrength * distance);
//         }
//         else if (particles->x[i] > maxX - thresholdDistance) {
//             float distance = particles->x[i] - (maxX - thresholdDistance);
//             particles->x[i] -= repulsionStrength * distance;
//             printf("Particle %d at right border, repelled by %f\n", i, repulsionStrength * distance);
//         }

//         // Check top and bottom borders
//         if (particles->y[i] < minY + thresholdDistance) {
//             float distance = minY + thresholdDistance - particles->y[i];
//             particles->y[i] += repulsionStrength * distance;
//             printf("Particle %d at bottom border, repelled by %f\n", i, repulsionStrength * distance);
//         }
//         else if (particles->y[i] > maxY - thresholdDistance) {
//             float distance = particles->y[i] - (maxY - thresholdDistance);
//             particles->y[i] -= repulsionStrength * distance;
//             printf("Particle %d at top border, repelled by %f\n", i, repulsionStrength * distance);
//         }
//     }
// }

// void applyBounceBack(Particles *particles, int numParticles, float centerX, float centerY, float bounceStrength) {
//     // Constants
//     float thresholdDistance = 0.1f;  // Adjust this value as needed

//     for (int i = 0; i < numParticles; ++i) {
//         // Compute distance from the particle to the center point
//         float dx = particles->x[i] - centerX;
//         float dy = particles->y[i] - centerY;
//         float distance = sqrtf(dx * dx + dy * dy);

//         // Apply bounce back only if within threshold distance
//         if (distance < thresholdDistance && distance > 0.0f) {
//             // Normalize the direction vector
//             float forceX = dx / distance;
//             float forceY = dy / distance;

//             // Compute the bounce-back force
//             float forceMagnitude = bounceStrength * (1.0f - distance / thresholdDistance);
            
//             // Update particle positions based on bounce-back force
//             // Move the particle away from the center more aggressively
//             particles->x[i] += forceX * forceMagnitude;
//             particles->y[i] += forceY * forceMagnitude;
//         }
//     }
// }

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
