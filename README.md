# DL_AlgoX

## Usage

struct dlinks itself is small and can be allocated safely on the stack.
Pass a pointer to a struct dlinks to dl_init, along with an integer, `N`, representing the size of the universe set,
a `char ** subsets` which is an array of `char` pointers (not strings), each of length `N`, such that
`subsets[i][j] = 1` indicates that the `i`'th set contains element `j` of the universe set, and
`subsets[i][j] = 0` indcates that the `i`th set does not contain element `j` of the universe set.  The
final argument, `nsubs`, is the number of subsets encoded by `subsets`.  The caller is responsible for
allocating and freeing memory space for `subset`.  All other memory allocation is handled by `dl_init`.

`dl_algoX` actually executes the algorithm.  Currently, pass `0` as the `k` element.
TODO: make a wrapper function for `dl_algoX` with only one argument.

Although `struct dlinks` contains `int * sols`, this feature is not implemented, and all you'll get is a
count of the number of solutions in `sols_len`.

TODO: replace `sols` with some type of callback function to allow for arbitrary behavior when detecting
an exact cover. Additionally, add some type of limit behavior to `dl_algoX` so that the caller can
determine the number of solutions to find (often, only one is sufficient).

