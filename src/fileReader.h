#ifndef _FILEREADER_H_IS_INCLUDED_
#define _FILEREADER_H_IS_INCLUDED_

#include <stdio.h>
#include <stdlib.h>

// The lexem enum (NONE means an expression which is not relevant for the analysis)
typedef enum Lexem {END_OF_FILE, OPEN_BRACKET, CLOSE_BRACKET, OPEN_BRACE, CLOSE_BRACE, IDENTIFIER, KEYWORD, NONE} Lexem;

// Sets the file to check lexems for
void setFile(FILE *file);

// Return true if the given character is a whitespace
int isSpaceChar(char c);

// Returns true if the given identifier is a keyword
int isKeyword(char *word);

// Gets the next character from the file, or returnes currentChar if recheckChar is true
char getNextChar();

// Returns the next lexem in the file
Lexem getNextLexem();

// Returns the last read identifier
char *getLastIdentifier();

// Returns the current line number
int getLineNumber();

#endif