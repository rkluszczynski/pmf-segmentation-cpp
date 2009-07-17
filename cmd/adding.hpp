#ifndef ADDING_HPP_INCLUDED
#define ADDING_HPP_INCLUDED


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))
template <class REAL>
void
PMF<REAL> :: AddBirthPoint (REAL xx, REAL yy, REAL alpha = 0.0)
{
    using Geometry::RadiansToDegree;
    using Geometry::IsZero;

    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>();
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>();

    REAL sinL = sin(alpha);
    REAL cosL = cos(alpha);

    PMFLog("[ ADD ] : point at (%.2lf, %.2lf) in directions at angle %.3lf (%.1lf)", xx, yy, alpha, RadiansToDegree(alpha));

    cf->SetPointsIDs ();
    long count = GetCount();

    if (IsZero(GetHeight() - yy)) yy = GetHeight() - 2. * EPSILON;
    else if (IsZero(yy)) yy = 2. * EPSILON;

    REAL rotxx = X_ROTATED (xx, yy, sinL, cosL);
    REAL rotyy = Y_ROTATED (xx, yy, sinL, cosL);

    PrepareTheEvolution (sinL, cosL, evts, line, rotxx);
    out << endl << line << endl << endl;

    Point<REAL> * newpt = new Point<REAL>(rotxx, rotyy, 0.0, 0.0, ++count, PT_BirthInField);
    newpt->org_x = xx;
    newpt->org_y = yy;
    //evts->InsertBirthEvent(newpt);
    cf->PushBack(newpt);
    /// TODO (klusi#1#): should I also add new point to set idsok ???

    Point<REAL> * newpt1 = NULL, * newpt2 = NULL;
    while (true)
    {
        REAL upperLength = Probability::Exp<REAL>(2.0);
        REAL lowerLength = Probability::Exp<REAL>(2.0);
        REAL upperAngle, lowerAngle;
        Probability::DetermineBirthAngles (upperAngle, lowerAngle);

        if (! newpt1)
        {
            newpt1 = newpt->GenerateNeighbour(1, upperAngle, count, upperLength);
            bool ans = ArrangeNewEvent(newpt1, evts, line, count, sinL, cosL);
            if (! ans)
            {
                delete newpt1;
                newpt1 = NULL;
                continue;
            }
        }
        if (! newpt2)
        {
            newpt2 = newpt->GenerateNeighbour(2, lowerAngle, count, lowerLength);
            bool ans = ArrangeNewEvent(newpt2, evts, line, count, sinL, cosL);
            if (! ans)
            {
                delete newpt2;
                newpt2 = NULL;
                continue;
            }
        }
        break;
    }


    EvolveTheRestOfField (sinL, cosL, evts, line, count);

    delete line;
    delete evts;
    return;
}
#undef X_ROTATED
#undef Y_ROTATED


#endif // ADDING_HPP_INCLUDED