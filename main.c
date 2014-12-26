#include <stdio.h>
#include <stdlib.h>
#include "dyn.h"
#include "coupled.h"

void to_binary(struct Hist hist){
    FILE *f = fopen("hist.bin", "w");
    fwrite(hist.h, sizeof(struct State), hist.len, f);
    fclose(f);
}

void to_csv(struct Hist hist, double *ptime) {
    FILE *f = fopen("hist.csv", "w");
    struct State s;
    int i;
    for (i = 0; i < hist.len; ++i) {
        s = hist.h[i];
        fprintf(f, "%f, %f, %f\n", ptime[i], s.x, s.vx);
    }
    fclose(f);
}

void coupl_test(){
    double *h = sim_coupled();

    FILE *f = fopen("hist.csv", "w");
    int i;
    for (i = 0; i < NSTEPS * N * NVAR; i+=2) {
        if((i/2)%N == 0){
            fprintf(f, "\n");
        }
        fprintf(f, "%f, ", h[i]);
    }
    fclose(f);

    free(h);
}

void dyn_test(){
    struct State st = {.x=0, .vx=1};
    struct Hist hist = sim(st, 10, 0.04);
    double * time = timeline(10, 0.04);
    to_binary(hist);
    to_csv(hist, time);
    del_hist(&hist);
    free(time);
}

int main(int argc, char *argv[]){
    coupl_test();
    return 0;
}

