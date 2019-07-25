#include <assert.h>
#include "dlinks.h"
#include <string.h>
#include <stdio.h>

static int
dl_idx(struct dlinks * dl, int row, int col);

static void
dl_add_sol(struct dlinks *dl);

static struct dl_node *
dl_choose_col(struct dlinks *dl);

static void
dl_cover_col(struct dl_node * chead);

static void
dl_uncover_col(struct dl_node * chead);


int
dl_init(struct dlinks * dl, int N, char **subsets, int nsubs)
{
    // Create a matrix of nsubs + 1 rows and N cols.  Row 0 is the 
    // header row.  All pointers initialized to NULL by calloc
    dl->nodes = NULL; dl->header_sizes = NULL;
    dl->nodes = calloc((nsubs + 1) * N, sizeof(*dl->nodes));
    dl->header_sizes = calloc(N, sizeof(*dl->header_sizes));
    if ((dl->header_sizes == NULL) || (dl->nodes == NULL)) {
        perror("calloc"); goto fail_allocate;
    }

    // Set up the nodes
    for (int row = 0; row < nsubs+1; ++row) {
        for (int col = 0; col < N; ++col) {
            int idx = dl_idx(dl, row, col);
            dl->nodes[idx].row = row;
            dl->nodes[idx].col = col;
        }
    }

    // Set up header row
    for (int col = 0; col < N; ++col) {
        int lcol = col == 0 ? N-1 : col-1;
        int rcol = col == N-1 ? 0 : col+1;
        dl->nodes[col].left = &dl->nodes[lcol];
        dl->nodes[col].right = &dl->nodes[rcol];
    }

    // The first and last column headers must be set to point to 'h'
    dl->nodes[0].left = &dl->h;
    dl->nodes[N-1].right = &dl->h;

    // Link used nodes
    // First, link vertically
    for (int col = 0; col < N; ++col) {
        struct dl_node * valid = &dl->nodes[col];
        for (int setidx = 0; setidx < nsubs; ++setidx) {
            if (subsets[setidx][col] == 0)
                continue;
            int nidx = dl_idx(dl, setidx+1, col);
            valid->down = &dl->nodes[nidx];
            dl->nodes[nidx].up = valid;
            valid = &dl->nodes[nidx];
        }
        // Make array circular
        valid->down = &dl->nodes[col];
        dl->nodes[col].up = valid;
    }

    // Next, link horizontally
    for (int setidx = 0; setidx < nsubs; ++setidx) {
        struct dl_node * valid = NULL;
        struct dl_node * first = NULL;
        for (int col = 0; col < N; ++col) {
            if (subsets[setidx][col] == 0)
                continue;
            int nidx = dl_idx(dl, setidx+1, col);
            if (first == NULL) {
                valid = first = &dl->nodes[nidx];
            }
            valid->right = &dl->nodes[nidx];
            dl->nodes[nidx].left = valid;
            valid = &dl->nodes[nidx];
        }
        // Make array circular
        valid->right = first;
        first->left = valid;
    }
    dl->sols = NULL;
    dl->sols_len = 0;
    dl->h.right = &dl->nodes[0];
    dl->h.left = &dl->nodes[N-1];
    dl->h.col = dl->h.row = -1;
    dl->h.up = dl->h.down = NULL;

    return 0;
fail_allocate:
    free(dl->header_sizes);
    free(dl->nodes);
    return -1;
}


void
dl_algoX(struct dlinks *dl, int k)
{
    if (dl->h.right == &dl->h) {
        dl->sols_len += 1;
        //dl_add_sol(dl);
        return;
    }
    struct dl_node * const colh = dl_choose_col(dl);
    // Cover Column
    dl_cover_col(colh);
    
    struct dl_node * colptr = colh->down;
    for (; colptr != colh; colptr = colptr->down) {
        assert(colptr != NULL);
        struct dl_node * other_col = colptr->right;
        for (; other_col != colptr; other_col = other_col->right) {
            // Cover other column
            // Not actually a  column pointer, so need to grab it
            dl_cover_col(&dl->nodes[other_col->col]);
        }
        // Recurse!
        dl_algoX(dl, k+1);
        other_col = colptr->right;
        for (; other_col != colptr; other_col = other_col->right) {
            // Uncover other column
            dl_uncover_col(&dl->nodes[other_col->col]);
        }
    }
    // Uncover colh
    dl_uncover_col(colh);
}


struct dl_node *
dl_choose_col(struct dlinks * dl)
{
    // Keeping it simple
    return dl->h.right;
    // TODO use size
}


static void
dl_cover_col(struct dl_node * chead)
{
    // Remove From the header list
    chead->left->right = chead->right;
    chead->right->left = chead->left;

    // Remove ALL sets (rows) which contain this member (column)
    struct dl_node * d = chead->down;
    for (; d != chead; d = d->down) {
        struct dl_node *r = d->right;
        for (; r != d; r = r->right) {
            r->up->down = r->down;
            r->down->up = r->up;
            // TODO change size of column header for 'r'
            // See Knuth paper page 6
        }
    }
}


static void
dl_uncover_col(struct dl_node * chead)
{
    struct dl_node * u = chead->up;
    for (; u != chead; u = u->up) {
        struct dl_node * l = u->left;
        for (; l != u; l = l->left) {
            // TODO Size of column header for 'l'
            // See Knuth paper page 6
            l->up->down = l;
            l->down->up = l;
        }
    }
    // Insert column header
    chead->left->right = chead;
    chead->right->left = chead;
}


static int
dl_idx(struct dlinks * dl, int row, int col)
{
    return row * (dl->N) + col;
}

#include <alloca.h>
int main(void)
{
    char ** sets = alloca(5 * sizeof(*sets));
    for (int i = 0; i < 5; ++i) {
        sets[i] = alloca(3);
    }
    sets[0][0] = sets[0][1] = sets[0][2] = 1;
    sets[1][0] = sets[1][1] = 1;
    sets[1][2] = 0;
    sets[2][2] = 1;
    sets[2][0] = sets[2][1] = 0;
    sets[3][0] = 1;
    sets[3][1] = sets[3][2] = 0;
    sets[4][1] = 1;
    sets[4][0] = sets[4][2] = 0;

    struct dlinks dlink;
    dl_init(&dlink, 3, sets, 5);
    dl_algoX(&dlink, 0);
    printf("%lu\n", dlink.sols_len);
    return 0;
}
