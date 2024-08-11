#include "particle.h"
#include "quadtree.h"
#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void initParticles(Particle* particles, int numParticles) {
    for (int i = 0; i < numParticles; ++i) {
        particles[i].x = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].y = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        particles[i].vx = (float)rand() / RAND_MAX * 0.1f - 0.05f;
        particles[i].vy = (float)rand() / RAND_MAX * 0.1f - 0.05f;
        particles[i].mass = (float)rand() / RAND_MAX * 0.01f + 0.01f;
        particles[i].momentum_x = particles[i].mass * particles[i].vx;
        particles[i].momentum_y = particles[i].mass * particles[i].vy;
        particles[i].r = (float)rand() / RAND_MAX;
        particles[i].g = (float)rand() / RAND_MAX;
        particles[i].b = (float)rand() / RAND_MAX;
    }
}

void computeForces(Particle* particles, int numParticles, float G) {
    // Determine the bounds of the simulation
    float minX = -1.0f, minY = -1.0f, maxX = 1.0f, maxY = 1.0f;

    // Create the root node of the quadtree
    QuadNode* root = createNode(minX, minY, maxX, maxY);

    // Insert all particles into the quadtree
    for (int i = 0; i < numParticles; ++i) {
        insertParticle(root, &particles[i]);
    }

    // Compute forces using the quadtree
    float theta = 0.5f;  // Barnes-Hut approximation threshold
    for (int i = 0; i < numParticles; ++i) {
        computeForce(root, &particles[i], theta, G);
    }

    // Free the quadtree memory
    freeQuadtree(root);
}

void updateParticles(Particle* particles, int numParticles, float dt) {
    const float maxVelocity = 0.5f;  // Increased maximum velocity

    for (int i = 0; i < numParticles; ++i) {
        particles[i].vx += particles[i].ax * dt;
        particles[i].vy += particles[i].ay * dt;

        // Clamp the velocity
        float speed = sqrtf(particles[i].vx * particles[i].vx + particles[i].vy * particles[i].vy);
        if (speed > maxVelocity) {
            float scale = maxVelocity / speed;
            particles[i].vx *= scale;
            particles[i].vy *= scale;
        }

        particles[i].momentum_x = particles[i].mass * particles[i].vx;
        particles[i].momentum_y = particles[i].mass * particles[i].vy;

        particles[i].x += particles[i].vx * dt;
        particles[i].y += particles[i].vy * dt;
    }
}

void handleBoundaryCollisions(Particle* particles, int numParticles, float minX, float maxX, float minY, float maxY) {
    for (int i = 0; i < numParticles; ++i) {
        if (particles[i].x < minX) {
            particles[i].x = minX;
            particles[i].vx *= -1.0f;
        } else if (particles[i].x > maxX) {
            particles[i].x = maxX;
            particles[i].vx *= -1.0f;
        }

        if (particles[i].y < minY) {
            particles[i].y = minY;
            particles[i].vy *= -1.0f;
        } else if (particles[i].y > maxY) {
            particles[i].y = maxY;
            particles[i].vy *= -1.0f;
        }
    }
}

void applyAttraction(Particle* particles, int numParticles, float centerX, float centerY, float strength) {
    for (int i = 0; i < numParticles; ++i) {
        float dx = centerX - particles[i].x;
        float dy = centerY - particles[i].y;
        float distSq = dx * dx + dy * dy + 1e-4f;  // Avoid division by zero
        float dist = sqrtf(distSq);
        
        // Compute the attraction force
        float force = strength / distSq;
        
        // Apply the force to the particle's acceleration
        particles[i].ax += force * dx / dist;
        particles[i].ay += force * dy / dist;
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
