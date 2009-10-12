#ifndef REMOVE_HPP_INCLUDED
#define REMOVE_HPP_INCLUDED


template <class REAL>
void
PMF<REAL> :: RemoveBirthPoint (long number, REAL sinL, REAL cosL)
{
    using Geometry::RadiansToDegree;

    long count = GetCount();

    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>();
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>();

    /* ************************************************************************************** */
    PMFLog("[ REM ] : point in directions at angle %.3lf (%.1lf)", acos(cosL), RadiansToDegree(acos(cosL)));
    out << "[ REM ]" << endl;

    Point<REAL> * pt;
    if (! cf->IsEmpty())
    {
        PointPriorityQueue ppq( cf->begin(), cf->end(), PointComparator<REAL>() );
        SegmentsMap       smap( (SegmentMapComparator()) );

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
            while (! cf->IsEmpty()  &&  Geometry::IsZero( cf->Back()->x - pt->x ))
            {
                ppq.push( cf->Back() );
                cf->PopBack();
            }

            smap[ make_pair(pt->id, pt->n1->id) ] = new Segment<REAL> (pt, pt->n1);
            if (pt->type == PT_BirthInField)
                smap[ make_pair(pt->id, pt->n2->id) ] = new Segment<REAL> (pt, pt->n2);

            out << "... removing point : " << pt << endl;
            PrepareTheEvolution (evts, line, ppq, smap, pt->x);
        }
    }
    /* ************************************************************************************** */
    PMFLog("[ REM ] : point  ( %.2lf , %.2lf )", pt->org_x, pt->org_y);

    //assert (evts->SeeFirst()->GetPoint() == pt);
    //evts->Erase ( evts->SeeFirst() );
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
