/*
Contains all functions for a kdTree
TODO: replace entry pointers with void pointers, and make this more generalised.
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "kdTree.h"
#include "entry.h"
#include "list.h"


struct bst *createTree() {
    return NULL;
}


/*
Opens input file, uses it to build kd tree, then closes file and returns pointer to the tree.
*/
struct bst *buildTreeKD(char *inputFileName) {

    FILE *inputData = fopen(inputFileName, "r");
    assert(inputData);

    struct bst *outputTree = createTree();

    // While loop reads every line of csv into its own struct. Then puts struct in tree.
    char *currentLine = NULL;
    size_t buffSize = 0;
    int lineNumber = 0;
    while(getline(&currentLine, &buffSize, inputData) != -1) {

        // Skip header line and go to next loop.
        if (lineNumber == 0) {
            fflush(stdout);
            lineNumber++;
            continue;
        }
        struct entry *tempEntry = createRecord(currentLine);
        insertNodeKD(&outputTree, tempEntry, atof(tempEntry->xCoordinate), atof(tempEntry->yCoordinate), 0);
        fflush(stdout);
    }
    free(currentLine);
    fclose(inputData);
    return outputTree;
}


/*
Inserts struct entry into tree at correct coordinates.
Each node stores its own coordinates rather than pointing to the ones held in the struct.
If ticker is 0: use x as key
If ticker is 1: use y as key
On first insertion: value of 0 should be used for ticker, to conform to assignment spec
*/
void insertNodeKD(struct bst **tree, struct entry *data, double x, double y, int ticker) {

    // When you get to null child, insert node.
    if (*tree == NULL) {
        // create node here
        *tree = malloc(sizeof(struct bst));
        assert(*tree);
        (*tree)->left = NULL;
        (*tree)->right = NULL;
        (*tree)->x = x;
        (*tree)->y = y;

        (*tree)->list = createList();
        insertList(&(*tree)->list, data); //not sure if this is inserting a pointer, or the entire structure...

        return;
    }
    
    if (ticker == 0) {
        // compare x
        if (x < (*tree)->x) {
            // Insert on left of current node
            insertNodeKD(&(*tree)->left, data, x, y, 1);
        } else if (x > (*tree)->x) {
            // Insert on right of current node
            insertNodeKD(&(*tree)->right, data, x, y, 1);
        } else {
            // x values are equal
            if (y != (*tree)->y) {
                // y values not equal, so insert node on the right
                insertNodeKD(&(*tree)->right, data, x, y, 1);
            } else {
                // y values are equal

                // duplicate set of coordinates: add this entry to the preixisting list.
                insertList(&(*tree)->list, data); //not sure if this is inserting a pointer, or the entire structure...
                
                return;
            }
        }
    } 
    else if (ticker == 1) {
        // compare y
        if (y < (*tree)->y) {
            // Insert on left of current node
            insertNodeKD(&(*tree)->left, data, x, y, 0);
        } else if (y > (*tree)->y) {
            // Insert on right of current node
            insertNodeKD(&(*tree)->right, data, x, y, 0);
        } else {
            // y values are equal
            if (x != (*tree)->x) {
                // x values not equal, so insert node on the right (coordinates not the same)
                insertNodeKD(&(*tree)->right, data, x, y, 0);
            } else {
                // x values are equal

                // duplicate set of coordinates: add this entry to the preixisting list.
                insertList(&(*tree)->list, data); //not sure if this is inserting a pointer, or the entire structure...

                return;
            }
        }
    }
}


/*
Finds cartesian distance between a node, and a set of coordinates.
*/
double distanceBetween(struct bst *currentNode, double x, double y) {
    double xDistance = currentNode->x - x;
    double yDistance = currentNode->y - y;
    double squaredDistance = pow(xDistance, 2) + pow(yDistance, 2);
    return sqrt(squaredDistance);
}


/*
Returns the node which is closer to the given coordinates
Won't notice exact matches, doesn't matter.
*/
struct bst *nearestOf(struct bst *node1, struct bst *node2, double x, double y) {

