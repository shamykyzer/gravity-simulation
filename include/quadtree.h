#ifndef QUADTREE_H
#define QUADTREE_H

#include "particle.h" // Include particle definition
#include <stdio.h>    // Include standard input-output
#include <math.h>     // Include math library

typedef struct QuadNode {
    float minX, minY, maxX, maxY; // Boundaries of the node
    float mass;                   // Total mass of particles in this node
    float centerX, centerY;       // Center of mass for this node
    Particle* particle;           // Particle stored in this node (if leaf)
    struct QuadNode* nw;          // Northwest child
    struct QuadNode* ne;          // Northeast child
    struct QuadNode* sw;          // Southwest child
    struct QuadNode* se;          // Southeast child
} QuadNode;

// Function prototypes
QuadNode* createNode(float minX, float minY, float maxX, float maxY);
void insertParticle(QuadNode* node, Particle* p);
void computeForce(QuadNode* node, Particle* p, float theta, float G);
void freeQuadtree(QuadNode* node);

#endif // QUADTREE_H
