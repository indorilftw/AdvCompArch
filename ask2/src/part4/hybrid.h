/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
 * File Name : hybrid.h
 * Purpose : 2nd Assignment in AdvCompArch
 * Creation Date : 27-05-2012
 * Last Modified : Sun 27 May 2012 22:55:56 EEST
 * Created By : Vasilis Gerakaris <vgerak@gmail.com>
 _._._._._._._._._._._._._._._._._._._._._.*/

#include <math.h>
#include <string.h>
#include "ghistory.h"
#include "predictor.h"

class hybrid_update : public branch_update
{
public:
	ghistory_update *ups[2];
	int index;
	int pred;

};


class hybrid_predictor : public branch_predictor
{
public:

	hybrid_update u;
	branch_info brInf;
	int c_limit;
	int p_entries;
	int p_length;

	branch_predictor **preds;

	unsigned char *tab;

	hybrid_predictor(int entries)
    {
        tab = new unsigned char[p_entries];
        p_length = 2;
        c_limit = 2;

        preds = new branch_predictor*[2];
        preds[0] = new ghistory_predictor(8192, 2, 2);
        preds[1] = new ghistory_predictor(8192, 2, 4);
    }

    branch_update *predict (branch_info & b)
    {
        brInf = b;
        if (b.br_flags & BR_CONDITIONAL)
        {
            u.index =  (b.address & (p_entries-1));
            u.pred = tab[u.index] >> (p_length - 1);
        }
        u.ups[0] = (ghistory_update *) preds[0]->predict(b);
        u.ups[1] = (ghistory_update *) preds[1]->predict(b);
        u.direction_prediction(u.ups[u.pred]->direction_prediction());
        return &u;
    }

    void update (branch_update *u, bool taken, unsigned int target)
    {
        if (brInf.br_flags & BR_CONDITIONAL)
        {
            unsigned char *c = &tab[((hybrid_update*)u)->index];
            bool pred0_result = ((hybrid_update *)u)->ups[0]->direction_prediction() == taken;
            bool pred1_result = ((hybrid_update *)u)->ups[1]->direction_prediction() == taken;
            int meta_update = pred1_result - pred0_result;

            if (meta_update > 0 && *c < 3)
                (*c)++;
            else if (meta_update < 0 && *c > 0)
                (*c)--;

            preds[0]->update(((hybrid_update *)u)->ups[0], taken, target);
            preds[1]->update(((hybrid_update *)u)->ups[1], taken, target);
        }
    }

};
