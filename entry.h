/*
Contains all functions and structures for entries
An entry is just a struct representing a line from the CLUE csv file.
*/
#ifndef ENTRY_H
#define ENTRY_H


struct entry {
  char *censusYear;
  char *blockID;
  char *propertyID;
  char *basePropertyID;
  char *smallArea;
  char *tradingName;
  char *industryCode;
  char *industryDescription;
  char *xCoordinate;
  char *yCoordinate;
  char *location;
};

struct entry *createRecord(char *string);

void insertField(struct entry *record, char *currentValue, int column);

void writeStructToFile(struct entry *record, FILE *outputFile);

char *wordUpTo(char *word, char *subchars);

void freeEntry(struct entry *pntr);
#endif
