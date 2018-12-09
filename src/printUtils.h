#ifndef _PRINTUTILS_H_IS_INCLUDED_
#define _PRINTUTILS_H_IS_INCLUDED_

#include "stack.h"

// Prints the function prototype for the given definition
void printPrototype(UsageStack *prototypeStack, FunctionUsage *definition);

// Prints all lines where given function (definition) is used
void printUsages(UsageStack *usageStack, FunctionUsage *definition);

// Prints all function where given function (definition) is calling
void printCalls(UsageStack *callStack, FunctionUsage *definition);

// Returns true if the given function call is inside the given function definition
int isInsideDefinition(FunctionUsage *call, FunctionUsage *definition);

#endif