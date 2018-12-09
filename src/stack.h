#ifndef _STACK_H_IS_INCLUDED_
#define _STACK_H_IS_INCLUDED_

// Usage of a function - definition, prototype or a call
typedef struct FunctionUsage {
    char *functionName;
    char *fileName;
    int startLineNumber;
    int endLineNumber;
    struct FunctionUsage *prev;
} FunctionUsage;

// A function
typedef struct Function {
    char *functionName;
    int numBrackets;
    struct Function *prev;
} Function;

// Function name - for stack storage
typedef struct FunctionName {
    char *functionName;
    struct FunctionName *prev;
} FunctionName;

// Function stack
typedef struct {
    struct Function *top;
} FunctionStack;

// Usage stack
typedef struct {
    struct FunctionUsage *top;
} UsageStack;

// Name stack
typedef struct {
    struct FunctionName *top;
} NameStack;

// Puts a new function on a given stack (copies the function name to a new allocation unit)
void putOnFunctionStack(FunctionStack *stack, int numBrackets, char *functionName);

// Puts the function on a usage stack
void putOnUsageStack(UsageStack *stack, char *functionName, char *fileName, int startLineNumber, int endLineNumber);

// Puts the function name on a stack
void putOnNameStack(NameStack *stack, char *functionName);

// Frees function stack memory and its items memory
void freeFunctionStack(FunctionStack *stack);

// Frees usage stack memory and its items memory
void freeUsageStack(UsageStack *stack);

// Frees name stack memory and its items memory
void freeNameStack(NameStack *stack);

// Takes a function from the top of a stack and returns its name
char *getFromFunctionStack(FunctionStack *stack);

#endif