#include "quadtree.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h> // For debug print statements

// Create a new quadtree node
QuadNode* createNode(float minX, float minY, float maxX, float maxY) {
    QuadNode* node = (QuadNode*)malloc(sizeof(QuadNode));
    if (!node) {
        fprintf(stderr, "Failed to allocate memory for quadtree node\n");
        exit(EXIT_FAILURE); // Consider handling this more gracefully
    }
    node->mass = 0.0f;
    node->centerX = 0.0f;
    node->centerY = 0.0f;
    node->minX = minX;
    node->minY = minY;
    node->maxX = maxX;
    node->maxY = maxY;
    node->particle = NULL;
    node->nw = node->ne = node->sw = node->se = NULL;
    return node;
}

// Insert a particle into the quadtree
void insertParticle(QuadNode* node, Particle* p) {
    if (!node || !p) return;

    // Check if the node is a leaf node
    if (!node->nw && !node->particle) {
        node->particle = p;
        node->mass = p->mass;
        node->centerX = p->x;
        node->centerY = p->y;
        return;
    }

    // Subdivide the node if necessary
    if (!node->nw) {
        float midX = (node->minX + node->maxX) / 2.0f;
        float midY = (node->minY + node->maxY) / 2.0f;
        node->nw = createNode(node->minX, node->minY, midX, midY);
        node->ne = createNode(midX, node->minY, node->maxX, midY);
        node->sw = createNode(node->minX, midY, midX, node->maxY);
        node->se = createNode(midX, midY, node->maxX, node->maxY);

        if (node->particle) {
            insertParticle(node, node->particle); // Redistribute existing particle
            node->particle = NULL;
        }
    }

    // Insert the particle into the appropriate quadrant
    if (p->x < (node->minX + node->maxX) / 2.0f) {
        if (p->y < (node->minY + node->maxY) / 2.0f) {
            insertParticle(node->nw, p);
        } else {
            insertParticle(node->sw, p);
        }
    } else {
        if (p->y < (node->minY + node->maxY) / 2.0f) {
            insertParticle(node->ne, p);
        } else {
            insertParticle(node->se, p);
        }
    }

    // Update this node's mass and center of mass
    node->mass += p->mass;
    node->centerX = (node->centerX * (node->mass - p->mass) + p->x * p->mass) / node->mass;
    node->centerY = (node->centerY * (node->mass - p->mass) + p->y * p->mass) / node->mass;
}

// Compute the force on a particle using the quadtree
void computeForce(QuadNode* node, Particle* p, float theta, float G) {
    if (!node || node->mass == 0.0f || !p) {
        return;
    }

    float dx = node->centerX - p->x;
    float dy = node->centerY - p->y;
    float distSquared = dx * dx + dy * dy;
    
    // Add a small value to avoid division by zero
    float eps = 1e-9f;
    float dist = sqrt(distSquared + eps);
    
    // Use this node's aggregate mass if sufficiently far away
    float s = node->maxX - node->minX; // Width of the quadrant
    if (s / dist < theta || node->particle == p) {
        if (dist > eps) { // Avoid applying force to itself
            float force = G * node->mass * p->mass / (dist * dist + eps);
            p->vx += force * dx / dist;
            p->vy += force * dy / dist;
        }
    } else {
        // Otherwise, recurse into children
        computeForce(node->nw, p, theta, G);
        computeForce(node->ne, p, theta, G);
        computeForce(node->sw, p, theta, G);
        computeForce(node->se, p, theta, G);
    }
}

// Free the quadtree
void freeQuadtree(QuadNode* node) {
    if (!node) return;
    freeQuadtree(node->nw);
    freeQuadtree(node->ne);
    freeQuadtree(node->sw);
    freeQuadtree(node->se);
    free(node); // Free the current node
}
