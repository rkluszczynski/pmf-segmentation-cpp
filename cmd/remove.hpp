#ifndef REMOVE_HPP_INCLUDED
#define REMOVE_HPP_INCLUDED


template <class REAL>
void
PMF<REAL> :: RemoveBirthPoint (long number, REAL alpha = 0.0)
{
    using Geometry::RadiansToDegree;

    REAL sinL = sin(alpha);
    REAL cosL = cos(alpha);

    cf->SetPointsIDs ();
    long count = GetCount();

    cf->PrintConfiguration(out);
    RotatePoints2 (sinL, cosL);
    out << "[ ROTATED ]" << endl;  FOREACH(it, *cf) out << (*it) << endl;
    //SavePMF("../output/geo-rot.zip", GeoGebraFile);


    /* ************************************************************************************** */
    PMFLog("[ REM ] : point in directions at angle %.3lf (%.1lf)", alpha, RadiansToDegree(alpha));

    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>();
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>();
    Point<REAL> * pt;
    if (! cf->IsEmpty())
    {
        PointPriorityQueue ppq( cf->begin(), cf->end(), PointComparator<REAL>() );
        cf->ClearPointsContainer();
        long counter = 0;
        while (! ppq.empty())
        {
            pt = ppq.top();

            if (pt->type == PT_BirthInField  ||  pt->type == PT_BirthOnBorder)
            {
                if (number == counter)  break;
                ++counter;
            }

            ppq.pop();
            cf->PushBack(pt);
        }

        if (! ppq.empty())
        {
            out << "... removing point : " << pt << endl;
            PrepareTheEvolution (evts, line, ppq, pt->x);
        }
    }
    /* ************************************************************************************** */

    assert (evts->SeeFirst()->GetPoint() == pt);
    evts->Erase ( evts->SeeFirst() );
    delete pt;

    /* ************************************************************************************** */
    // and the riot starts again ...
    EvolveTheRestOfField (sinL, cosL, evts, line, count);

    delete line;
    delete evts;
    return;
}


#endif // REMOVE_HPP_INCLUDED
