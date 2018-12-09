#include <string.h>

#include "fileReader.h"
#include "printUtils.h"

/*void commentTestFunction1(int x) {
    x = 1;
    malloc();
}*/

//int commentTestFunction2(int x, int y);

int main(int argc, char **argv) {
    FunctionStack *functionStack = malloc(sizeof(FunctionStack));

    UsageStack *definitionStack = malloc(sizeof(UsageStack));
    UsageStack *prototypeStack = malloc(sizeof(UsageStack));
    UsageStack *usageCallStack = malloc(sizeof(UsageStack));

    int i = 1;
    for (; i < argc; i++) {
        FILE *inputFile = fopen(argv[i], "r");
        if (inputFile == NULL) {
            continue;
        }

        setFile(inputFile);

        int numBrackets = 0;
        int numBraces = 0;

        Lexem nextLexem = getNextLexem();
        while (nextLexem != END_OF_FILE) {

            // Identifier - if opening parentheses is next then it's a function
            if (nextLexem == IDENTIFIER) {
                char *tempIdentifier = malloc(sizeof(char) * strlen(getLastIdentifier()));
                strcpy(tempIdentifier, getLastIdentifier());

                Lexem next = getNextLexem();
                if (next == OPEN_BRACKET) {
                    numBrackets++;
                    putOnFunctionStack(functionStack, numBrackets, tempIdentifier);
                } else {
                    nextLexem = next;
                    continue;
                }
            }

            // Opening parentheses
            else if (nextLexem == OPEN_BRACKET) {
                numBrackets++;
            }
            
            // Closing parentheses - can be function definition, prototype or call
            else if (nextLexem == CLOSE_BRACKET) {
                int topNumBrackets = -1;
                Function *topFunction = functionStack -> top;

                if (topFunction != NULL) {
                    topNumBrackets = topFunction -> numBrackets;
                }

                if (topNumBrackets == numBrackets--) {
                    Lexem next = getNextLexem();

                    // Opening brace - definition
                    if (next == OPEN_BRACE) {
                        putOnUsageStack(definitionStack, getFromFunctionStack(functionStack), argv[i], getLineNumber(), -1);
                        numBraces++;
                    }
                    
                    // Probably a semicolon - prototype
                    else if (numBraces == 0) {
                        putOnUsageStack(prototypeStack, getFromFunctionStack(functionStack), argv[i], getLineNumber(), getLineNumber());
                    }
                    
                    // Nothing special - call
                    else {
                        putOnUsageStack(usageCallStack, getFromFunctionStack(functionStack), argv[i], getLineNumber(), getLineNumber());
                    }

                    // Closing parentheses next - this function is inside another one, this needs to be checked again
                    if (next == CLOSE_BRACKET) {
                        nextLexem = next;
                        continue;
                    }
                }
            }
            
            // Opening brace
            else if (nextLexem == OPEN_BRACE) {
                numBraces++;
            }
            
            // Closing brace - if this is a definition and then set the definition ending line number
            else if (nextLexem == CLOSE_BRACE) {
                numBraces--;

                if (numBraces == 0 && !strcmp(argv[i], definitionStack -> top -> fileName)) {
                    definitionStack -> top -> endLineNumber = getLineNumber();
                }
            }

            nextLexem = getNextLexem();
        }
    }

    // Print all the definitions and its prototype, usages and calls
    FunctionUsage *definition = definitionStack -> top;

    while (definition != NULL) {
        printf("Function %s():\n", definition -> functionName);
        printf("  Definition:\n");
        printf("    File %s, lines %d to %d\n", definition -> fileName, definition -> startLineNumber, definition -> endLineNumber);

        printPrototype(prototypeStack, definition);
        printUsages(usageCallStack, definition);
        printCalls(usageCallStack, definition);

        printf("\n");
        definition = definition -> prev;
    }

    // Free the stacks memory
    freeFunctionStack(functionStack);

    freeUsageStack(definitionStack);
    freeUsageStack(prototypeStack);
    freeUsageStack(usageCallStack);

    return 0;
}