#ifndef PARTICLE_STRUCT_H
#define PARTICLE_STRUCT_H

typedef struct {
    float x, y;
    float vx, vy;
    float ax, ay;
    float mass;
    float r, g, b;
    float momentum_x, momentum_y;
} Particle;

#endif
