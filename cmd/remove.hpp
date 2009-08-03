#ifndef REMOVE_HPP_INCLUDED
#define REMOVE_HPP_INCLUDED


template <class REAL>
void
PMF<REAL> :: RemoveBirthPoint (long number, REAL alpha = 0.0)
{
    using Geometry::RadiansToDegree;

    REAL  sinL = sin(alpha);
    REAL  cosL = cos(alpha);
    long count = GetCount();

    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>();
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>();

    /* ************************************************************************************** */
    PMFLog("[ REM ] : point in directions at angle %.3lf (%.1lf)", alpha, RadiansToDegree(alpha));

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


#endif // REMOVE_HPP_INCLUDED