    double distance1 = distanceBetween(node1, x, y);
    double distance2 = distanceBetween(node2, x, y);
    if (distance1 < distance2) {
        return node1;
    } else {
        return node2;
    }
}


/*
Returns node closest to given coordinates.
Distance should initially be passed in as maximum double size.
Ticker is used to alternate key comparisons between x and y.
Goes straight down normal insertion path, then recurses back up stack whilst 
checking if alternate branches should be checked too.
Comparisons should be reset to 0, outside of function, after every use.
*/
struct bst *nearestNeighbourSearch(struct bst **tree, struct bst *bestNode, double bestDistance, double x, double y, int ticker, int *comparisons) {

    if (*tree == NULL) {
        // Will return at the end of our first "naive" search.
        // Will then return the best match found from each recursive path down a branch
        return bestNode;
    }
    (*comparisons)++;

    double newDistance = distanceBetween(*tree, x, y);
    double sneakyDistance; // This will hold the bestDistance returned at the bottom of recursive calls to branches, for use after returning up the stack

    if (newDistance < bestDistance) {
        bestDistance = newDistance;
        bestNode = *tree;
    }

    if (ticker == 0) {
        // check x value of key 
        if (x < (*tree)->x) {
            // Go left
            // If this call can't find a better node, it will just return the same node back.
            bestNode = nearestNeighbourSearch(&(*tree)->left, bestNode, bestDistance, x, y, 1, comparisons);
            // above call won't update local "bestDistance" in local call, we need to do that below:
            sneakyDistance = distanceBetween(bestNode, x, y);
            if (sneakyDistance < bestDistance) {
                bestDistance = sneakyDistance;
            }
            // When the above recursive call returns, it should come down to this logic :)
            // We've checked the "good" partitions, now we should the other side
            if ((*tree)->right != NULL) {
                if (fabs((*tree)->x - x) <= bestDistance) {
                // We need to check the other branch in this case.
                bestNode = nearestOf(nearestNeighbourSearch(&(*tree)->right, bestNode, bestDistance, x, y, 1, comparisons), bestNode, x, y);
                }
            }
        } else {
            // Go right
            // If this call can't find a better node, it will just return the same node back.
            bestNode = nearestNeighbourSearch(&(*tree)->right, bestNode, bestDistance, x, y, 1, comparisons);
            // above call won't update local "bestDistance" in local call, we need to do that below:
            sneakyDistance = distanceBetween(bestNode, x, y);
            if (sneakyDistance < bestDistance) {
                bestDistance = sneakyDistance;
            }
            // When the above recursive call returns, it should come down to this logic :)
            // We've checked the "good" partitions, now we should the other side
            if ((*tree)->left != NULL) {            
                if (fabs((*tree)->x - x) <= bestDistance) {
                    // We need to check the other branch in this case.
                    bestNode = nearestOf(nearestNeighbourSearch(&(*tree)->left, bestNode, bestDistance, x, y, 1, comparisons), bestNode, x, y);
                }
            }
        }
    } else if (ticker == 1) {
        // check y value of key
        if (y < (*tree)->y) {
            // Go left
            // If this call can't find a better node, it will just return the same node back.
            bestNode = nearestNeighbourSearch(&(*tree)->left, bestNode, bestDistance, x, y, 0, comparisons);
            // above call won't update local "bestDistance" in local call, we need to do that here:
            sneakyDistance = distanceBetween(bestNode, x, y);
            if (sneakyDistance < bestDistance) {
                bestDistance = sneakyDistance;
            }
            // When the above recursive call returns, it should come down to this logic :)
            // We've checked the "good" partitions, now we should the other side
            if ((*tree)->right != NULL) {
                if (fabs((*tree)->y - y) <= bestDistance) {
                    // We need to check the other branch in this case.
                    bestNode = nearestOf(nearestNeighbourSearch(&(*tree)->right, bestNode, bestDistance, x, y, 0, comparisons), bestNode, x, y);
                }
            }
        } else {
            // Go right
            // If this call can't find a better node, it will just return the same node back.
            bestNode = nearestNeighbourSearch(&(*tree)->right, bestNode, bestDistance, x, y, 0, comparisons);
            // above call won't update local "bestDistance" in local call, we need to do that below:
            sneakyDistance = distanceBetween(bestNode, x, y);
            if (sneakyDistance < bestDistance) {
                bestDistance = sneakyDistance;
            }
            // When the above recursive call returns, it should come down to this logic :)
            // We've checked the "good" partitions, now we should the other side
            if ((*tree)->left != NULL) {
                if (fabs((*tree)->y - y) <= bestDistance) {
                // We need to check the other branch in this case.
                bestNode = nearestOf(nearestNeighbourSearch(&(*tree)->left, bestNode, bestDistance, x, y, 0, comparisons), bestNode, x, y);
                }
            }
        }
    }
    return bestNode;
}


