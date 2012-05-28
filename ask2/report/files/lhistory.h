/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
 * File Name : lhistory.h
 * Purpose : 2nd Assignment in AdvCompArch
 * Creation Date : 27-05-2012
 * Last Modified : Sun 27 May 2012 22:55:19 EEST
 * Created By : Vasilis Gerakaris <vgerak@gmail.com>
 _._._._._._._._._._._._._._._._._._._._._.*/

#include <math.h>
#include <string.h>
#include "predictor.h"

class lhistory_update : public branch_update
{
public:
    unsigned int pindex;
    unsigned int bindex;
};

class lhistory_predictor: public branch_predictor
{
    lhistory_update u;
    branch_info brInf;

	int p_limit;
	int b_limit;

	int p_entries;
	int p_nbit;

	int b_entries;
	int b_length;

    unsigned char *pht;
    unsigned char *bht;

	unsigned int pht_mask;

public:
    lhistory_predictor (int x,int z)
    {
        p_entries=8192;
        p_nbit=2;

        pht = new unsigned char [p_entries];
        memset (pht, 0, sizeof (pht));

        bht = new unsigned char [x];
        memset (bht, 0, sizeof (bht));

        pht_mask = ((1<<(((int) log2(p_entries))-b_length))-1);

        p_limit = (1<<p_nbit);
        b_limit = (1<<b_length);
    }

    branch_update *predict (branch_info & b)
    {
        brInf = b;
        if (b.br_flags & BR_CONDITIONAL)
        {

            u.bindex = (b.address & (b_entries-1));
            u.pindex = ((b.address & pht_mask)<<b_length);
            u.pindex |= bht[u.bindex];
            u.direction_prediction(pht[u.pindex]>>(p_nbit-1));
        }
        else
            u.direction_prediction(true);
        u.target_prediction (0);
        return &u;
    }


    void update (branch_update *u, bool taken, unsigned int target)
    {
        if (brInf.br_flags & BR_CONDITIONAL)
        {
            unsigned char *c = &pht[((lhistory_update*)u)->pindex];
            unsigned char *d = &bht[((lhistory_update*)u)->bindex];

            if (taken)
            {
                if (*c < p_limit)
                    (*c)++;
            }
            else
                if (*c > 0)
                    (*c)--;
            (*d) <<= 1;
            (*d) |= taken;
            (*d) &= b_limit;
        }
    }
};
