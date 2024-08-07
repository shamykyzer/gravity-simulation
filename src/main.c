#include "particle.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    const int numParticles = 100;
    const int numSteps = 1000; // Number of steps to simulate
    Particle* particles = (Particle*)malloc(sizeof(Particle) * numParticles);

    if (!particles) {
        fprintf(stderr, "Failed to allocate memory for particles.\n");
        return EXIT_FAILURE;
    }

    // Initialize particles
    printf("Initializing particles...\n");
    initParticles(particles, numParticles);

    // Simulation loop
    printf("Starting simulation...\n");
    for (int step = 0; step < numSteps; ++step) {
        printf("Simulation step %d\n", step);
        updateParticles(particles, numParticles);

        // Print particle states for debugging
        for (int i = 0; i < numParticles; ++i) {
            printf("Particle %d: Position (%.2f, %.2f), Velocity (%.2f, %.2f)\n", 
                   i, particles[i].x, particles[i].y, particles[i].vx, particles[i].vy);
        }
    }

    printf("Simulation completed.\n");
    free(particles);
    return 0;
}
