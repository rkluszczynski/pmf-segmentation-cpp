#ifndef EVOLVE_HPP_INCLUDED
#define EVOLVE_HPP_INCLUDED


template <class REAL>
inline
void
PMF<REAL> :: ProcessOldEvent (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id)
{
    Point<REAL>   * pt = ev->GetPoint();
    Segment<REAL> * s1 = ev->GetSegment();
    Segment<REAL> * s2 = ev->GetSegment(false);

    switch (pt->type)
    {
        case  PT_BirthInField :
                            line->Insert( pt, s2 );
        case PT_BirthOnBorder :
                            line->Insert( pt, s1 );
                            break;;
        case     PT_Collision :
                            line->Erase( s2 );
        case PT_DeathOnBorder :
                            line->Erase( s1 );
                            break;;
        case        PT_Update :
                            line->Erase( s1 );
                            line->Insert( pt, s2 );
                            break;;
        default:
            assert("WRONG POINT TYPE" && false);
    }
    return;
}


template <class REAL>
inline
void
PMF<REAL> :: ForgetOldCollisionPoint (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, set<long> & idsok, long & id)
{
    ///TODO
    return;
}


template <class REAL>
void
PMF<REAL> :: EvolveTheRestOfField (REAL sinL, REAL cosL, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long id)
{
    set<long> idsok;
    long oldSize = id + 1;

    long step = 0;
    while (! evts->IsEmpty())
    {
        cout << endl << "_________________________________________________" << endl;
        cout << "  STEP " << (++step) << endl;
        Event * evt = evts->SeeFirst();

        cout << endl;
        cout << evts << endl;
        cout << "... event at point " << evt->GetPoint() << endl;
        cout << line << endl;

        /// check if the event is still valid
        //*
        if (evt->GetType() == PointUpdate  ||  evt->GetType() == BorderDeath)
        {
            if (! line->HasSegmentWithEndId(evt->GetPoint()->id))
            {
                Point<REAL> * tmp = evt->GetPoint();
                Segment<REAL> * seg = evt->GetSegment();
                evts->Erase(evt);
                delete seg;
                delete tmp;
                continue;
            }
        }
        else if (evt->GetType() == NormalDeath)
        {
            if ( (! line->HasSegmentWithEndId(evt->GetSegment(true)->GetQ()->id))
                || (! line->HasSegmentWithEndId(evt->GetSegment(false)->GetQ()->id)) )
            {
                delete evt->GetPoint();
                evts->Erase(evt);
                continue;
            }
        }
        else if (evt->GetType() == OldPoint)
        {
            Point<REAL> * pt = evt->GetPoint();

            if (pt->type == PT_Collision)
            {
                if (! line->HasSegmentWithEndId(pt->id))
                {
                    ForgetOldCollisionPoint(evt, evts, line, idsok, id);
                }
            }
            else
                if (! line->HasSegmentWithEndId(pt->id))
                {
                    Point<REAL> * tmp = evt->GetPoint();
                    Segment<REAL> * seg = evt->GetSegment();
                    evts->Erase(evt);
                    delete seg;
                    delete tmp;
                    continue;
                }
        }
        //*/

        cf->PushBack(evt->GetPoint());
        idsok.insert(evt->GetPoint()->id);
        switch (evt->GetType())
        {
            case    OldPoint :
                    PMFLogV("-> _OLD_ EVENT");
                    ProcessOldEvent(evt, evts, line, id);
                    break;;
            case NormalBirth :
                    PMFLogV("-> BIRTH EVENT");
                    ProcessBirthEvent(evt, evts, line, id);
                    break;;
            case BorderBirth :
                    PMFLogV("-> BIRTH EVENT");
            case PointUpdate :
                    PMFLogV("-> UPDATE EVENT");
                    ProcessUpdateEvent(evt, evts, line, id);
                    break;;
            case NormalDeath :
            case BorderDeath :
                    PMFLogV("-> DEATH EVENT");
                    ProcessDeathEvent(evt, evts, line, id);
                    break;;
            default :
                    assert(false);
        }
        evts->Erase(evt);
        cout << "-------------------------------------------------" << endl;
    }

    /*
    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        pt->ResumeCoordinates();
    }
    //*/
    return;
}


#endif // EVOLVE_HPP_INCLUDED
