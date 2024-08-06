#include "particle.h"
#include "quadtree.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h> // For debug print statements

// Function to initialize particles with random positions and velocities
void initParticles(Particle* particles, int numParticles) {
    srand(time(NULL)); // Seed the random number generator

    for (int i = 0; i < numParticles; ++i) {
        // Assign random positions within the range [-1.0, 1.0]
        particles[i].x = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
        particles[i].y = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

        // Initialize velocities to zero
        particles[i].vx = 0.0f;
        particles[i].vy = 0.0f;

        // Assign a small random mass, ensuring it's not zero
        particles[i].mass = ((float)rand() / RAND_MAX) * 0.01f + 0.01f;

        // Set initial color (e.g., blue)
        particles[i].r = 0.0f;
        particles[i].g = 0.0f;
        particles[i].b = 1.0f;
    }
}

// Function to update particles using the quadtree
void updateParticles(Particle* particles, int numParticles) {
    // Create the root node of the quadtree covering the entire simulation area
    QuadNode* root = createNode(-1.0f, -1.0f, 1.0f, 1.0f);

    // Insert particles into the quadtree
    for (int i = 0; i < numParticles; ++i) {
        printf("Inserting particle %d at (%f, %f)\n", i, particles[i].x, particles[i].y);
        insertParticle(root, &particles[i]);
    }

    // Compute forces for each particle using the quadtree
    for (int i = 0; i < numParticles; ++i) {
        printf("Computing force for particle %d\n", i);
        computeForce(root, &particles[i], 0.5f, 0.0001f);
    }

    // Update particle positions based on their velocities
    for (int i = 0; i < numParticles; ++i) {
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;

        // Wrap particles around the screen edges
        if (particles[i].x > 1.0f) particles[i].x = -1.0f;
        if (particles[i].x < -1.0f) particles[i].x = 1.0f;
        if (particles[i].y > 1.0f) particles[i].y = -1.0f;
        if (particles[i].y < -1.0f) particles[i].y = 1.0f;
    }

    // Clean up the quadtree memory
    freeQuadtree(root);
}
