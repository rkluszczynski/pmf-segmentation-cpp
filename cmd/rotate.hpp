#ifndef ROTATE_HPP_INCLUDED
#define ROTATE_HPP_INCLUDED


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))
template <class REAL>
void
PMF<REAL> :: RotatePoints (REAL sinL = 0.0, REAL cosL = 1.0)
{
    using Geometry::IsZero;
    typedef enum { BirthEnd, DeathEnd } EndType;

    PMFLog("Rotating configuration with  sinL = %f  and  cosL = %f", sinL, cosL);

    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        REAL xx = pt->x;
        REAL yy = pt->y;
        pt->StoreCoordinates();
        pt->x = X_ROTATED(xx, yy, sinL, cosL);
        pt->y = Y_ROTATED(xx, yy, sinL, cosL);
    }

    std::vector<bool> notChanged(GetCount() + 1);
    std::fill_n(notChanged.begin(), GetCount() + 1, true);
    /* Changing types of points */
    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        Point<REAL> * n1 = pt->n1;
        Point<REAL> * n2 = pt->n2;

        if (n1 && n2)
        {
            if (pt->x < n1->x)
            {
                if (pt->x < n2->x)  { pt->type = PT_BirthInField; }
                else {
                    pt->type = PT_Update;
                    std::swap(pt->n1, pt->n2);
                    std::swap(pt->l1, pt->l2);
                }
            }
            /*
            else if (IsZero(pt->x - n1->x)
            {
                EndType end1;// = DetermineEnd(1);
                EndType end2;// = DetermineEnd(2);

            }
            //*/
            else {
                if (pt->x > n2->x) { pt->type = PT_Collision; }
                else {
                    pt->type = PT_Update;
                }
            }
        }
        else if (n1)
        {
            assert(n2 == NULL);
            if (pt->x < n1->x)
                pt->type = PT_BirthOnBorder;
            else
                pt->type = PT_DeathOnBorder;
        }
        else
            assert(n1 && n2 && false);

        notChanged[pt->id] = false;
    }
    return;
}
#undef X_ROTATED
#undef Y_ROTATED


#endif // ROTATE_HPP_INCLUDED
