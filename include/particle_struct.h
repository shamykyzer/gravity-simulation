#ifndef PARTICLE_STRUCT_H
#define PARTICLE_STRUCT_H

typedef struct {
    float x, y;       // Current positions
    float vx, vy;     // Velocities
    float ax, ay;     // Accelerations
    float mass;       // Mass
} Particle;

#endif
