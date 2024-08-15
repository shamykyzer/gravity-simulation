#ifndef QUADTREE_H
#define QUADTREE_H

#include "particle.h"  // Include the Particle struct definition

typedef struct QuadNode {
    float minX, minY, maxX, maxY;
    float centerX, centerY;
    float mass;
    struct QuadNode *nw, *ne, *sw, *se;
    Particle* particle;
} QuadNode;

QuadNode* createNode(float minX, float minY, float maxX, float maxY);
void subdivideNode(QuadNode* node);
QuadNode* getQuadrant(QuadNode* node, Particle* p);
void insertParticle(QuadNode* node, Particle* p);
void computeForce(QuadNode* node, Particle* p, float theta, float G);
void freeQuadtree(QuadNode* node);

#endif
