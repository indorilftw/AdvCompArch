/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
 * File Name : ntaken.h
 * Purpose : 2nd Assignment in AdvCompArch
 * Creation Date : 27-05-2012
 * Last Modified : Sun 27 May 2012 18:52:27 EEST
 * Created By : Vasilis Gerakaris <vgerak@gmail.com>
 _._._._._._._._._._._._._._._._._._._._._.*/

#include "predictor.h"

class ntaken_update : public branch_update
{
public:
    unsigned int index;
};

class ntaken_predictor: public branch_predictor
{
    ntaken_update u;

    branch_update *predict (branch_info & b)
    {
        if (b.br_flags & BR_CONDITIONAL)
            u.direction_prediction (false);
        else
            u.direction_prediction (true);
        u.target_prediction (0);
        return &u;
    }
    void update (branch_update *u, bool taken, unsigned int target)
    {
    }
};
