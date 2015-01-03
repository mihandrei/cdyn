#include <stdlib.h>

// a multidimensional array
struct Ar{
    double *data;
    size_t ndim;
    size_t len;
    size_t *shape;
};

struct Ar* new_arr(size_t ndim, const size_t shape[]);
void free_ar(struct Ar *ar);
//double get_ar(struct Ar const *ar, ...);

// sparse matrix csr
struct CSR{
    double *nz;
    int *col_idx;
    int *row_ptr;
    int nrows;
    int ncols;
};

struct CSR* CSR_alloc(int nrows, int ncols, int nnz);
void CSR_free(struct CSR* csr);
struct CSR*CSR_from_dense(const double *arr, int nrows, int ncols);
void CSR_mul(struct CSR *m, const double *v, double *out);