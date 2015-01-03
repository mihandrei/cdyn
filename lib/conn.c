#include <stdlib.h>
#include "coupled.h"
#include "arr.h"

/**
* build a coupling matrix for a linear array of n oscillators
* |---M---M--- ... ---M---|
* It is a weighted adjacency matrix. If the k's are all 1 it will lok like this
*  -2.0, 1.0, 0.0,
*   1.0, -2.0, 1.0,
*   0.0, 1.0, -2.0
*
* The matrix is conceptually sparse but we return a dense one. This will have to be changed.
*/
double * build_linear_coupled_array(int n){
    double *ret = calloc(n * n, sizeof(double));
    int i;
    for (i = 0; i < n; ++i) {
        ret[i*n + i] = -2.0;
    }
    for (i = 0; i < n - 1; ++i) {
        ret[i*n + i + 1] = 1.0;
    }
    for (i = 1; i < n; ++i) {
        ret[i*n + i - 1] = 1.0;
    }
    return ret;
}

//struct CSR * csr_build_linear_coupled_array(int n){
//    struct CSR *csr = CSR_alloc(n, n, 3*n - 2);
//    for (int i = 0; i < n; ++i) {
//        csr->nz[i] =
//    }
//    return csr;
//}