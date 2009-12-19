#ifndef ROTATE_HPP_INCLUDED
#define ROTATE_HPP_INCLUDED


template <class REAL>
EndType
PMF<REAL> :: DetermineN (Point<REAL> * pt, int which, std::vector<bool> & changed)
{
    using Geometry::IsZero;
    while (true)
    {
        Point<REAL> * tmp = pt->n(which);

        if (changed[tmp->id])
        {
            switch (tmp->type)
            {
                case PT_BirthOnBorder :
                case PT_BirthInField  :
                                    return BirthEnd;
                case PT_Collision     :
                case PT_DeathOnBorder :
                                    return DeathEnd;
                default :
                                    ;
            }
        }
        if (! IsZero(tmp->x - pt->x))
        {
            if (tmp->x < pt->x) return BirthEnd;
            else return DeathEnd;
        }
        else if (tmp->n2 == NULL)
        {
            if (tmp->x < tmp->n1->x)
                return BirthEnd;
            else
                return DeathEnd;
        }
        int wh = tmp->WhichNeighbourHasID(pt->id);
        assert(wh > 0);
        pt = tmp;
        which = 3 - wh;
    }
}


template <class REAL>
void
PMF<REAL> :: RotatePoints2 (REAL alpha = 0.0)
{
    PMFLog("Rotating configuration with  alpha = %.3f  (%.1f)", alpha, Geometry::RadiansToDegree(alpha));
    REAL sinL = sin(alpha);
    REAL cosL = cos(alpha);
    RotatePoints2 ( sinL, cosL );
}


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))
template <class REAL>
void
PMF<REAL> :: RotatePoints2 (REAL sinL, REAL cosL)
{
    using Geometry::IsZero;
    PMFLog("Rotating configuration with  sinL = %f  and  cosL = %f", sinL, cosL);
    //cf->PrintConfiguration(out);

    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        REAL xx = pt->x;
        REAL yy = pt->y;
        pt->StoreCoordinates();
        pt->x = X_ROTATED(xx, yy, sinL, cosL);
        pt->y = Y_ROTATED(xx, yy, sinL, cosL);
    }

    std::vector<bool> changed(GetCount() + 1);
    std::fill_n(changed.begin(), GetCount() + 1, false);
    /* Changing types of points */
    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        Point<REAL> * n1 = pt->n1;
        Point<REAL> * n2 = pt->n2;

        if (n1 && n2)
        {
            EndType end1 = DetermineN (pt, 1, changed);
            EndType end2 = DetermineN (pt, 2, changed);

            if (end1 == BirthEnd)
            {
                if (end2 == BirthEnd)  pt->type = PT_Collision;
                else  pt->type = PT_Update;
            }
            else {
                if (end2 == BirthEnd)  pt->type = PT_Update;
                else  pt->type = PT_BirthInField;
            }

            if (pt->type == PT_Update  &&  pt->n1->x > pt->n2->x)
            {
                std::swap(pt->n1, pt->n2);
                std::swap(pt->l1, pt->l2);
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

        changed[pt->id] = true;
    }
    out << " GetCount() == " << GetCount() << endl;

    // Reassigning IDs
    FOREACH(it, *cf) (*it)->id = 0;
    long count = 0;
    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        out << " --> " << pt << endl;
        if (pt->type == PT_Update) continue;

        switch (pt->type)
        {
            case PT_BirthInField  :
                                pt->id = ++count;
                                for(Point<REAL> * tmp = pt->n1; tmp->type == PT_Update; tmp = tmp->n2)
                                {
                                    out << "  _> " << tmp << endl;
                                    tmp->id = ++count;
                                    out << "  1> " << tmp << endl;
                                }
                                for(Point<REAL> * tmp = pt->n2; tmp->type == PT_Update; tmp = tmp->n2)
                                {
                                    out << "  _> " << tmp << endl;
                                    tmp->id = ++count;
                                    out << "  2> " << tmp << endl;
                                }
                                break;;
            case PT_BirthOnBorder :
                                pt->id = ++count;
                                for(Point<REAL> * tmp = pt->n1; tmp->type == PT_Update; tmp = tmp->n2)
                                {
                                    out << "  _> " << tmp << endl;
                                    tmp->id = ++count;
                                    out << "  0> " << tmp << endl;
                                }
                                break;;
            default :
                                pt->id = ++count;
        }
        out << " ^^^ " << pt << endl;
    }
    out << "    count   == " << count << endl;
    FOREACH(it, *cf)
    {
        assert( (*it)->id > 0 );
        out << "    ~ " << *it << endl;
    }
    assert(count == GetCount());
    out << "[ ROTATED ]" << endl;  FOREACH(it, *cf) out << (*it) << endl;
}


template <class REAL>
void
PMF<REAL> :: RotatePoints (REAL sinL = 0.0, REAL cosL = 1.0)
{
    using Geometry::IsZero;
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
