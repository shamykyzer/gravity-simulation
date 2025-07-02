#include "quadtree.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    QuadNode *root = createNode(0.0f, 0.0f, 1.0f, 1.0f);

    Particle p1 = { .x = 0.25f, .y = 0.25f, .vx = 0, .vy = 0, .ax = 0, .ay = 0, .mass = 2.0f };
    Particle p2 = { .x = 0.75f, .y = 0.75f, .vx = 0, .vy = 0, .ax = 0, .ay = 0, .mass = 3.0f };
    Particle p3 = { .x = 0.50f, .y = 0.50f, .vx = 0, .vy = 0, .ax = 0, .ay = 0, .mass = 5.0f };

    insertParticle(root, &p1);
    assert(fabs(root->mass - p1.mass) < 1e-6);
    assert(fabs(root->centerX - p1.x) < 1e-6);
    assert(fabs(root->centerY - p1.y) < 1e-6);

    insertParticle(root, &p2);
    float totalMass = p1.mass + p2.mass;
    float expectedX = (p1.x * p1.mass + p2.x * p2.mass) / totalMass;
    float expectedY = (p1.y * p1.mass + p2.y * p2.mass) / totalMass;
    assert(fabs(root->mass - totalMass) < 1e-6);
    assert(fabs(root->centerX - expectedX) < 1e-6);
    assert(fabs(root->centerY - expectedY) < 1e-6);

    insertParticle(root, &p3);
    totalMass += p3.mass;
    expectedX = (p1.x * p1.mass + p2.x * p2.mass + p3.x * p3.mass) / totalMass;
    expectedY = (p1.y * p1.mass + p2.y * p2.mass + p3.y * p3.mass) / totalMass;
    assert(fabs(root->mass - totalMass) < 1e-6);
    assert(fabs(root->centerX - expectedX) < 1e-6);
    assert(fabs(root->centerY - expectedY) < 1e-6);

    freeQuadtree(root);
    printf("quadtree tests passed\n");
    return 0;
}

