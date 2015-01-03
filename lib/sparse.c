#include "arr.h"

struct CSR* CSR_alloc(int nrows, int ncols, int nnz){
    struct CSR *csr = malloc(sizeof(struct CSR));

    csr->nrows = nrows;
    csr->ncols = ncols;
    csr->row_ptr = malloc(sizeof(int) * (nrows + 1));
    csr->nz = malloc(sizeof(double) * nnz);
    csr->col_idx = malloc(sizeof(int) * nnz);
    return csr;
}

void CSR_free(struct CSR* csr){
    free(csr->col_idx);
    free(csr->nz);
    free(csr->row_ptr);
    free(csr);
}

static int _nnz(const double *arr, int size){
    int i, nnz = 0;
    for (i = 0; i < size; ++i) {
        if(arr[i] != 0.0){
            nnz += 1;
        }
    }
    return nnz;
}

struct CSR* CSR_from_dense(const double *arr, int nrows, int ncols){
    int i, j, nz_idx;
    double elem;
    int nnz = _nnz(arr, nrows * ncols);
    struct CSR *csr = CSR_alloc(nrows, ncols, nnz);

    nz_idx = 0;
    for (i = 0; i < nrows; ++i) {
        csr->row_ptr[i] = nz_idx;
        for (j = 0; j < ncols; ++j) {
            elem = arr[i * ncols + j];
            if(elem != 0.0){
                csr->nz[nz_idx] = elem;
                csr->col_idx[nz_idx] = j;
                nz_idx += 1;
            }
        }
    }
    csr->row_ptr[nrows] = nnz;
    return csr;
}

void CSR_mul(struct CSR *m, const double *v, double *out){
    int row, col, ri;
    double elem, x;
    for (row = 0; row < m->nrows; ++row) {
        x = 0;
        for (ri = m->row_ptr[row]; ri < m->row_ptr[row + 1]; ++ri){
            col = m->col_idx[ri];
            elem = m->nz[ri];
            x += elem * v[col];
        }
        out[row] = x;
    }
}