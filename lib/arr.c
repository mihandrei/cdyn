#include "arr.h"

struct Ar* new_arr(size_t ndim, const size_t shape[]){
    int i;
    struct Ar* ar;
    ar = malloc(sizeof(struct Ar));
    ar->ndim = ndim;
    ar->shape = malloc(sizeof(size_t) * ndim);

    ar->len = 1;
    for (i = 0; i < ndim; ++i) {
        ar->shape[i] = shape[i];
        ar->len *= shape[i];
    }
    ar->data = malloc(sizeof(double) * ar->len);
    return ar;
}

void free_ar(struct Ar *ar){
    free(ar->data);
    free(ar->shape);
    free(ar);
}

//double get_ar(struct Ar const *ar, ...) {
//    for (int i = 0; i < ar->ndim; ++i) {
//
//    }
//    return 0;
//}