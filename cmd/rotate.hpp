#ifndef ROTATE_HPP_INCLUDED
#define ROTATE_HPP_INCLUDED


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))
template <class REAL>
void
PMF<REAL> :: RotatePoints (REAL sinL = 0.0, REAL cosL = 1.0)
{
    PMFLog("Rotating configuration with  sinL = %f  and  cosL = %f", sinL, cosL);

    FOREACH(it, *cf)
    {
        it->rotx = X_ROTATED(it->x, it->y, sinL, cosL);
        it->roty = Y_ROTATED(it->x, it->y, sinL, cosL);
    }

    /* Changing types of points */
    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        Point<REAL> * n1 = pt->n1;
        Point<REAL> * n2 = pt->n2;

        if (n1 && n2)
        {
            if (pt->rotx < n1->rotx)
            {
                if (pt->rotx < n2->rotx)  { pt->type = PT_BirthInField; }
                else {
                    pt->type = PT_Update;
                    std::swap(pt->n1, pt->n2);
                    std::swap(pt->l1, pt->l2);
                }
            }
            else {
                if (pt->rotx < n2->rotx) { pt->type = PT_Update; }
                else {
                    pt->type = PT_Collision;
                }
            }
        }
        else if (n1)
        {
            assert(n2 == NULL);
            if (pt->rotx < n1->rotx)
                pt->type = PT_BirthOnBorder;
            else
                pt->type = PT_DeathOnBorder;
        }
        else
            assert(n1 && n2 && false);
    }

    return;
}
#undef X_ROTATED
#undef Y_ROTATED


#endif // ROTATE_HPP_INCLUDED
