#include <stdlib.h>
#include <string.h>

#include "stack.h"

// Puts a new function on a given stack (copies the function name to a new allocation unit)
void putOnFunctionStack(FunctionStack *stack, int numBrackets, char *functionName) {
    Function *function = malloc(sizeof(Function));
    function -> numBrackets = numBrackets;

    // Copy the funtion name into a new memory location - given name pointer will change its value
    function -> functionName = malloc(sizeof(char) * strlen(functionName));
    strcpy(function -> functionName, functionName);

    function -> prev = stack -> top;
    stack -> top = function;
}

// Puts the function on a usage stack
void putOnUsageStack(UsageStack *stack, char *functionName, char *fileName, int startLineNumber, int endLineNumber) {
    FunctionUsage *usage = malloc(sizeof(FunctionUsage));

    usage -> functionName = functionName;
    usage -> fileName = fileName;
    usage -> startLineNumber = startLineNumber;
    usage -> endLineNumber = endLineNumber;

    usage -> prev = stack -> top;
    stack -> top = usage;
}

// Puts the function name on a stack
void putOnNameStack(NameStack *stack, char *functionName) {
    FunctionName *name = malloc(sizeof(FunctionName));
    name -> functionName = functionName;

    name -> prev = stack -> top;
    stack -> top = name;
}

// Frees function stack memory and its items memory
void freeFunctionStack(FunctionStack *stack) {
    Function *function = stack -> top;
    while (function != NULL) {
        free(function -> functionName);
        free(function);

        function = function -> prev;
    }

    free(stack);
}

// Frees usage stack memory and its items memory
void freeUsageStack(UsageStack *stack) {
    FunctionUsage *usage = stack -> top;
    while (usage != NULL) {
        free(usage);
        usage = usage -> prev;
    }

    free(stack);
}

// Frees name stack memory and its items memory
void freeNameStack(NameStack *stack) {
    FunctionName *name = stack -> top;
    while (name != NULL) {
        //free(name);
        name = name -> prev;
    }

    free(stack);
}

// Takes a function from the top of a stack and returns its name
char *getFromFunctionStack(FunctionStack *stack) {
    Function *function = stack -> top;
    if (function == NULL) {
        return "";
    }

    stack -> top = function -> prev;
    return function -> functionName;
}