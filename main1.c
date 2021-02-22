/*
Handles nearest neighbour searches
Inputs should be in the command line as follws: csvFileName.csv nameOfResultsOutputFile < redirectedStdin
redirectedStdin should be two coordinates like this: 144.666 37.54256
./map1 CLUEdata2018_median.csv nearestOutput < testNearest
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>
#include "kdTree.h"
#include "list.h"
#include "entry.h"

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Incorrect number of program inputs; only using first two\n");
    }
    // Build KD tree using input file
    struct bst *treeKD = buildTreeKD(argv[1]);

    // Open file to send search results to
    FILE *outputData = fopen(argv[2], "w");
    assert(outputData);

    // Variable will store the number of comparisons for each search.
    int comparisonCounter = 0;
    int *comparisonCounterPointer = &comparisonCounter;

    // Read search coordinates in and search tree, outputing results to outputData, and comparisons to stdout.
    double searchX;
    double searchY;
    while (scanf("%lf %lf", &searchX, &searchY) == 2) {

        // Store closest node
        writeList(nearestNeighbourSearch(&treeKD, NULL, DBL_MAX, searchX, searchY, 0, comparisonCounterPointer)->list, outputData);

        // Print number of comparisons to stdout.
        printf("%lf %lf --> %d\n", searchX, searchY, comparisonCounter);
        comparisonCounter = 0;
    }
    fclose(outputData);
    freeTree(treeKD);
    return 0;
}
