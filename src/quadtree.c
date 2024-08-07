#include "quadtree.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define EPSILON 1e-8  // Small constant to prevent singularities

// Function to subdivide a node into four children
void subdivideNode(Node* node) {
    printf("Subdividing node at (%f, %f)\n", node->centerX, node->centerY);
    float halfWidth = node->width / 2.0f;
    for (int i = 0; i < 4; ++i) {
        node->children[i] = (Node*)malloc(sizeof(Node));
        if (!node->children[i]) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        node->children[i]->width = halfWidth;
        node->children[i]->mass = 0.0f;
        node->children[i]->hasParticle = 0;
        node->children[i]->isLeaf = 1;
        for (int j = 0; j < 4; ++j) {
            node->children[i]->children[j] = NULL;
        }
    }

    // Set the centers for the four children
    node->children[0]->centerX = node->centerX - halfWidth / 2.0f;
    node->children[0]->centerY = node->centerY - halfWidth / 2.0f;
    
    node->children[1]->centerX = node->centerX + halfWidth / 2.0f;
    node->children[1]->centerY = node->children[0]->centerY;
    
    node->children[2]->centerX = node->children[0]->centerX;
    node->children[2]->centerY = node->centerY + halfWidth / 2.0f;
    
    node->children[3]->centerX = node->children[1]->centerX;
    node->children[3]->centerY = node->children[2]->centerY;
}

// Function to determine which quadrant a particle belongs to
int getQuadrant(Node* node, Particle* p) {
    if (p->x < node->centerX) {
        if (p->y < node->centerY) return 0;
        else return 2;
    } else {
        if (p->y < node->centerY) return 1;
        else return 3;
    }
}

void insertParticle(Node* node, Particle* p) {
    printf("Inserting particle at (%f, %f)\n", p->x, p->y);
    if (node->isLeaf) {
        if (!node->hasParticle) {
            node->particle = p;
            node->mass = p->mass;
            node->centerX = p->x;
            node->centerY = p->y;
            node->hasParticle = 1;
        } else {
            subdivideNode(node);
            int quadrant = getQuadrant(node, node->particle);
            insertParticle(node->children[quadrant], node->particle);

            quadrant = getQuadrant(node, p);
            insertParticle(node->children[quadrant], p);

            node->hasParticle = 0;
            node->particle = NULL;
            node->isLeaf = 0;
        }
    } else {
        int quadrant = getQuadrant(node, p);
        insertParticle(node->children[quadrant], p);
    }

    node->mass += p->mass;
    node->centerX = (node->centerX * (node->mass - p->mass) + p->x * p->mass) / (node->mass + EPSILON);
    node->centerY = (node->centerY * (node->mass - p->mass) + p->y * p->mass) / (node->mass + EPSILON);
}

void computeForce(Node* node, Particle* p, float theta, float G, float eps) {
    if (!node->mass || (node->isLeaf && node->particle == p)) return;

    float dx = node->centerX - p->x;
    float dy = node->centerY - p->y;
    float dist = sqrtf(dx * dx + dy * dy + EPSILON);

    if (node->isLeaf || (node->width / dist) < theta) {
        float force = G * node->mass * p->mass / (dist * dist + eps * eps + EPSILON);
        p->vx += force * dx / (dist + EPSILON);
        p->vy += force * dy / (dist + EPSILON);
    } else {
        for (int i = 0; i < 4; ++i) {
            if (node->children[i]) {
                computeForce(node->children[i], p, theta, G, eps);
            }
        }
    }
}
