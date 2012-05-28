/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
 * File Name : btfnt.h
 * Purpose : 2nd Assignment in AdvCompArch
 * Creation Date : 27-05-2012
 * Last Modified : Sun 27 May 2012 22:58:13 EEST
 * Created By : Vasilis Gerakaris <vgerak@gmail.com>
 _._._._._._._._._._._._._._._._._._._._._.*/

#include <math.h>
#include "predictor.h"

class btfnt_predictor: public branch_predictor
{
    branch_update u;
    branch_info brInf;

    bool jump;

public:
	void set_target(bool t)
    {
        jump = t;
    }

    branch_update *predict (branch_info & b)
    {
        brInf = b;
        if (b.br_flags & BR_CONDITIONAL)
        {
            if (jump)
            {
                u.direction_prediction (false);
            }
            else
            {
                u.direction_prediction(true);
            }
        }
        else
            u.direction_prediction (true);
        u.target_prediction (0);
        return &u;
    }

    void update (branch_update *u, bool taken, unsigned int target)
    {
    }
};
