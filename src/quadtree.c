#include "quadtree.h"
#include <stdlib.h>
#include <math.h>

QuadNode* createNode(float minX, float minY, float maxX, float maxY) {
    QuadNode* node = (QuadNode*)malloc(sizeof(QuadNode));
    node->mass = 0;
    node->centerX = (minX + maxX) / 2;
    node->centerY = (minY + maxY) / 2;
    node->minX = minX;
    node->minY = minY;
    node->maxX = maxX;
    node->maxY = maxY;
    node->particle = NULL;
    node->nw = NULL;
    node->ne = NULL;
    node->sw = NULL;
    node->se = NULL;
    return node;
}

void subdivideNode(QuadNode* node) {
    float midX = (node->minX + node->maxX) / 2;
    float midY = (node->minY + node->maxY) / 2;

    node->nw = createNode(node->minX, midY, midX, node->maxY);
    node->ne = createNode(midX, midY, node->maxX, node->maxY);
    node->sw = createNode(node->minX, node->minY, midX, midY);
    node->se = createNode(midX, node->minY, node->maxX, midY);
}

QuadNode* getQuadrant(QuadNode* node, Particle* p) {
    float midX = (node->minX + node->maxX) / 2;
    float midY = (node->minY + node->maxY) / 2;

    if (p->x < midX) {
        if (p->y < midY) {
            return node->sw;
        } else {
            return node->nw;
        }
    } else {
        if (p->y < midY) {
            return node->se;
        } else {
            return node->ne;
        }
    }
}

void insertParticle(QuadNode* node, Particle* p) {
    if (node->particle == NULL && node->nw == NULL) {
        node->particle = p;
        node->mass = p->mass;
        node->centerX = p->x;
        node->centerY = p->y;
    } else {
        if (node->nw == NULL) {
            subdivideNode(node);
        }
        QuadNode* quadrant = getQuadrant(node, p);
        insertParticle(quadrant, p);

        // Update mass and center of mass
        node->mass += p->mass;
        node->centerX = (node->centerX * (node->mass - p->mass) + p->x * p->mass) / node->mass;
        node->centerY = (node->centerY * (node->mass - p->mass) + p->y * p->mass) / node->mass;
    }
}

void computeForce(QuadNode* node, Particle* p, float theta, float G) {
    if (node == NULL || node->particle == p) return;

    float dx = node->centerX - p->x;
    float dy = node->centerY - p->y;
    float dist = sqrt(dx * dx + dy * dy + 0.01f); // Add small value to prevent division by zero

    if ((node->maxX - node->minX) / dist < theta) {
        float force = G * node->mass * p->mass / (dist * dist + 0.01f); // Add small value to prevent division by zero
        force = fminf(force, 1e6f); // Cap the force to prevent it from becoming too large

        p->vx += force * dx / dist;
        p->vy += force * dy / dist;
    } else {
        computeForce(node->nw, p, theta, G);
        computeForce(node->ne, p, theta, G);
        computeForce(node->sw, p, theta, G);
        computeForce(node->se, p, theta, G);
    }
}

void freeQuadtree(QuadNode* node) {
    if (node == NULL) return;
    freeQuadtree(node->nw);
    freeQuadtree(node->ne);
    freeQuadtree(node->sw);
    freeQuadtree(node->se);
    free(node);
}
