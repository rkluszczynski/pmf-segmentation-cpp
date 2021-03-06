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
                            out << line << endl;
                            delete s1;
                            line->Erase( s2 );
                            delete s2;
                            out << " CHECK 1 " << endl;
                            out << line << endl;
                            out << " CHECK 2 " << endl;
                            break;;
        case PT_DeathOnBorder :
                            CheckIntersectionAfterDeath (s1, NULL, evts, line, id, sinL, cosL);
                            line->Erase( s1 );
                            delete s1;
                            break;;
        case        PT_Update :
                            //CheckIntersectionAfterDeath (s1, NULL, evts, line, id, sinL, cosL);
                            /// FIXME (Rafel#4#): do not know if line above should exist ??? It is checked during insertion //
                            line->Erase( s1 );
                            delete s1;
                            //line->Insert( pt, s2 );
                            assert(s2);
                            InsertNewSegmentIntoSweep (pt, s2, evts, line, id, sinL, cosL);
                            break;;
        default:
            assert("WRONG OLD POINT TYPE" && false);
    }
    return;
}


template <class REAL>
inline
bool
PMF<REAL> :: CheckExistanceAfterForgeting  (Point<REAL> * npt, REAL sinL, REAL cosL, Point<REAL> * dpt, Point<REAL> * dptn, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, /*set<Point<REAL> *> & idsok,*/ long & id)
{
    typedef typename EventsSchedule<REAL>::Iterator EventIterator;
    bool pointExistance = false;

    PointType type = npt->type;
    npt->type = PT_BirthOnBorder;

    OldEvent * op = new OldEvent(npt, NULL, NULL);
    EventIterator it = evts->LowerBound( op );
    delete op;

    std::vector<Point<REAL> *> vec;
    npt->type = type;

    REAL epsilon = nparams.GetAxisEpsilon();
    while (true)
    {
        if (it == evts->End()) break;

        Point<REAL> * tmp = (*it)->GetPoint();

        if (! Geometry::IsZero( npt->x - tmp->x, epsilon )) break;

        if (Geometry::IsZero( npt->y - tmp->y, epsilon ))
        {
            vec.push_back(tmp);
        }
        ++it;
    }

    if (vec.size() > 0)
    {
        FOREACH(vit, vec)
            out << endl << " EXIST : " << *vit << endl << endl;
        pointExistance = true;
    }
    else
        out << endl << "NOTHING EXISTS" << endl << endl;

    return pointExistance;
}


template <class REAL>
inline
void
//PMF<REAL> :: ForgetOldCollisionPoint (REAL sinL, REAL cosL, Point<REAL> * dpt, Segment<REAL> * seg, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, /*set<Point<REAL> *> & idsok,*/ long & id)
PMF<REAL> :: ForgetOldCollisionPoint (REAL sinL, REAL cosL, Point<REAL> * dpt, Point<REAL> * dptn, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, /*set<Point<REAL> *> & idsok,*/ long & id)
{
    typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;
    typedef typename set<Point<REAL> *>::iterator  OkPointsSetIterator;

    //Point<REAL> * dpt = ev->GetPoint();
    //OkPointsSetIterator it1 = idsok.find(dpt->n1);
    //OkPointsSetIterator it2 = idsok.find(dpt->n2);

    //SweepIterator it1 = line->Find( ev->GetSegment(true) );
    //SweepIterator it2 = line->Find( ev->GetSegment(false) );

    ///Point<REAL> * dptn = seg->GetP(); //
    /*
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
    // */
    out << "   dpt : " << dpt << "  ( " << dpt->org_x << " ; " << dpt->org_y << " )" << endl;
    out << "  dptn : " << dptn << "  ( " << dptn->org_x << " ; " << dptn->org_y << " )" << endl;

    int wh = dptn->WhichNeighbourHasID( dpt->id );
    assert(wh > 0);
    REAL length = (wh == 1) ? dptn->l1 : dptn->l2;
    // 2009-11-14
    REAL depsilon = nparams.GetDistEpsilon();
    length += ( GetPRNG()->GetExp(1.0) + depsilon );
    if (wh == 1) dptn->l1 = length;
    else dptn->l2 = length;
    // *

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

    /// FIXME (Rafel#1#): Check if the point exist before adding //
    bool exists = CheckExistanceAfterForgeting (newpt, sinL, cosL, dpt, dptn, evts, line, id);
    if (exists)
    {
        cout << " NOT NICE POINT EXISTANCE " << endl;

        printf("QQ ->>\n");
        //*
        ///REAL newLength = Probability::Uniform<REAL>(dist + EPSILON, length - EPSILON);
        REAL newLength = GetPRNG()->GetUniform(dist + depsilon, length - depsilon);
        REAL newScale = newLength / length;
        assert(newScale < 1.0);
        assert(newScale > dist/length);

        cout << " existing newpt = " << newpt->x << " , " << newpt->y << endl;
        cout << "  source  dptn  = " << dptn->x << " , " << dptn->y << endl;

        REAL nxx = dptn->x + newScale * ( newpt->x - dptn->x );
        REAL nyy = dptn->y + newScale * ( newpt->y - dptn->y );

        cout << "                = " << nxx << " , " << nyy << endl;
        newpt->x = nxx;
        newpt->y = nyy;

        //scanf("%*i");
        //*/
        printf("-<<\n");
    }
    out << "     x : " << newpt->x << endl;
    out << " x + E : " << newpt->x + nparams.GetAxisEpsilon() << endl;
    out << "    x0 : " << line->GetX0() << endl;

    bool res = ((newpt->x + nparams.GetAxisEpsilon()) >= line->GetX0());
    out << "   res : " << (res ? "TRUE" : "FALSE")  << endl;

    assert( ArrangeNewEvent(newpt, evts, line, id, sinL, cosL, true) );
    out << " newpt -< added" << endl;

    return;
}