/*
Searches through tree for points inside given radius.
If a node is inside the radius, everything its list pointer points to will be written to 
the specified file.
numberOfMatches should be passed in as a pointer to int, which should start at zero.
Soooooooooooooo much better than nearestNeighbourSearch.
*/
void radiusSearch(struct bst **tree, double radius, double x, double y, int ticker, int *comparisons, int *numberOfMatches, FILE *ouputFile) {

    if (*tree == NULL) {
        return;
    }
    (*comparisons)++;

    double newDistance = distanceBetween(*tree, x, y);

    if (newDistance <= radius) {
        // notify outside observer that we've found at least one match inside radius
        (*numberOfMatches)++;
        // write node to file
        writeList((*tree)->list, &(*ouputFile));
        // go down both branches
        if (ticker == 0) {
            ticker = 1;
        } else {
            ticker = 0;
        }
        radiusSearch(&(*tree)->left, radius, x, y, ticker, comparisons, numberOfMatches, ouputFile);
        radiusSearch(&(*tree)->right, radius, x, y, ticker, comparisons, numberOfMatches, ouputFile);
    } else {
        // the current node is outside the radius
        if (ticker == 0) {
            // check x value of key 
            // first, check if current node partitions the radius on both sides
            if (fabs(x - (*tree)->x) <= radius) {
                // If it does; go down both branches
                radiusSearch(&(*tree)->left, radius, x, y, 1, comparisons, numberOfMatches, ouputFile);
                radiusSearch(&(*tree)->right, radius, x, y, 1, comparisons, numberOfMatches, ouputFile);
            }else if (x < (*tree)->x) {
                // Go left
                radiusSearch(&(*tree)->left, radius, x, y, 1, comparisons, numberOfMatches, ouputFile);
            } else {
                // Go right
                radiusSearch(&(*tree)->right, radius, x, y, 1, comparisons, numberOfMatches, ouputFile);
            }
        } else if (ticker == 1) {
            // check y value of key
            // first, check if current node partitions the radius on both sides
            if (fabs(y - (*tree)->y) <= radius) {
                // If it does; go down both branches
                radiusSearch(&(*tree)->left, radius, x, y, 0, comparisons, numberOfMatches, ouputFile);
                radiusSearch(&(*tree)->right, radius, x, y, 0, comparisons, numberOfMatches, ouputFile);
            } else if (y < (*tree)->y) {
                // Go left
                radiusSearch(&(*tree)->left, radius, x, y, 0, comparisons, numberOfMatches, ouputFile);
            } else {
                // Go right
                radiusSearch(&(*tree)->right, radius, x, y, 0, comparisons, numberOfMatches, ouputFile);
            }
        }
    }
    return;
}


/*
Frees whole tree, including nested lists at nodes.
*/
void freeTree(struct bst *tree) {

    if (tree) {
        freeTree(tree->left);
        freeTree(tree->right);
        freeList(tree->list);
        free(tree);
    }
}


/*
Shows me if my tree looks how it should during testing
Prints tree in traditional sorted order.
*/
void printTree(struct bst *tree) {

    if (tree) {
        printTree(tree->left);
        printf("%s\n(%lf,%lf)\n", tree->list->record->tradingName, tree->x, tree->y);
        printTree(tree->right);
    }
}
