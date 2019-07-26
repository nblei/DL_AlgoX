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
    int nsubs; // Number of subsets
    struct dl_node * nodes;
    struct dl_node h;
    int * header_sizes;
    int * sols; // solutions to the exact cover problem
    size_t sols_len;
    // Callback called when exact cover is found
    // void * is caller designated argument
    void (*cover_cb)(struct dlinks *, void *);
    void * cb_arg;
    // set_vec contains an array of set indices 
    // When cover_cb is called, set_vec contains indices of each
    // set in the exact cover
    struct {
        int size;
        int * vec;
    } set_vec;
};

int
dl_init(struct dlinks * dl, int N, char ** subsets, int nsubs);


void
dl_free(struct dlinks * dl);

void
dl_algoX(struct dlinks *dl, void (*cover_cb)(struct dlinks *, void *),
        void * cb_arg);

#endif

