/*
Contains all functions for entries.
Entry is a data structure representing a line from the CLUE csv file.
TODO: refactor this entire file.
*/
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "entry.h"


/* 
Creates a new struct entry, and reads all of the values of a
given csv line into the appropriate fields of the struct.
Each struct variable will need to be freed after use.
*/
struct entry *createRecord(char *string) {

	struct entry *record = malloc(sizeof(struct entry));
    assert(record);

	char *stringCopy = strdup(string);
	char *startLastWord = stringCopy; // Points to first letter to start with.
	char *newWord = NULL;
	int i = 0;
	int column = 0;
	int TOTAL_COLUMNS = 11;
	while (column < TOTAL_COLUMNS) {

		// Deals with vanilla comma's.
		if (stringCopy[i] == ',') {
			column++;
			newWord = wordUpTo(startLastWord, ",");
			// Add column value to record
			insertField(record, newWord, column);
			// Move pointer to value after comma.
			startLastWord = &(stringCopy[i + 1]);
			i++;
			continue;
		}
		// Deals with location coordinates at end of line.
		if (string[i] == '\"' && column == 10) {
			column++;
			// Moves pointer past the " so wordUpTo doesn't return empty string.
			startLastWord = &(stringCopy[i + 1]); // Note: will cause error if location is in double quotes.
			newWord = wordUpTo(startLastWord, "\"");

			// Add column value to record
			insertField(record, newWord, column);
			break;
		}
		// Deals with double or more quotes.
		if (string[i] == '\"') {
			column++;
			startLastWord = &(stringCopy[i + 1]);
			newWord = wordUpTo(startLastWord, "\",");

			// Add column value to record
			insertField(record, newWord, column);

			// Makes string[i] point to next value after comma after last set of quotes.
			i += strlen(newWord) + 3;
			startLastWord = &(stringCopy[i]);
			continue;
		}
		i++;
	}
	free(stringCopy);
	return record;
}


/*
Adds given string into struct entry, depending on given column number.
*/
void insertField(struct entry *record, char *currentValue, int column) {

	switch (column) {
		case 1:
			record->censusYear = currentValue;
			break;
		case 2:
			record->blockID = currentValue;
			break;
		case 3:
			record->propertyID = currentValue;
			break;
		case 4:
			record->basePropertyID = currentValue;
			break;
		case 5:
			record->smallArea = currentValue;
			break;
		case 6:
			record->tradingName = currentValue;
			break;
		case 7:
			record->industryCode = currentValue;
			break;
		case 8:
			record->industryDescription = currentValue;
			break;
		case 9:
			record->xCoordinate = currentValue;
			break;
		case 10:
			record->yCoordinate = currentValue;
			break;
		case 11:
			record->location = currentValue;
			break;
	}
}


/* 
Write struct to given file, in format specified by assignment. Assumes file has been opened. 
*/
void writeStructToFile(struct entry *record, FILE *outputFile) {
	// xCoordinate and yCoordinate at the end of output don't neeed to be rounded to 4 places (Piazza).
	fprintf(outputFile, "%s %s --> Census year: %s || Block ID: %s || Property ID: %s || Base property ID: %s || CLUE small area: %s || Trading Name: %s || Industry (ANZSIC4) code: %s || Industry (ANZSIC4) description: %s || x coordinate: %s || y coordinate: %s || Location: %s || \n", record->xCoordinate, record->yCoordinate, record->censusYear, record->blockID, record->propertyID, record->basePropertyID, record->smallArea, record->tradingName, record->industryCode, record->industryDescription, record->xCoordinate, record->yCoordinate, record->location);
}


/* 
Takes string and substring as args
Returns String up to occurence of substring, but not including it.
Designed to find and cut off ", in strings
Returned word needs to be freed after use.
Does not remove quotation mark at start of string.
Strtok() gives me nightmares.
*/
char *wordUpTo(char *word, char *subchars) {

	char *startOfStringToCopy = strdup(word);
	assert(startOfStringToCopy);
	
	char *commaLocation = strstr(startOfStringToCopy, subchars);
	*commaLocation = '\0'; // Modifies the actual string inputted and cuts off everything after and including the comma.

	char *outputWord = strndup(startOfStringToCopy, strlen(startOfStringToCopy) + 1);
	assert(outputWord);

	free(startOfStringToCopy);

	return outputWord;
}


/*
Frees all variables in entry
*/
void freeEntry(struct entry *pntr) {
    free(pntr->censusYear);
    free(pntr->blockID);
    free(pntr->propertyID);
    free(pntr->basePropertyID);
    free(pntr->smallArea);
    free(pntr->tradingName);
    free(pntr->industryCode);
    free(pntr->industryDescription);
    free(pntr->xCoordinate);
    free(pntr->yCoordinate);
    free(pntr->location);
	free(pntr);
}
