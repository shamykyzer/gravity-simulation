#ifndef QUADTREE_H
#define QUADTREE_H

#include "particle.h"

typedef struct Node {
    float centerX, centerY;
    float width;
    float mass;
    int hasParticle;
    int isLeaf;
    Particle* particle;
    struct Node* children[4];
} Node;

void insertParticle(Node* node, Particle* p);
void computeForce(Node* node, Particle* p, float theta, float G, float eps);

#endif
