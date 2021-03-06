#ifndef REMOVE_HPP_INCLUDED
#define REMOVE_HPP_INCLUDED


template <class REAL>
void
PMF<REAL> :: RemoveBirthPoint (long number, REAL sinL, REAL cosL)
{
    using Geometry::RadiansToDegree;

    long count = GetCount();

    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>(nparams.GetAxisEpsilon());
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>(nparams);

    /* ************************************************************************************** */
    PMFLog("[ REM ] : point in directions at angle %.3lf (%.1lf)", acos(cosL), RadiansToDegree(acos(cosL)));
    out << "[ REM ]" << endl;

    Point<REAL> * pt;
    if (! cf->IsEmpty())
    {
        PointPriorityQueue ppq( cf->begin(), cf->end(), PointComparator<REAL>(nparams.GetAxisEpsilon()) );
        SegmentsMap       smap( (SegmentMapComparator()) );

        //*
        PointPriorityQueue ppq2( cf->begin(), cf->end(), PointComparator<REAL>(nparams.GetAxisEpsilon()) );
        while (! ppq2.empty())
        {
            pt = ppq2.top();
            ppq2.pop();

            out << " ___:> " << pt << endl;
        }
        // */

        cf->ClearPointsContainer();
        long counter = 0;
        while (! ppq.empty())
        {
            pt = ppq.top();
            ppq.pop();

            if (pt->type == PT_BirthInField  ||  pt->type == PT_BirthOnBorder)
            {
                if (number == counter)  break;
                ++counter;
            }
            cf->PushBack(pt);
        }

        if (! ppq.empty())
        {
            cf->PrintConfiguration(out);
            REAL epsilon = nparams.GetAxisEpsilon();
            while (! cf->IsEmpty()  &&  Geometry::IsZero( cf->Back()->x - pt->x, epsilon ))
            {
                ppq.push( cf->Back() );
                cf->PopBack();
            }

            smap[ make_pair(pt->id, pt->n1->id) ] = new Segment<REAL> (pt, pt->n1, nparams);
            if (pt->type == PT_BirthInField)
                smap[ make_pair(pt->id, pt->n2->id) ] = new Segment<REAL> (pt, pt->n2, nparams);
            //*
            out << "[ CONFIGURATION before PrepareTheEvolution ] : " << endl;
            cf->PrintConfiguration(out);
            out << "[ SMAP before PrepareTheEvolution ] : " << endl;
            FOREACH(it, smap)  out << "  { " << it->ST.ST << ", " << it->ST.ND << " } : " << it->ND << endl;;
            // */
            out << "... removing point : " << pt << endl;
            cf->PrintConfiguration (out);
            PrepareTheEvolution (evts, line, ppq, smap, pt->x);
        }
    }
    /* ************************************************************************************** */
    PMFLog("[ REM ] : point  ( %.2lf , %.2lf )", pt->org_x, pt->org_y);
    out << cf << endl;

    //assert (evts->SeeFirst()->GetPoint() == pt);
    //evts->Erase ( evts->SeeFirst() );
    //*
    Point<REAL> * n1 = pt->n1;
    int i1 = n1->WhichNeighbourHasID(pt->id);
    if (i1 == 1) n1->n1 = NULL; else if (i1 == 2) n1->n2 = NULL; else assert("WRONG POINTER 1 in REM operation" && false);

    if (pt->n2)
    {
        Point<REAL> * n2 = pt->n2;
        int i2 = n2->WhichNeighbourHasID(pt->id);
        if (i2 == 1) n2->n1 = NULL; else if (i2 == 2) n2->n2 = NULL; else assert("WRONG POINTER 2 in REM operation" && false);
    }
    // */
    delete pt;

    /* ************************************************************************************** */
    // and the riot starts again ...
    EvolveTheRestOfField (sinL, cosL, evts, line, count);

    delete line;
    delete evts;
    return;
}


template <class REAL>
void
PMF<REAL> :: RemoveBirthPoint (long number, REAL alpha = 0.0)
{
    REAL  sinL = sin(alpha);
    REAL  cosL = cos(alpha);
    RotatePoints2 (sinL, cosL);
    RemoveBirthPoint (number, sinL, cosL);
    RotatePoints2 (0., 1.);
    return;
}


#endif // REMOVE_HPP_INCLUDED
