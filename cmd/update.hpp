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

            out << "  *> " << pt << endl;
            if (pt->type == PT_Update)
            {
                if (number == counter)  break;
                ++counter;
            }

            cf->PushBack(pt);
        }
        out << " ... chosen" << endl;

        if (! ppq.empty())
        {
            /// NOTE (Rafal#1#): Is it needed during update?
            /*
            // 2009-12-19 : problem with getting vertical and previous update point
            while (! cf->IsEmpty()  &&  Geometry::IsZero( cf->Back()->x - pt->x ))
            {
                ppq.push( cf->Back() );
                cf->PopBack();
            }
            // */

            smap[ make_pair(pt->id, pt->n2->id) ] = new Segment<REAL> (pt, pt->n2);
            /// FIXME (klusi#1#): possible BUG : analize when two update points are vertical
            out << endl << "... updating point : " << pt << endl;

            PrepareTheEvolution (evts, line, ppq, smap, pt->x);
            out << endl << "... evolution prepared" << endl;
        }
    }
    /* ************************************************************************************** */
    PMFLog("[ UPD ] : point  ( %.2lf , %.2lf )", pt->org_x, pt->org_y);
    out << line << endl;

    //assert (evts->SeeFirst()->GetPoint() == pt);
    //evts->Erase ( evts->SeeFirst() );
    //REAL x0 = line->GetX0();
    //if (! (pt->x >= x0 || Geometry::IsZero(pt->x - x0)))
    //if (! Geometry::IsZero(pt->x - x0))
        line->SetSweepLinePosition2(pt->x + 0.5 * EPSILON);
    out << "... sweep line position set" << endl;
    /*
    ForgetOldCollisionPoint(sinL, cosL, pt, pt->n1, evts, line, count);
    assert(pt->n2);
    if (pt->n2->type == PT_Collision)
    {
        out << " NEXT ONE is COLLISION - also fix it" << endl;
    }
    delete pt;
    // */
    while(true)
    {
        REAL newAngle = PRNG->GetUniform(EPSILON-M_PI_2, M_PI_2-EPSILON);
        out << " newAngle = " << newAngle << endl;

        REAL length = PRNG->GetExp (2.0);
        if (length < EPSILON) length = EPSILON;
        Point<REAL> * newpt = pt->GenerateNeighbour(2, newAngle, count, length);

        if (ArrangeNewEvent(newpt, evts, line, count, sinL, cosL)) break;

        delete newpt;
    }
    cf->PushBack(pt);

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
