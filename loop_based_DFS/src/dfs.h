// here is the actual loop- based deep field research algorithm
//
#pragma once
#include "structures.h"

void push(int node, int *stack, int *top, int *visided);

int pop(int *stack, int *top); // removes the node and returns the top noode

int is_empty(int top);

void dfs(void);
