#include "quadtree.h"
#include <stdio.h>

int main() {
    QuadNode* root = createNode(0.0f, 0.0f, 1.0f, 1.0f);
    if (!root) {
        fprintf(stderr, "Failed to create root node\n");
        return 1;
    }
    if (root->minX != 0.0f || root->minY != 0.0f || root->maxX != 1.0f || root->maxY != 1.0f) {
        fprintf(stderr, "Node bounds incorrect\n");
        return 1;
    }
    if (root->centerX != 0.5f || root->centerY != 0.5f) {
        fprintf(stderr, "Node center incorrect\n");
        return 1;
    }
    freeQuadtree(root);
    printf("test_quadtree passed\n");
    return 0;
}
