#include <stdlib.h>
#include <cairo/cairo.h>
#include "lib/coupled.h"

#define N 3
#define NSTEPS 300
#define STEP 0.1

static const double INITIAL[] = {1.0, 0.0,  0.0, 0.0, 0.0, 0.0};

//coupling matrix. a adjacency matrix. the values are not k's, derived from them!
static const double km[] = {
        -2.0, 1.0, 0.0,
        1.0, -2.0, 1.0,
        0.0, 1.0, -2.0,
};

void timeseries_2_png(double *h, char *filename) {

    int YSPACING = 60;

    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, NSTEPS, YSPACING*(N+1) );
    cr = cairo_create(surface);

    cairo_set_line_width(cr, 2.0);

    cairo_set_source_rgb (cr, 0.1, 0.1, 0.2);
    cairo_paint(cr);

    int n, v, t;
    double y, dy;
    for (n = 0; n < N; ++n) {
        dy = (n + 1) * YSPACING;
        for (v = 0; v < NVAR; ++v) {
            cairo_move_to(cr, 0, dy);

            if (v%2 == 0) cairo_set_source_rgba(cr, 0.1, 0.5, 0.8, 0.8);
            else cairo_set_source_rgba(cr, 0.8, 0.1, 0.5, 0.2);

            for (t = 0; t < NSTEPS; ++t) {
                y = h[t * N * NVAR + n * NVAR + v];
                cairo_line_to(cr, t, dy + 0.8 * YSPACING * y);
            }

            cairo_stroke(cr);
        }
    }

    cairo_surface_write_to_png(surface, filename);

    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

int main(int argc, char ** argv){
    struct Sim *s = sim_init(N, km, STEP);
    double *h = sim_alloc_hist(s, NSTEPS);

    sim_run(s, h, INITIAL, NSTEPS);

    timeseries_2_png(h, "/home/mihai/learnc/concrete_osc/image-a.png");
    
    //continue sim
    sim_run(s, h, h + N * NVAR * (NSTEPS - 1), NSTEPS);

    timeseries_2_png(h, "/home/mihai/learnc/concrete_osc/image-b.png");

    free(h);
    sim_free(s);
    return 0;
}