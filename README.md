# DL_AlgoX

C implementation of Donald Knuth's Dancing Links data structure and Algorithm X, which solves the exact cover problem.
This implementation uses 
&theta;(NM)
space, where `N` is the size of the universe set, and `M` is the number of subsets
considered.  The exact cover problem is NP-Complete and takes time exponential in the number of subsets.

This implementation is single-threaded (if you know of any multi-threaded implementations of algorithm-X, let me know,
I'd love to see them).

## Structure

Although Knuth's paper on Dancing Links describes several different structures (Column Object, Column Header Object, Node Object),
this implementation uses only a single `struct dl_node` structure to represent columns, column headers, and nodes.  Since
memory is so readily available, and this problem is in NP-Complete and PSPACE, it makes sense to 
trade a small amount of additional
space for a large increase in ease of programming, and memory management 
(additionally, as the Dancing Links are allocated at initialization as a one dimensional array,
this likely leads to an increase
of performance due to cache considerations).

In addition to the node structure, there is `struct dlinks` which contains all of the metadata to run
Algorithm X on the initialized Dancing Links.  Thus, `struct dlinks` is the base structure used in this program.



## Usage


### Initiailization
Since `struct dlinks` itself is small, the user can be allocated safely on the stack.
To initialize the Dancing Links,
pass a pointer to a `struct dlinks` to `dl_init`, along with an integer, `N`, representing the size of the universe set,
a `char ** subsets` which is an `nsubs` length array of `char *`, each of length `N`, such that
`subsets[i][j] = 1` indicates that the `i`'th set contains element `j` of the universe set, and
`subsets[i][j] = 0` indcates that the `i`th set does not contain element `j` of the universe set.  The
final argument, `nsubs`, is the number of subsets encoded by `subsets`.  The caller is responsible for
allocating and freeing memory space for `subset`.  All other memory allocation is handled by `dl_init`.

The function `dl_algoX` actually executes the algorithm.  This function finds every exact cover.  For each
exact cover found, `dl_algoX` calls `cover_cb` with `cb_arg` as an argument.  If `cover_cb` is `NULL`, then
the function call does not take place.

TODO: Add some type of limit behavior to `dl_algoX` so that the caller can
determine the number of solutions to find (often, only one is sufficient).