template <class REAL>
bool
PMF<REAL> :: IsTheEventInvalid (REAL sinL, REAL cosL, Event * & ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, /*set<Point<REAL> *> & idsok,*/ long & id)
{
    typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;
    //if (IsTheEventInvalid (ev, evts, line)) return true;
    //*
    if (ev->GetType() == PointUpdate  ||  ev->GetType() == BorderDeath)
    {
        //if (! line->HasSegmentWithEndId(ev->GetPoint()->id))
        if (line->IsNull(line->Find(ev->GetSegment())))
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
        /*
            SweepIterator it1 = line->Find(seg1);
            SweepIterator it2 = line->Find(seg2);

            if (!line->IsNull(it1)) out << " -1-> " << (*it1)->GetSegment() << endl;
            if (!line->IsNull(it2)) out << " -2-> " << (*it2)->GetSegment() << endl;
            out << " ... searching 1st line (" << (line->IsNull(it1) ? "NULL" : " OK ") << ") : " << seg1 << endl;
            out << " ... searching 2nd line (" << (line->IsNull(it2) ? "NULL" : " OK ") << ") : " << seg2 << endl;
        // */
        if (line->IsNull(line->Find(seg1)) || line->IsNull(line->Find(seg2)))
        {
            Point<REAL> * tmp = ev->GetPoint();
            evts->Erase(ev);
            //evts->Erase( evts->SeeFirst() );
            delete tmp;
            return true;
        }
    }
    else
    // */
    //return false;
    if (ev->GetType() == OldPoint)
    {
        Point<REAL> * pt = ev->GetPoint();

        if (pt->type == PT_Collision)
        {
            Segment<REAL> * seg1 = ev->GetSegment();
            Segment<REAL> * seg2 = ev->GetSegment(false);
            assert(seg1);
            assert(seg2);
            //line->SetSweepLinePosition( pt->x + EPSILON);
            //REAL savedX0 = line->GetX0();
            REAL epsilon = nparams.GetAxisEpsilon();
            if (! (line->GetX0() < pt->x))
            {
                line->SetSweepLinePosition2( pt->x - epsilon * 0.5 );
            }
            line->SetSweepLinePosition2( pt->x - epsilon * 0.5 );
            assert(line->GetX0() < pt->x);

            /*
            SweepIterator it1 = line->Find(seg1);
            SweepIterator it2 = line->Find(seg2);
            out << line << endl;
            if (!line->IsNull(it1)) out << " -1-> " << (*it1)->GetSegment() << endl;
            if (!line->IsNull(it2)) out << " -2-> " << (*it2)->GetSegment() << endl;
            out << " ... searching 1st line (" << (line->IsNull(it1) ? "NULL" : " OK ") << ") : " << seg1 << endl;
            out << " ... searching 2nd line (" << (line->IsNull(it2) ? "NULL" : " OK ") << ") : " << seg2 << endl;
            // */
            bool cond1 = (pt->n1 == NULL);// || line->IsNull(it1);
            out << "  cond1  = " << (cond1 ? "TRUE" : "FALSE") << endl;
            if (! cond1  &&  line->IsNull( line->Find(seg1) )) cond1 = true;
            out << "  cond1  = " << (cond1 ? "TRUE" : "FALSE") << endl;

            bool cond2 = (pt->n2 == NULL);// || line->IsNull(it2);
            out << "  cond2  = " << (cond2 ? "TRUE" : "FALSE") << endl;
            if (! cond2  &&  line->IsNull( line->Find(seg2) )) cond2 = true;
            out << "  cond2  = " << (cond2 ? "TRUE" : "FALSE") << endl;

            if (cond1 || cond2)
            {
                Point<REAL> * tmp = ev->GetPoint();
                evts->Erase(ev);
                if (! cond1)
                {
                    line->Erase( line->Find(seg1) );
                    ForgetOldCollisionPoint(sinL, cosL, pt, seg1->GetP(), evts, line, /*idsok,*/ id);
                }
                else if (! cond2)
                {
                    line->Erase( line->Find(seg2) );
                    ForgetOldCollisionPoint(sinL, cosL, pt, seg2->GetP(), evts, line, /*idsok,*/ id);
                }
                delete seg1;
                delete seg2;
                delete tmp;

                //line->SetSweepLinePosition2(savedX0);

                return true;
            }
            //line->SetSweepLinePosition2(savedX0);
        }
        else if (pt->type != PT_BirthInField  &&  pt->type != PT_BirthOnBorder)
        {
            ///out << " ... checking if segment is valid : " << ev->GetSegment() << endl;
            // sometimes sweep line is put before the beginning of the segment while
            // collision was processed, and then this segment is not found in the set
            // (@rufus : size 3, seed 35, iteration 27270)
            line->SetSweepLinePosition2(ev->GetPoint()->x);

            //if (! line->HasSegmentWithEndId(pt->id))
            if (pt->n1 == NULL  ||  line->IsNull(line->Find(ev->GetSegment())))
            {
                Point<REAL> * tmp = ev->GetPoint();
                if (tmp->n2  &&  (tmp->n2->type == PT_Collision || tmp->n2->type == PT_DeathOnBorder || tmp->n2->type == PT_Update))
                {
                    Point<REAL> * cpt = tmp->n2;

                    if (cpt->n1 == tmp) { cpt->n1 = NULL; }
                    else if (cpt->n2 == tmp) { cpt->n2 = NULL; }
                    else  assert("SOMETHING IS WRONG" && false);
                }
                Segment<REAL> * seg = ev->GetSegment();
                evts->Erase(ev);
                delete seg;
                delete tmp;
                return true;
            }
        }
    }
    return false;
}


