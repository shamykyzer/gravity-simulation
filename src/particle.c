#include "particle.h"  // Include the header file defining the Particle structure and function declarations.
#include <stdlib.h>    // Include the standard library for memory allocation and random number generation.
#include <time.h>      // Include the time library for seeding the random number generator.
#include <math.h>      // Include the math library for mathematical functions like sqrt().

void initParticles(Particle* particles, int numParticles) {
    srand(time(NULL)); // Seed the random number generator with the current time for randomization.

    // Initialize each particle with random position, zero velocity, and random mass.
    for (int i = 0; i < numParticles; ++i) {
        // Random position between -1.0 and 1.0 on both axes.
        particles[i].x = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        particles[i].y = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

        // Initial velocity set to zero.
        particles[i].vx = 0.0f;
        particles[i].vy = 0.0f;

        // Random mass between 0.01 and 0.02.
        particles[i].mass = ((float)rand() / RAND_MAX) * 0.01f + 0.01f;

        // Set the initial color of the particles to blue.
        particles[i].r = 0.0f;
        particles[i].g = 0.0f;
        particles[i].b = 1.0f;
    }
}

void updateParticles(Particle* particles, int numParticles, float speedFactor, int mousePressed, double mouseX, double mouseY) {
    const float G = 0.0001f;  // Define a small gravitational constant for scaling the forces.

    // Iterate over each particle to compute its new velocity and position.
    for (int i = 0; i < numParticles; ++i) {
        float fx = 0.0f, fy = 0.0f;  // Initialize net force components to zero.

        // Compute gravitational force from every other particle.
        for (int j = 0; j < numParticles; ++j) {
            if (i != j) { // Avoid self-interaction.
                float dx = particles[j].x - particles[i].x; // Compute distance components.
                float dy = particles[j].y - particles[i].y;
                float distSquared = dx * dx + dy * dy + 0.01f; // Compute distance squared, add small value to prevent division by zero.
                
                // Compute the gravitational force magnitude.
                float force = G * particles[i].mass * particles[j].mass / distSquared;
                
                // Compute net force components by normalizing with distance.
                fx += force * dx / sqrt(distSquared);
                fy += force * dy / sqrt(distSquared);
            }
        }

        // Update the velocity of the particle using the net force and mass.
        particles[i].vx += fx / particles[i].mass;
        particles[i].vy += fy / particles[i].mass;

        // Update the position of the particle based on its velocity.
        particles[i].x += particles[i].vx * speedFactor;
        particles[i].y += particles[i].vy * speedFactor;

        // Wrap around logic: If a particle moves beyond the boundaries, wrap it to the other side.
        if (particles[i].x > 1.0f) particles[i].x = -1.0f;
        if (particles[i].x < -1.0f) particles[i].x = 1.0f;
        if (particles[i].y > 1.0f) particles[i].y = -1.0f;
        if (particles[i].y < -1.0f) particles[i].y = 1.0f;
    }
}
