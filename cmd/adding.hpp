#ifndef ADDING_HPP_INCLUDED
#define ADDING_HPP_INCLUDED


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))
template <class REAL>
void
PMF<REAL> :: AddBirthPoint (REAL xx, REAL yy, REAL alpha = 0.0)
{
    using Geometry::RadiansToDegree;

    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>();
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>();

    REAL sinL = sin(alpha);
    REAL cosL = cos(alpha);

    PMFLog("[ ADD ] : point at (%.5lf, %.5lf) in directions at angle %.3lf (%.3lf)", xx, yy, alpha, RadiansToDegree(alpha));

    cf->SetPointsIDs ();
    long oldSize = GetCount() + 1;
    long ptId = oldSize + 1;

    REAL rotxx = X_ROTATED (xx, yy, sinL, cosL);
    REAL rotyy = Y_ROTATED (xx, yy, sinL, cosL);

    PrepareTheEvolution (sinL, cosL, evts, line, rotxx);


    EvolveTheRestOfField ();

    delete line;
    delete evts;
    return;
}
#undef X_ROTATED
#undef Y_ROTATED


#endif // ADDING_HPP_INCLUDED
