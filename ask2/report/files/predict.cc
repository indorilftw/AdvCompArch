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
#include "btfnt.h"
#include "ghistory.h"
#include "gshare_predictor.h"
#include "hybrid.h"
#include "lhistory.h"
#include "nbit_predictor.h"		//the .h files of the branch predictors' implementations
#include "ntaken.h"
#include "predictor.h"
#include "trace.h"


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

	branch_predictor **p = new branch_predictor*[11];

    p[0] = new ntaken_predictor();
    p[1] = new btfnt_predictor();
    p[2] = new nbit_predictor(4);
    p[3] = new gshare_predictor();

    /* Local History */
        /* X = 2048 */
    p[4] = new lhistory_predictor(2048, 8);
        /* X = 4096 */
    p[5] = new lhistory_predictor(4096, 4);

    /* Global History */
        /* X=2 BHR=4 */
    p[6] = new ghistory_predictor(16384, 2, 4);
        /* X=2 BHR=8 */
    p[7] = new ghistory_predictor(16384, 2, 8);
        /* X=4 BHR=4 */
    p[8] = new ghistory_predictor(8192, 4, 4);
        /* X=4 BHR=8 */
    p[9] = new ghistory_predictor(8192, 4, 8);

    /* Tournament Hybrid */
    p[10] = new hybrid_predictor(512);


	// some statistics to keep, currently just for conditional branches

	long long int
		tmiss[11],	// number of target mispredictions
		dmiss[11]; 	// number of direction mispredictions

	for(int i = 0; i < 11; i++)
		dmiss[i] = tmiss[i] = 0;


	// keep looping until end of file

	for (;;)
    {

		// get a trace

		trace *t = read_trace ();

		// NULL means end of file

		if (!t) break;

		// send this trace to the competitor's code for prediction

		branch_update *u;

        /* not taken */
        u = p[0]->predict(t->bi);
        p[0]->update(u, t->taken, t->target);
        dmiss[0] += u->direction_prediction() != t->taken;
        tmiss[0] += u->target_prediction() != t->target;

		/* b-taken, f-not taken */
        ((btfnt_predictor *)p[1])->set_target(t->target > t->bi.address);
        u = p[1]->predict(t->bi);
        p[1]->update(u, t->taken, t->target);
        dmiss[1] += u->direction_prediction() != t->taken;
        tmiss[1] += u->target_prediction() != t->target;

		/* 4 bit */
        u = p[2]->predict(t->bi);
        p[2]->update(u, t->taken, t->target);
        dmiss[2] += u->direction_prediction() != t->taken;
        tmiss[2] += u->target_prediction() != t->target;

		/* gshare */
        u = p[3]->predict(t->bi);
        p[3]->update(u, t->taken, t->target);
        dmiss[3] += u->direction_prediction() != t->taken;
        tmiss[3] += u->target_prediction() != t->target;

		/* local history, X=2048 */
        u = p[4]->predict(t->bi);
        p[4]->update(u, t->taken, t->target);
        dmiss[4] += u->direction_prediction() != t->taken;
        tmiss[4] += u->target_prediction() != t->target;

		/* local history, X=4096 */
        u = p[5]->predict(t->bi);
        p[5]->update(u, t->taken, t->target);
        dmiss[5] += u->direction_prediction() != t->taken;
        tmiss[5] += u->target_prediction() != t->target;

		/* global history, X=2, BHR=4 */
        u = p[6]->predict(t->bi);
        p[6]->update(u, t->taken, t->target);
        dmiss[6] += u->direction_prediction() != t->taken;
        tmiss[6] += u->target_prediction() != t->target;

		/* global history, X=2, BHR=8 */
        u = p[7]->predict(t->bi);
        p[7]->update(u, t->taken, t->target);
        dmiss[7] += u->direction_prediction() != t->taken;
        tmiss[7] += u->target_prediction() != t->target;

		/* global history, X=4, BHR=4 */
        u = p[8]->predict(t->bi);
        p[8]->update(u, t->taken, t->target);
        dmiss[8] += u->direction_prediction() != t->taken;
        tmiss[8] += u->target_prediction() != t->target;

		/* global history X=4, BHR=8 */
        u = p[9]->predict(t->bi);
        p[9]->update(u, t->taken, t->target);
        dmiss[9] += u->direction_prediction() != t->taken;
        tmiss[9] += u->target_prediction() != t->target;

        /* Tournament hybrid! */
        u = p[10]->predict(t->bi);
        p[10]->update(u, t->taken, t->target);
        dmiss[10] += u->direction_prediction() != t->taken;
        tmiss[10] += u->target_prediction() != t->target;
    }


	// done reading traces

	end_trace ();

	// give final mispredictions per kilo-instruction and exit.
	// each trace represents exactly 100 million instructions.

    for(int i = 0; i < 11; ++i)
    {
        printf("%d\t%0.3f\n", i + 1, 1000.0 * (dmiss[i] / 1e8));
        delete p[i];
    }

	delete [] p;

	exit (0);
}
