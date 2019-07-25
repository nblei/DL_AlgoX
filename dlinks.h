#ifndef __DLINKS_H__
#define __DLINKS_H__
#include <stdlib.h>

/* Dancing links node */
struct dl_node {
    struct dl_node *up, *down, *left, *right;
    int row;
    int col;
};

struct dlinks {
    int N;  // Number of items in the universe set [0..N-1]
    struct dl_node * nodes;
    struct dl_node h;
    int * header_sizes;
    int * sols; // solutions to the exact cover problem
    size_t sols_len;
};

int
dl_init(struct dlinks * dl, int N, char ** subsets, int nsubs);

void
dl_algoX(struct dlinks *dl, int k);

#endif

