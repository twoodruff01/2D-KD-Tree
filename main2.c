/*
Handles radius searches
Inputs should be in the command line as follows: csvFileName.csv nameOfResultsOutputFile < redirectedStdin
redirectedStdin should be two coordinates and a radius like this: 144.666 37.54256 0.0005
./map2 CLUEdata2018_median.csv radiusOutput < testRadius
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
        printf("Incorrect number of programs inputs; only using first two\n");
    }
    // Build KD tree using input file
    struct bst *treeKD = buildTreeKD(argv[1]);

    // Open file to send search results to
    FILE *outputData = fopen(argv[2], "w");
    assert(outputData);

    // Variable will store the number of comparisons for each search.
    int comparisonCounter = 0;
    int *comparisonCounterPointer = &comparisonCounter;

    // Variable will store the number of nodes inside the radius, and therefore tell us if nothing is found.
    int numberOfMatches = 0;
    int *numberOfMatchesPointer = &numberOfMatches;

    // Read search coordinates in and search tree, outputing results to outputData, and comparisons to stdout.
    double searchX;
    double searchY;
    double searchRadius;
    while (scanf("%lf %lf %lf", &searchX, &searchY, &searchRadius) == 3) {

        // Store all nodes which are inside the radius
        radiusSearch(&treeKD, searchRadius, searchX, searchY, 0, comparisonCounterPointer, numberOfMatchesPointer, outputData);

        // Check whether we've actually found anything inside the radius.
        if (numberOfMatches == 0) {
            // If we haven't we should write something to the file:
            fprintf(outputData, "%lf %lf %lf -->NOTFOUND\n", searchX, searchY, searchRadius);
        }

        // Print number of comparisons to stdout.
        printf("%lf %lf %lf --> %d\n", searchX, searchY, searchRadius, comparisonCounter);
        comparisonCounter = 0;
        numberOfMatches = 0;
    }
    fclose(outputData);
    freeTree(treeKD);
    return 0;
}
