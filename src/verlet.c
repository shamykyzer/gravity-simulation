#include "verlet.h"

typedef struct {
    mfloat_t current[VEC3_SIZE];
    mfloat_t previous[VEC3_SIZE];
    mfloat_t acceleration[VEC3_SIZE];
    mfloat_t radius;
} VerletObject;

void initVerletObject(VerletObject* obj, mfloat_t x, mfloat_t y, mfloat_t z, mfloat_t radius) {
    obj->current[0] = x;
    obj->current[1] = y;
    obj->current[2] = z;
    
    obj->previous[0] = x;
    obj->previous[1] = y;
    obj->previous[2] = z;
    
    obj->acceleration[0] = 0.0f;
    obj->acceleration[1] = 0.0f;
    obj->acceleration[2] = 0.0f;
    
    obj->radius = radius;
}

void updateVerletObject(VerletObject* obj, mfloat_t dt) {
    mfloat_t temp[VEC3_SIZE];
    for (int i = 0; i < VEC3_SIZE; i++) {
        temp[i] = obj->current[i];
        obj->current[i] = 2 * obj->current[i] - obj->previous[i] + obj->acceleration[i] * dt * dt;
        obj->previous[i] = temp[i];
    }
}

void applyForce(VerletObject* obj, mfloat_t fx, mfloat_t fy, mfloat_t fz) {
    obj->acceleration[0] += fx;
    obj->acceleration[1] += fy;
    obj->acceleration[2] += fz;
}
