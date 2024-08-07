#ifndef QUADTREE_H
#define QUADTREE_H

#include "particle.h" // Include particle definition

// Define the quadtree node structure
typedef struct QuadNode {
    float mass;         // Total mass of this node
    float centerX;      // Center of mass x-coordinate
    float centerY;      // Center of mass y-coordinate
    float minX, minY;   // Bounding box of this quadrant
    float maxX, maxY;
    Particle* particle; // Pointer to a particle, if this is a leaf node
    struct QuadNode* nw; // Northwest child
    struct QuadNode* ne; // Northeast child
    struct QuadNode* sw; // Southwest child
    struct QuadNode* se; // Southeast child
} QuadNode;

// Function to create a new quadtree node
QuadNode* createNode(float minX, float minY, float maxX, float maxY);

// Function to insert a particle into the quadtree
void insertParticle(QuadNode* node, Particle* p);

// Function to compute forces on a particle using the quadtree
void computeForce(QuadNode* node, Particle* p, float theta, float G);

// Function to free the quadtree
void freeQuadtree(QuadNode* node);

#endif // QUADTREE_H
