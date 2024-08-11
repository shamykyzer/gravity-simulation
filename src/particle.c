#include "particle.h"
#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>  // Add this for OpenGL functions
#include <GLFW/glfw3.h>  // Add this for GLFW

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
    for (int i = 0; i < numParticles; ++i) {
        float fx = 0.0f;
        float fy = 0.0f;
        for (int j = 0; j < numParticles; ++j) {
            if (i == j) continue;
            float dx = particles[j].x - particles[i].x;
            float dy = particles[j].y - particles[i].y;
            float distSq = dx * dx + dy * dy + 1e-4f; // Avoid division by zero
            float dist = sqrtf(distSq);
            float force = (G * particles[i].mass * particles[j].mass) / distSq;
            fx += force * dx / dist;
            fy += force * dy / dist;
        }
        particles[i].ax = fx / particles[i].mass;
        particles[i].ay = fy / particles[i].mass;
    }
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
