#ifndef UPDATE_HPP_INCLUDED
#define UPDATE_HPP_INCLUDED


template <class REAL>
void
PMF<REAL> :: UpdatePointVelocity (long id, REAL alpha = 0.0)
{
    using Geometry::RadiansToDegree;
    using Geometry::IsZero;

    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>();
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>();

    REAL sinL = sin(alpha);
    REAL cosL = cos(alpha);

    PMFLog("[ UPD ] : point in directions at angle %.3lf (%.1lf)", alpha, RadiansToDegree(alpha));

    cf->SetPointsIDs ();
    long count = GetCount();

    //PrepareTheEvolution (sinL, cosL, evts, line, 0);
    out << " PMF_ELEMENT_COUNTER  = " << pmf::pmf_element_counter << endl;
    out << " PMF_SEGMENT_COUNTER  = " << pmf::pmf_segment_counter << endl;

    out << endl << line << endl << endl;
    out << "__________ DO THE (R)EVOLUTION !!! __________" << endl;


    EvolveTheRestOfField (sinL, cosL, evts, line, count);

    delete line;
    delete evts;
    return;
}


#endif // UPDATE_HPP_INCLUDED
