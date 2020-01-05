#include <string.h>
#include <ctype.h>

#include "fileReader.h"

static int lineNumber;
static FILE *inputFile;
static char lastIdentifier[1024];

static char currentChar = '\0';
static int recheckChar = 0;

// Sets the file to check lexems for
void setFile(FILE *file) {
    lineNumber = 1;
    inputFile = file;
}

// Return true if the given character is a whitespace
int isSpaceChar(char c) {
    return c == ' ' || c == '\t';
}

// Returns true if the given identifier is a keyword
int isKeyword(char *word) {
    char *lowerWord = malloc(sizeof(char) * (strlen(word) + 1));

    int i = 0;
    for (; word[i]; i++) {
        lowerWord[i] = tolower(word[i]);
    }

    int isKeyword =  !strcmp(lowerWord, "char") || !strcmp(lowerWord, "const") || !strcmp(lowerWord, "double")
        || !strcmp(lowerWord, "float") || !strcmp(lowerWord, "int") || !strcmp(lowerWord, "long") || !strcmp(lowerWord, "short")
        || !strcmp(lowerWord, "struct") || !strcmp(lowerWord, "void") || !strcmp(lowerWord, "if") || !strcmp(lowerWord, "else")
        || !strcmp(lowerWord, "for") || !strcmp(lowerWord, "while") || !strcmp(lowerWord, "do") || !strcmp(lowerWord, "return");

    free(lowerWord);
    return isKeyword;
}

// Gets the next character from the file, or returnes currentChar if recheckChar is true
char getNextChar() {
    if (!recheckChar) {
        return fgetc(inputFile);
    }

    recheckChar = 0;
    return currentChar;
}

// Returns the next lexem in the file
Lexem getNextLexem() {
    while ((currentChar = getNextChar()) != EOF) {

        // Skip whitespace chars
        if (isSpaceChar(currentChar)) {
            continue;
        }

        // New line, change line number
        if (currentChar == '\n') {
            lineNumber++;
            continue;
        }

        // Opening parentheses
        if (currentChar == '(') {
            return OPEN_BRACKET;
        }

        // Closing parentheses
        if (currentChar == ')') {
            return CLOSE_BRACKET;
        }

        // Opening brace
        if (currentChar == '{') {
            return OPEN_BRACE;
        }

        // Closing brace
        if (currentChar == '}') {
            return CLOSE_BRACE;
        }

        // Single quotes - a character definition
        if (currentChar == '\'') {
            char prevPrevChar = currentChar;
            char prevChar = currentChar;
            
            // Skip the whole character definition, watch out for '\'' definition
            while ((currentChar = getNextChar()) != '\'' || (prevChar == '\\' && prevPrevChar != '\\')) {
                prevPrevChar = prevChar;
                prevChar = currentChar;
            }

            continue;
        }

        // Double quotes - string definition
        if (currentChar == '"') {
            char prevPrevChar = currentChar;
            char prevChar = currentChar;
            
            // Skip the whole string definition, watch out for "\"" definition
            while ((currentChar = getNextChar()) != '\"' || (prevChar == '\\' && prevPrevChar != '\\')) {
                prevPrevChar = prevChar;
                prevChar = currentChar;
            }

            continue;
        }

        // Forward slash - should be a comment
        if (currentChar == '/') {
            currentChar = getNextChar();

            // Single line comment - skip all characters until the end of the line and increase line number
            if (currentChar == '/') {
                while ((currentChar = getNextChar()) != '\n' && currentChar != EOF);
                lineNumber++;
            }
            
            // Multiline comment - skip all characters until */ is found, increase line number if needed
            else if (currentChar == '*') {
                while ((currentChar = getNextChar()) != EOF) {

                    // Line break - increase line number and keep skipping characters
                    if (currentChar == '\n') {
                        lineNumber++;
                        continue;
                    }

                    // Not an asterisk - keep skipping characters
                    if (currentChar != '*') {
                        continue;
                    }

                    // Last character was an asterisk, if this one is the forward slash - stop skipping
                    if (getNextChar() == '/') {
                        break;
                    }
                }

                continue;
            }

            // Not a start of a comment - recheck the character after slash
            recheckChar = 1;

            continue;
        }

        // Alphabetic character - can be a keyword or an identifier
        if (isalpha(currentChar)) {
            int i = 0;
            lastIdentifier[i++] = currentChar;

            while (isalnum(currentChar = getNextChar())) {
                lastIdentifier[i++] = currentChar;
            }

            lastIdentifier[i] = '\0';
            recheckChar = 1;
            
            return isKeyword(lastIdentifier) ? KEYWORD : IDENTIFIER;
        }

        // A number - just skip
        if (isdigit(currentChar) || currentChar == '.') {
            continue;
        }

        return NONE;
    }

    return END_OF_FILE;
}

// Returns the last read identifier
char *getLastIdentifier() {
    return lastIdentifier;
}

// Returns the current line number
int getLineNumber() {
    return lineNumber;
}
