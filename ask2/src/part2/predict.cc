// predict.cc
// This file contains the main function.  The program accepts a single
// parameter: the name of a trace file.  It drives the branch predictor
// simulation by reading the trace file and feeding the traces one at a time
// to the branch predictor.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "branch.h"
#include "trace.h"
#include "predictor.h"
#include "nbit_predictor.h"     //the .h files of the branch predictors' implementations

int main (int argc, char *argv[]) {

    // make sure there is one parameter

    if (argc != 2) {
        fprintf (stderr, "Usage: %s <filename>.gz\n", argv[0]);
        exit (1);
    }

    // open the trace file for reading

    init_trace (argv[1]);

    // initialize competitor's branch prediction code

    // you can use more than one predictor in an array of predictors!!!

    branch_predictor **p = new branch_predictor*[3];

    p[0] = new nbit_predictor(1, 15);
    p[1] = new nbit_predictor(2, 14);
    p[2] = new nbit_predictor(4, 13);

    // some statistics to keep, currently just for conditional branches

    long long int
        tmiss[3],   // number of target mispredictions
        dmiss[3];   // number of direction mispredictions

    for (int i = 0; i < 3; i++)
        dmiss[i] = tmiss[i] = 0;
    // keep looping until end of file

    for (;;) {

        // get a trace

        trace *t = read_trace ();

        // NULL means end of file

        if (!t) break;

        // send this trace to the competitor's code for prediction

        branch_update *u;

        for (int i = 0; i < 3; ++i)
        {
            u = p[i]->predict(t->bi);
            dmiss[i] += u->direction_prediction() != t->taken;
            p[i]->update(u, t->taken, t->target);
        }

    }

    // done reading traces

    end_trace ();

    // give final mispredictions per kilo-instruction and exit.
    // each trace represents exactly 100 million instructions.

    for (int i = 0; i < 3; ++i)
    {
        printf("%d-bit_pred: %0.3f MPKI\n",i + 1, 1000.0 * (dmiss[i]/1e8));
        delete p[i];
    }

    delete [] p;

    exit (0);
}
