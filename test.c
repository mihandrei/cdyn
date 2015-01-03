#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lib/coupled.h"
#include "lib/arr.h"

#define N 200
#define NSTEPS 4*1024
#define STEP 0.1

void test_sim(){
    double *km = build_linear_coupled_array(N);
    double *initial = calloc(N * NVAR, sizeof(double));
    initial[0] = 1.0;

    struct Sim *s = sim_init(N, km, STEP);
    double *h = sim_alloc_hist(s, NSTEPS);

    sim_run(s, h, initial, NSTEPS);

    //continue sim
    sim_run(s, h, h + N * NVAR * (NSTEPS - 1), NSTEPS);

    free(h);
    sim_free(s);
    free(km);
    free(initial);
}

void test_sparse(){
    double arr[] = {
            10.0, 0.0, 0.0, 0.0,
             3.0, 9.0, 0.0, 0.0,
             0.0, 7.0, 0.0, 7.0,
    };
    double v[] = {2, 3, 4, 1};
    double out[] = {0, 0, 0, 0};
    double expected[] = {20, 33, 28};

    struct CSR *m = CSR_from_dense(arr, 3, 4);
    CSR_mul(m, v, out);

    assert(memcmp(out, expected, 3) == 0);

}

int main(int argc, char ** argv){
    test_sparse();
    return 0;
}