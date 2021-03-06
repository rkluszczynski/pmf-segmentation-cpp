#ifndef ADDING_HPP_INCLUDED
#define ADDING_HPP_INCLUDED


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))
template <class REAL>
bool
PMF<REAL> :: AddBirthPoint (REAL xx, REAL yy, REAL sinL, REAL cosL)
{
    using Geometry::RadiansToDegree;
    using Geometry::IsZero;

    PMFLog("[ ADD ] : point at (%.2lf, %.2lf) in directions at angle %.3lf (%.1lf)", xx, yy, acos(cosL), RadiansToDegree(acos(cosL)));

    REAL epsilon = nparams.GetAxisEpsilon();
    if (IsZero(GetHeight() - yy, epsilon)) yy = GetHeight() - 2. * epsilon;
    else if (IsZero(yy, epsilon)) yy = 2. * epsilon;

    REAL rotxx = X_ROTATED (xx, yy, sinL, cosL);
    REAL rotyy = Y_ROTATED (xx, yy, sinL, cosL);
    out << "[ ADD ]   rotated = ( " << rotxx << " , " << rotyy << " )" << endl;

    if (! IsFreeEpsilonCoincidence(rotxx, rotyy))  return false;

    /* ************************************************************************************** */
    long count = GetCount();

    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>(nparams.GetAxisEpsilon());
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>(nparams);

    if (! cf->IsEmpty())
    {
        PointPriorityQueue ppq( cf->begin(), cf->end(), PointComparator<REAL>(nparams.GetAxisEpsilon()) );
        SegmentsMap       smap( (SegmentMapComparator()) );

        cf->ClearPointsContainer();
        while (ppq.top()->x < rotxx)
        {
            Point<REAL> * pt = ppq.top();
            cf->PushBack(pt);
            ppq.pop();
            if (ppq.empty()) break; //return;
        }

        PrepareTheEvolution (evts, line, ppq, smap, rotxx);
    }
    //*
    out << " PMF_ELEMENT_COUNTER  = " << pmf::pmf_element_counter << endl;
    //out << " PMF_SEGMENT_COUNTER  = " << pmf::pmf_segment_counter << endl;

    out << endl << line << endl << endl;
    out << "__________ DO THE (R)EVOLUTION !!! __________" << endl;
    // */
    /* ************************************************************************************** */

    Point<REAL> * newpt = new Point<REAL>(rotxx, rotyy, 0.0, 0.0, ++count, PT_BirthInField);
    newpt->org_x = xx;
    newpt->org_y = yy;
    cf->PushBack(newpt);
    out << "  newpt added : " << newpt << endl;

    Point<REAL> * newpt1 = NULL, * newpt2 = NULL;
    while (true)
    {
        REAL upperLength = GetPRNG()->GetExp(2.0);
        REAL lowerLength = GetPRNG()->GetExp(2.0);
        REAL upperAngle, lowerAngle;
        GetPRNG()->DetermineBirthAngles (upperAngle, lowerAngle);

        if (! newpt1)
        {
            newpt1 = newpt->GenerateNeighbour(GetPRNG(), 1, upperAngle, count, upperLength);
            out << " trying newpt1 : " << newpt1 << endl;
            bool ans = ArrangeNewEvent(newpt1, evts, line, count, sinL, cosL);
            if (! ans)
            {
                delete newpt1;
                newpt1 = NULL;
                continue;
            }
            out << " newpt1 added : " << newpt1 << endl;
        }
        if (! newpt2)
        {
            newpt2 = newpt->GenerateNeighbour(GetPRNG(), 2, lowerAngle, count, lowerLength);
            out << " trying newpt2 : " << newpt2 << endl;
            bool ans = ArrangeNewEvent(newpt2, evts, line, count, sinL, cosL);
            if (! ans)
            {
                delete newpt2;
                newpt2 = NULL;
                continue;
            }
            out << " newpt2 added : " << newpt2 << endl;
        }
        break;
    }
    out << "_______ points added" << endl;

    /* ************************************************************************************** */
    // and the riot starts again ...
    EvolveTheRestOfField (sinL, cosL, evts, line, count);

    delete line;
    delete evts;
    return true;
}
#undef X_ROTATED
#undef Y_ROTATED


template <class REAL>
void
PMF<REAL> :: AddBirthPoint (REAL xx, REAL yy, REAL alpha = 0.0)
{
    REAL  sinL = sin(alpha);
    REAL  cosL = cos(alpha);
    RotatePoints2 (sinL, cosL);
    AddBirthPoint (xx, yy, sinL, cosL);
    RotatePoints2 (0., 1.);
    return;
}


#endif // ADDING_HPP_INCLUDED
