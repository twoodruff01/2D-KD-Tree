This is most of my code for assignments 1 and 2 for algorithms and data structures at Melbourne Uni in 2020
I wrote *all* of it.

The dataset used https://data.melbourne.vic.gov.au/Business/Business-establishment-trading-name-and-industry-c/vesm-c7r2 
is basically a record of all the registered businesses in Melbourne and their locations.

Assignment 1 mainly involved writing a *CSV parser* for the dataset (entry.c and entry.h) which was a huge pain-in-the-ass 
due to unescaped commas in the dataset, and the fact that I'd never used pointers before.

Assignment 2 involved creating a *2 dimensional KD tree* which would take the businesses' parsed details and store each 
business in the KD tree based on its coordinates. Building the KD tree was extremely challenging, but rewarding when it 
actually worked. It allows for super fast location based searching of the dataset.
Duplicate location coordinates for different businesses are handled by storing them in the same node in the KD tree, in
a linked list connected to that node. This is why simple searches can output many business details.

----------------------------------------------------------------------------------------------------------------------------

**Compiling**

In the command-line:

make

- This will create two files: map1 and map2
- There are two main functions of the program:

map1 -> Performs a *nearest-neighbour search* using given coordinates
map2 -> Performs a *radius search* around given coordinates, with a given radius to search

----------------------------------------------------------------------------------------------------------------------------

**Nearest-Neighbour Search**

To perform this search on a bunch of different queries, run:

./map1 Data/CLUEdata2018_random.csv output_file < Data/queriesStage1

The number of comparisons for each query will be shown on your terminal, and the results will be in output_file

----------------------------------------------------------------------------------------------------------------------------

**Radius Search**

To perform this search on a bunch of different queries, run:

./map2 Data/CLUEdata2018_random.csv output_file < Data/queriesStage2

The number of comparisons for each query will be shown on your terminal, and the results will be in output_file
