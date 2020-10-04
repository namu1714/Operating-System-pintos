#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hash.h"
#include "bitmap.c"
#include "bitmap.h"

#define MAX_DATA_SIZE 10
#define MAX_COMMAND_SIZE 100
#define MAX_ARGS 6

#define CREATE 1
#define DELETE 2
#define DUMP   3
#define OTHERS 4

//global variables
struct list *L[MAX_DATA_SIZE];
struct hash *H[MAX_DATA_SIZE];
struct bitmap *B[MAX_DATA_SIZE];

char args[MAX_ARGS][25]; //arguments

//functions
void processCommand(char *cmd);
bool stringToBool(char *s);

void createHandler();
void deleteHandler();
void dumpdataHandler();

void listFunctions(int flag);
void hashFunctions(int flag);
void bitmapFunctions(int flag);
