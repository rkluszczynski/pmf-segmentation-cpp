#ifndef UPDATE_HPP_INCLUDED
#define UPDATE_HPP_INCLUDED


template <class REAL>
void
PMF<REAL> :: UpdatePointVelocity (long number, REAL sinL, REAL cosL)
{
    using Geometry::RadiansToDegree;

    long count = GetCount();

    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>();
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>();

    /* ************************************************************************************** */
    PMFLog("[ UPD ] : point in directions at angle %.3lf (%.1lf)", acos(cosL), RadiansToDegree(acos(cosL)));
    out << "[ UPD ]" << endl;

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

            if (pt->type == PT_Update)
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

            smap[ make_pair(pt->id, pt->n2->id) ] = new Segment<REAL> (pt, pt->n2);
            /// FIXME (klusi#1#): possible BUG : analize when two update points are vertical
            out << "... updating point : " << pt << endl;
            PrepareTheEvolution (evts, line, ppq, smap, pt->x);
        }
    }
    /* ************************************************************************************** */

    //assert (evts->SeeFirst()->GetPoint() == pt);
    //evts->Erase ( evts->SeeFirst() );
    ForgetOldCollisionPoint(sinL, cosL, pt, pt->n1, evts, line, count);
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
PMF<REAL> :: UpdatePointVelocity (long number, REAL alpha = 0.0)
{
    REAL  sinL = sin(alpha);
    REAL  cosL = cos(alpha);
    RotatePoints2 (sinL, cosL);
    UpdatePointVelocity (number, sinL, cosL);
    RotatePoints2 (0., 1.);
    return;
}


#endif // UPDATE_HPP_INCLUDED
