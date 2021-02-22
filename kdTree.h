/*
Contains all functions and structures for a kdTree
*/
#ifndef KDTREE_H
#define KDTREE_H
#include "list.h"

struct bst {
    struct listNode *list;
    double x;
    double y;
    struct bst *left;
    struct bst *right;
};

struct bst *createTree();

struct bst *buildTreeKD(char *inputFileName);

struct bst *nearestNeighbourSearch(struct bst **tree, struct bst *bestNode, double bestDistance, double x, double y, int ticker, int *comparisons);

void radiusSearch(struct bst **tree, double radius, double x, double y, int ticker, int *comparisons, int *numberOfMatches, FILE *ouputFile);

struct bst *nearestOf(struct bst *node1, struct bst *node2, double x, double y);

double distanceBetween(struct bst *currentNode, double x, double y);

void insertNodeKD(struct bst **tree, struct entry *data, double x, double y, int ticker);

void freeTree(struct bst *tree);

void printTree(struct bst *tree);
#endif
