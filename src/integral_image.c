#include "integral_image.h"
#include <stdlib.h>
#include <string.h>

// Allocates memory for the integral image
float** allocateIntegralImage(int width, int height) {
    float** integralImage = (float**)malloc(height * sizeof(float*));
    for (int i = 0; i < height; ++i) {
        integralImage[i] = (float*)calloc(width, sizeof(float));
    }
    return integralImage;
}

// Initializes the integral image with zeroes
void initializeIntegralImage(float** integralImage, int width, int height) {
    for (int y = 0; y < height; ++y) {
        memset(integralImage[y], 0, width * sizeof(float));
    }
}

// Updates the integral image based on the current particle positions
void updateIntegralImage(float** integralImage, Particle* particles, int numParticles, int width, int height) {
    // Clear the integral image
    initializeIntegralImage(integralImage, width, height);

    // Update the integral image with particle masses
    for (int i = 0; i < numParticles; ++i) {
        int x = (int)((particles[i].x + 1.0f) * 0.5f * (width - 1));
        int y = (int)((particles[i].y + 1.0f) * 0.5f * (height - 1));

        if (x >= 0 && x < width && y >= 0 && y < height) {
            integralImage[y][x] += particles[i].mass;
        }
    }

    // Convert the mass field into an integral image
    for (int y = 1; y < height; ++y) {
        for (int x = 1; x < width; ++x) {
            integralImage[y][x] += integralImage[y - 1][x] + integralImage[y][x - 1] - integralImage[y - 1][x - 1];
        }
    }
}

// Frees the memory used by the integral image
void freeIntegralImage(float** integralImage, int height) {
    for (int i = 0; i < height; ++i) {
        free(integralImage[i]);
    }
    free(integralImage);
}