template <class REAL>
void
PMF<REAL> :: EvolveTheRestOfField (REAL sinL, REAL cosL, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long id)
{
    long step = 0;
    //*
    out << endl << "_________________________________________________" << endl;
    out << evts << endl;
    out << "evts.size() == " << evts->size() << endl;
    out << endl << "_________________________________________________" << endl;
    // */
    while (! evts->IsEmpty())
    {
        out << " PMF_ELEMENT_COUNTER  = " << pmf::pmf_element_counter << endl;
        ///out << " PMF_SEGMENT_COUNTER  = " << pmf::pmf_segment_counter << endl;
        out << endl << "_________________________________________________" << endl;
        out << "  STEP " << (++step) << endl;
        Event * evt = evts->SeeFirst();

        out << endl;
        out << evts << endl;
        out << "... event at point " << evt->GetPoint() << endl;
        out << line << endl;

        if (IsTheEventInvalid(sinL, cosL, evt, evts, line, /*idsok,*/ id)) continue;

        cf->PushBack(evt->GetPoint());
        out << " ... point added : " << evt->GetPoint() << endl;

        switch (evt->GetType())
        {
            case    OldPoint :
                    PMFLogV("-{%li}-> _OLD_ EVENT", step);
                    ProcessOldEvent(evt, evts, line, id, sinL, cosL);
                    break;;
            case BorderBirth :
                    PMFLogV("-{%li}-> BIRTH EVENT", step);
            case PointUpdate :
                    PMFLogV("-{%li}-> UPDATE EVENT", step);
                    ProcessUpdateEvent(evt, evts, line, id, sinL, cosL);
                    break;;
            case NormalDeath :
            case BorderDeath :
                    PMFLogV("-{%li}-> DEATH EVENT", step);
                    ProcessDeathEvent(evt, evts, line, id, sinL, cosL);
                    break;;
            case NormalBirth :
                    PMFLogV("-{%li}-> BIRTH EVENT", step);
            default :
                    assert("WRONG EVENT TYPE DURING EVOLUTION" && false);
        }
        evts->Erase(evt);
        out << line << endl;
        out << "----------------------------------------------------" << endl;
    }
    out << endl;
    out << "___after__iterations________________________________" << endl;
    out << evts << endl;
    out << line << endl;
    out << " PMF_ELEMENT_COUNTER  = " << pmf::pmf_element_counter << endl;
    //assert(pmf::pmf_element_counter == 0);
    ///out << " PMF_SEGMENT_COUNTER  = " << pmf::pmf_segment_counter << endl;

    //*
    FOREACH(it, *cf)
    {
        Point<REAL> * pt = *it;
        out << pt << "   " << pt->org_x << " ; " << pt->org_y << endl;
        pt->ResumeCoordinates();
        out << "\t\t\t" << pt << endl;
    }
    // */
    cf->SetPointsIDs ();
    //RotatePoints2 ();
    return;
}


#endif // EVOLVE_HPP_INCLUDED
