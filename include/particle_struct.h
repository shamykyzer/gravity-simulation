#ifndef PARTICLE_STRUCT_H
#define PARTICLE_STRUCT_H

typedef struct {
    float x, y;        // Position
    float vx, vy;      // Velocity
    float mass;        // Mass
    float r, g, b;     // Color components
    float ax, ay;      // Acceleration (optional, if needed)
} Particle;

#endif
