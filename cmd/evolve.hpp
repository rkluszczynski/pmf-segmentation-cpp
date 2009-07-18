#ifndef EVOLVE_HPP_INCLUDED
#define EVOLVE_HPP_INCLUDED


template <class REAL>
inline
void
PMF<REAL> :: ProcessOldEvent (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id, REAL sinL, REAL cosL)
{
    typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;

    Point<REAL>   * pt = ev->GetPoint();
    Segment<REAL> * s1 = ev->GetSegment();
    Segment<REAL> * s2 = ev->GetSegment(false);
    assert(s1);

    switch (pt->type)
    {
        case  PT_BirthInField :
                            assert(s2);
                            //line->Insert( pt, s2 );
                            InsertNewSegmentIntoSweep (pt, s2, evts, line, id, sinL, cosL);
        case PT_BirthOnBorder :
                            //line->Insert( pt, s1 );
                            InsertNewSegmentIntoSweep (pt, s1, evts, line, id, sinL, cosL);
                            break;;
        case     PT_Collision :
                            assert(s2);
                            CheckIntersectionAfterDeath (s1, s2, evts, line, id, sinL, cosL);
                            line->Erase( s1 );
                            line->Erase( s2 );
                            delete s1;
                            delete s2;
                            break;;
        case PT_DeathOnBorder :
                            CheckIntersectionAfterDeath (s1, NULL, evts, line, id, sinL, cosL);
                            line->Erase( s1 );
                            delete s1;
                            break;;
        case        PT_Update :
                            //CheckIntersectionAfterDeath (s1, NULL, evts, line, id, sinL, cosL);
                            /// FIXME (Rafel#4#): do not know if line above should exist ??? It is checked during insertion
                            line->Erase( s1 );
                            delete s1;
                            //line->Insert( pt, s2 );
                            assert(s2);
                            InsertNewSegmentIntoSweep (pt, s2, evts, line, id, sinL, cosL);
                            break;;
        default:
            assert("WRONG POINT TYPE" && false);
    }
    return;
}


template <class REAL>
inline
void
PMF<REAL> :: ForgetOldCollisionPoint (REAL sinL, REAL cosL, Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, /*set<Point<REAL> *> & idsok,*/ long & id)
{
    typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;
    typedef typename set<Point<REAL> *>::iterator  OkPointsSetIterator;

    Point<REAL> * dpt = ev->GetPoint();
    //OkPointsSetIterator it1 = idsok.find(dpt->n1);
    //OkPointsSetIterator it2 = idsok.find(dpt->n2);

    SweepIterator it1 = line->Find( ev->GetSegment(true) );
    SweepIterator it2 = line->Find( ev->GetSegment(false) );

    Point<REAL> * dptn;
    if (line->IsNull(it1))
    {
        assert(! line->IsNull(it2));
        dptn = (*it2)->GetSegment()->GetP();
        line->Erase( it2 );
    }
    else if (line->IsNull(it2))
    {
        assert(! line->IsNull(it1));
        dptn = (*it1)->GetSegment()->GetP();
        line->Erase( it1 );
    }
    else
        assert("WRONG OLD COLLISION POINT" && false);

    int wh = dptn->WhichNeighbourHasID( dpt->id );
    assert(wh > 0);
    REAL length = (wh == 1) ? dptn->l1 : dptn->l2;
    /*
    if (it1 != idsok.end() and (dptn = *it1)->WhichNeighbourHasID(dpt->id) > 0)
    {
        assert(it2 == idsok.end());
        dptn = *it1;
        length = dpt->l1;
    }
    else if (it2 != idsok.end() and (dptn = *it2)->WhichNeighbourHasID(dpt->id) > 0)
    {
        assert(it1 == idsok.end());
        dptn = *it2;
        length = dpt->l2;
    }
    else {
        return;
        assert("WRONG OLD COLLISION POINT" && false);
    }
    //*/
    out << "   dpt : " << dpt << endl;
    out << "  dptn : " << dptn << endl;

    REAL xx = dptn->x;
    REAL yy = dptn->y;

    REAL  dist = Geometry::PointsDistance(dpt->x, dpt->y, dptn->x, dptn->y);
    REAL scale = (length / dist);

    out << "  dist : " << dist << endl;
    out << " scale : " << scale << endl;

    xx += scale * ( dpt->x - dptn->x );
    yy += scale * ( dpt->y - dptn->y );

    Point<REAL> * newpt = new Point<REAL>(xx, yy, dptn, NULL, length, 0.0, ++id, PT_Update);

    int which = dptn->WhichNeighbourHasID(dpt->id);
    out << " which : " << which << endl;
    assert(which > 0);
    if (which == 1)
    {
        dptn->n1 = newpt;
        assert(dptn->l1 == length);
    }
    else {
        dptn->n2 = newpt;
        assert(dptn->l2 == length);
    }
    out << " newpt : " << newpt << endl;

    out << "     x : " << newpt->x << endl;
    out << " x + E : " << newpt->x + EPSILON << endl;
    out << "    x0 : " << line->GetX0() << endl;

    bool res = ((newpt->x + EPSILON) >= line->GetX0());
    out << "   res : " << (res ? "TRUE" : "FALSE")  << endl;

    assert( ArrangeNewEvent(newpt, evts, line, id, sinL, cosL, true) );
    out << " newpt -< added" << endl;

    return;
}


