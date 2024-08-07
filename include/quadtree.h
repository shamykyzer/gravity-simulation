#ifndef QUADTREE_H
#define QUADTREE_H

#include "particle.h"

typedef struct QuadNode {
    float mass;
    float centerX, centerY;
    float minX, minY, maxX, maxY;
    Particle* particle;
    struct QuadNode* nw;
    struct QuadNode* ne;
    struct QuadNode* sw;
    struct QuadNode* se;
} QuadNode;

QuadNode* createNode(float minX, float minY, float maxX, float maxY);
void insertParticle(QuadNode* node, Particle* p);
void computeForce(QuadNode* node, Particle* p, float theta, float G);
void freeQuadtree(QuadNode* node);
void subdivideNode(QuadNode* node);
QuadNode* getQuadrant(QuadNode* node, Particle* p);

#endif // QUADTREE_H
