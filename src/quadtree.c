#include "quadtree.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Function to create a new quadtree node
QuadNode* createNode(float minX, float minY, float maxX, float maxY) {
    QuadNode* node = (QuadNode*)malloc(sizeof(QuadNode));
    if (!node) {
        fprintf(stderr, "Failed to allocate memory for QuadNode.\n");
        exit(EXIT_FAILURE);
    }
    node->minX = minX;
    node->minY = minY;
    node->maxX = maxX;
    node->maxY = maxY;
    node->particle = NULL;
    node->mass = 0.0f;
    node->centerX = 0.0f;
    node->centerY = 0.0f;
    node->nw = node->ne = node->sw = node->se = NULL;
    return node;
}

// Insert a particle into the quadtree
void insertParticle(QuadNode* node, Particle* p) {
    if (!node || !p) return;

    // Check if node is a leaf and can hold a particle
    if (node->particle == NULL && node->nw == NULL) {
        node->particle = p;
        node->mass = p->mass;
        node->centerX = p->x;
        node->centerY = p->y;
        return;
    }

    // Subdivide if this is a leaf node with an existing particle
    if (node->nw == NULL) {
        // Subdivide
        float midX = (node->minX + node->maxX) / 2.0f;
        float midY = (node->minY + node->maxY) / 2.0f;
        node->nw = createNode(node->minX, node->minY, midX, midY);
        node->ne = createNode(midX, node->minY, node->maxX, midY);
        node->sw = createNode(node->minX, midY, midX, node->maxY);
        node->se = createNode(midX, midY, node->maxX, node->maxY);

        // Move the current particle into the appropriate quadrant
        if (node->particle) {
            Particle* oldParticle = node->particle;
            node->particle = NULL;
            insertParticle(node, oldParticle);
        }
    }

    // Insert particle into the appropriate quadrant
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

    // Update mass and center of mass for internal nodes
    node->mass += p->mass;
    node->centerX = (node->centerX * (node->mass - p->mass) + p->x * p->mass) / node->mass;
    node->centerY = (node->centerY * (node->mass - p->mass) + p->y * p->mass) / node->mass;
}

// Compute the force on a particle from the quadtree
void computeForce(QuadNode* node, Particle* p, float theta, float G) {
    if (!node || node->mass == 0.0f || !p) {
        return;
    }

    float dx = node->centerX - p->x;
    float dy = node->centerY - p->y;
    float distSquared = dx * dx + dy * dy;

    // Softening factor to prevent singularities
    float eps = 1e-5f; // Adjust as necessary for your simulation scale
    float dist = sqrt(distSquared + eps * eps);

    float s = node->maxX - node->minX;
    if (s / dist < theta || node->particle == p) {
        if (dist > eps) {
            float force = G * node->mass * p->mass / (dist * dist + eps * eps);
            p->vx += force * dx / dist;
            p->vy += force * dy / dist;
            printf("Force applied on particle at (%f, %f): fx = %f, fy = %f\n", p->x, p->y, force * dx / dist, force * dy / dist);
        }
    } else {
        if (node->nw) computeForce(node->nw, p, theta, G);
        if (node->ne) computeForce(node->ne, p, theta, G);
        if (node->sw) computeForce(node->sw, p, theta, G);
        if (node->se) computeForce(node->se, p, theta, G);
    }
}

// Function to free the quadtree memory
void freeQuadtree(QuadNode* node) {
    if (!node) return;
    freeQuadtree(node->nw);
    freeQuadtree(node->ne);
    freeQuadtree(node->sw);
    freeQuadtree(node->se);
    free(node);
}