template <class REAL>
bool
PMF<REAL> :: IsTheEventInvalid (REAL sinL, REAL cosL, Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, /*set<Point<REAL> *> & idsok,*/ long & id)
{
    //if (IsTheEventInvalid (ev, evts, line)) return true;
    //*
    if (ev->GetType() == PointUpdate  ||  ev->GetType() == BorderDeath)
    {
        if (! line->HasSegmentWithEndId(ev->GetPoint()->id))
        {
            Point<REAL> * tmp = ev->GetPoint();
            Segment<REAL> * seg = ev->GetSegment();
            evts->Erase(ev);
            delete seg;
            delete tmp;
            return true;
        }
    }
    else if (ev->GetType() == NormalDeath)
    {
        Segment<REAL> * seg1 = ev->GetSegment();
        Segment<REAL> * seg2 = ev->GetSegment(false);

        //if ( (! line->IsNullHasSegmentWithEndId(ev->GetSegment(true)->GetQ()->id))
        //    || (! line->HasSegmentWithEndId(ev->GetSegment(false)->GetQ()->id)) )
        if (line->IsNull(line->Find(seg1)) || line->IsNull(line->Find(seg2)))
        {
            delete ev->GetPoint();
            evts->Erase(ev);
            return true;
        }
    }
    else
    //*/
    //return false;
    if (ev->GetType() == OldPoint)
    {
        Point<REAL> * pt = ev->GetPoint();

        if (pt->type == PT_Collision)
        {
            Segment<REAL> * seg1 = ev->GetSegment();
            Segment<REAL> * seg2 = ev->GetSegment(false);

            if (line->IsNull(line->Find(seg1)) || line->IsNull(line->Find(seg2)))
            //if (! line->HasSegmentWithEndId(pt->id))
            {
                if (! line->IsNull(line->Find(seg1)))
                {
                    ForgetOldCollisionPoint(sinL, cosL, ev, evts, line, /*idsok,*/ id);
                }
                else if (! line->IsNull(line->Find(seg2)))
                {
                    ForgetOldCollisionPoint(sinL, cosL, ev, evts, line, /*idsok,*/ id);
                }

                Point<REAL> * tmp = ev->GetPoint();
                evts->Erase(ev);
                delete seg1;
                delete seg2;
                delete tmp;
                return true;
            }
        }
        else if (pt->type != PT_BirthInField  &&  pt->type != PT_BirthOnBorder)
        {
            if (! line->HasSegmentWithEndId(pt->id))
            {
                Point<REAL> * tmp = ev->GetPoint();
                Segment<REAL> * seg = ev->GetSegment();
                evts->Erase(ev);
                delete seg;
                delete tmp;
                return true;
            }
        }
    }
    /*
    if (ev->GetType() == OldPoint)
    {
        Point<REAL> * pt = ev->GetPoint();

        if (pt->type == PT_Collision)
        {
            if (! line->HasSegmentWithEndId(pt->id))
            {
                ForgetOldCollisionPoint(sinL, cosL, ev, evts, line, idsok, id);

                Point<REAL> * tmp = ev->GetPoint();
                Segment<REAL> * seg1 = ev->GetSegment();
                Segment<REAL> * seg2 = ev->GetSegment(false);
                evts->Erase(ev);
                delete seg1;
                delete seg2;
                delete tmp;
                return true;
            }
        }
        else if (pt->type != PT_BirthInField  &&  pt->type != PT_BirthOnBorder)
        {
            if (! line->HasSegmentWithEndId(pt->id))
            {
                Point<REAL> * tmp = ev->GetPoint();
                Segment<REAL> * seg = ev->GetSegment();
                evts->Erase(ev);
                delete seg;
                delete tmp;
                return true;
            }
        }
    }
    //*/
    return false;
}


template <class REAL>
void
PMF<REAL> :: EvolveTheRestOfField (REAL sinL, REAL cosL, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long id)
{
    //set<Point<REAL> *> idsok;
    //long oldSize = id + 1;

    long step = 0;
    while (! evts->IsEmpty())
    {
        out << endl << "_________________________________________________" << endl;
        out << "  STEP " << (++step) << endl;
        Event * evt = evts->SeeFirst();

        out << endl;
        out << evts << endl;
        out << "... event at point " << evt->GetPoint() << endl;
        out << line << endl;
        //out << "[ IDs OK ] : ";  FOREACH(it, idsok) out << " " << (*it)->id;  out << endl;

        if (IsTheEventInvalid(sinL, cosL, evt, evts, line, /*idsok,*/ id)) continue;

        cf->PushBack(evt->GetPoint());
        //idsok.insert(evt->GetPoint());

        out << " ... point added : " << evt->GetPoint() << endl;
        switch (evt->GetType())
        {
            case    OldPoint :
                    PMFLogV("-{%i}-> _OLD_ EVENT", step);
                    ProcessOldEvent(evt, evts, line, id, sinL, cosL);
                    break;;
            case BorderBirth :
                    PMFLogV("-{%i}-> BIRTH EVENT", step);
            case PointUpdate :
                    PMFLogV("-{%i}-> UPDATE EVENT", step);
                    ProcessUpdateEvent(evt, evts, line, id, sinL, cosL);
                    break;;
            case NormalDeath :
            case BorderDeath :
                    PMFLogV("-{%i}-> DEATH EVENT", step);
                    ProcessDeathEvent(evt, evts, line, id, sinL, cosL);
                    break;;
            case NormalBirth :
                    PMFLogV("-{%i}-> BIRTH EVENT", step);
            default :
                    assert("WRONG EVENT TYPE DURING EVOLUTION" && false);
        }
        evts->Erase(evt);
        out << "-------------------------------------------------" << endl;
    }

    //*
    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        out << pt << "   " << pt->org_x << " ; " << pt->org_y << endl;
        pt->ResumeCoordinates();
        out << "\t\t\t" << pt << endl;
    }
    //*/
    RotatePoints ();
    cf->SetPointsIDs ();
    return;
}


#endif // EVOLVE_HPP_INCLUDED
