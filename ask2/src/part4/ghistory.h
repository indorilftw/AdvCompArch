/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
 * File Name : ghistory.h
 * Purpose : 2nd Assignment in AdvCompArch
 * Creation Date : 27-05-2012
 * Last Modified : Sun 27 May 2012 22:57:52 EEST
 * Created By : Vasilis Gerakaris <vgerak@gmail.com>
 _._._._._._._._._._._._._._._._._._._._._.*/
#ifndef GHISTORY_H
#define GHISTORY_H

#include <math.h>
#include <string.h>
#include "predictor.h"

class ghistory_update : public branch_update
{
public:
    unsigned int index;
};

class ghistory_predictor: public branch_predictor
{
    ghistory_update u;
    branch_info brInf;

	int p_limit;
	int b_limit;

	int p_entries;
	int p_nbit;

	int bhr;
	int b_length;

    unsigned char **pht;

	unsigned int pht_mask;

public:
    ghistory_predictor (int x, int y, int z)
    {
        bhr=0;

        pht = new unsigned char *[1<<b_length];

        for (int i = 0; i < (1<<b_length); ++i)
            pht[i] = new unsigned char [p_entries>>b_length];
        for (int i = 0; i < (1<<b_length); ++i)
            memset(pht[i], 0, sizeof (pht[i]));

        p_limit = (1<<p_nbit);
        b_limit = (1<<b_length);
        pht_mask = ((p_entries>>b_length)-1);

    }

    branch_update *predict (branch_info & b)
    {
        brInf = b;
        if (b.br_flags & BR_CONDITIONAL)
        {
            u.index = (b.address & pht_mask);
            u.direction_prediction(pht[bhr][u.index]>>(p_nbit-1));
        }
        else
            u.direction_prediction (true);
        u.target_prediction (0);
        return &u;
    }



    void update (branch_update *u, bool taken, unsigned int target)
    {
        if (brInf.br_flags & BR_CONDITIONAL) {

            unsigned char *c = &pht[bhr][((ghistory_update*)u)->index];

            if (taken) {
                if (*c < p_limit)
                    (*c)++;
            }
            else {
                if (*c > 0)
                    (*c)--;
            }
            bhr <<= 1;
            bhr |= taken;
            bhr &= b_limit;

        }
    }

};
#endif
