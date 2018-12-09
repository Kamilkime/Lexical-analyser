#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printUtils.h"

// Prints the function prototype for the given definition
void printPrototype(UsageStack *prototypeStack, FunctionUsage *definition) {
    int hasPrototype = 0;
    printf("  Prototype:\n");

    FunctionUsage *prototype = prototypeStack -> top;
    while (prototype != NULL) {
        if (!strcmp(prototype -> functionName, definition -> functionName)) {
            printf("    File %s, lines %d to %d\n", prototype -> fileName, prototype -> startLineNumber, prototype -> endLineNumber);

            hasPrototype = 1;
            break;
        }

        prototype = prototype -> prev;
    }

    if (!hasPrototype) {
        printf("    NO PROTOTYPE\n");
    }
}

// Prints all lines where given function (definition) is used
void printUsages(UsageStack *usageStack, FunctionUsage *definition) {
    int hasUsage = 0;
    printf("  Usage:\n");

    FunctionUsage *usage = usageStack -> top;
    while (usage != NULL) {
        if (!strcmp(usage -> functionName, definition -> functionName)) {
            printf("    File %s, lines %d to %d\n", usage -> fileName, usage -> startLineNumber, usage -> endLineNumber);
            hasUsage = 1;
        }

        usage = usage -> prev;
    }

    if (!hasUsage) {
        printf("    NO USAGE\n");
    }
}

// Prints all function where given function (definition) is calling
void printCalls(UsageStack *callStack, FunctionUsage *definition) {
    NameStack *nameStack = malloc(sizeof(NameStack));
    int hasCalls = 0;

    printf("  Calls:\n");

    FunctionUsage *call = callStack -> top;
    while (call != NULL) {
        if (!isInsideDefinition(call, definition)) {
            call = call -> prev;
            continue;
        }

        int alreadyFound = 0;

        // Check if the calls have already been calculated for this function
        FunctionName *nameStackItem = nameStack -> top;
        while (nameStackItem != NULL) {
            if (!strcmp(nameStackItem -> functionName, call -> functionName)) {
                alreadyFound = 1;
                break;
            }

            nameStackItem = nameStackItem -> prev;
        }

        if (!alreadyFound) {
            int foundCalls = 0;

            // Find and count all the calls inside the definition
            FunctionUsage *tempCall = callStack -> top;
            while (tempCall != NULL) {
                if (!strcmp(tempCall -> functionName, call -> functionName) && isInsideDefinition(tempCall, definition)) {
                    foundCalls++;
                }

                tempCall = tempCall -> prev;
            }

            printf("    %s() - %d time\n", call -> functionName, foundCalls);
            putOnNameStack(nameStack, call -> functionName);

            hasCalls = 1;
        }

        call = call -> prev;
    }

    if (!hasCalls) {
        printf("    NO CALLS\n");
    }

    freeNameStack(nameStack);
}

// Returns true if the given function call is inside the given function definition
int isInsideDefinition(FunctionUsage *call, FunctionUsage *definition) {
    return (!strcmp(call -> fileName, definition -> fileName)) && (call -> startLineNumber > definition -> startLineNumber)
        && (call -> endLineNumber < definition -> endLineNumber);
}