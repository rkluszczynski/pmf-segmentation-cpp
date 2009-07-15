#ifndef COMMON_HPP_INCLUDED
#define COMMON_HPP_INCLUDED


template <class REAL>
inline
bool
PMF<REAL> :: IsPointInsideTheField (REAL x, REAL y)
{
    return ( 0.0 < x  &&  x < GetWidth()  &&  0.0 < y  &&  y < GetHeight() );
}


template <class REAL>
inline
Point<REAL> *
PMF<REAL> :: DetectPossibleCollision (Segment<REAL> * seg1, Segment<REAL> * seg2, long & id)
{
    using namespace Geometry;

    if (seg1->GetP()->id == seg2->GetP()->id) return NULL;

    Point<REAL> * result = NULL;
    int collision = CheckIntersection2<REAL>( seg1->GetP()->x, seg1->GetP()->y, seg1->GetQ()->x, seg1->GetQ()->y,
                                              seg2->GetP()->x, seg2->GetP()->y, seg2->GetQ()->x, seg2->GetQ()->y );
    cout << "COLLISION VALUE = " << collision << endl;
    if (collision > 0)
    {
        pair<REAL, REAL> cpt = CalculateIntersection<REAL> (
                                            seg1->GetP()->x, seg1->GetP()->y, seg1->GetQ()->x, seg1->GetQ()->y,
                                            seg2->GetP()->x, seg2->GetP()->y, seg2->GetQ()->x, seg2->GetQ()->y );
        /// TODO (klusi#2#): possible BUG : when collision point is an EPSILON further then two update points very near to each other (less then EPSILON)
        /// NOTE (klusi#1#): note in previous line probably fixed by ordering
        if (IsPointInsideTheField(cpt.ST, cpt.ND))
            result = new Point<REAL> (cpt.ST, cpt.ND, seg1->GetP(), seg2->GetP(), 0.0, 0.0, ++id, PT_Collision);
    }
    return result;
}


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))
template <class REAL>
bool
PMF<REAL> :: ArrangeNewEvent (Point<REAL> * npt, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id, REAL sinL, REAL cosL)
{
    typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;
    using  namespace Geometry;

    Point<REAL> * parent = npt->n1;
    Segment<REAL> * nseg = new Segment<REAL>(parent, npt);

    REAL nx = X_ROTATED(npt->x, npt->y, -sinL, cosL);
    REAL ny = Y_ROTATED(npt->x, npt->y, -sinL, cosL);

    // determine type of event (update or death)
    if (IsPointInsideTheField(nx, ny))
    //if (IsPointInsideTheField(npt->x, npt->y))
    {
        // npt inside the field
        cout << " ---->  new point : " << npt << endl;
        npt->org_x = nx;
        npt->org_y = ny;

        // new event
        UpdateEvent * e = new UpdateEvent(npt, nseg);
        bool ans = evts->Insert(e);
        if (! ans)
        {
            delete nseg;
            delete e;
            return false;
        }
    }
    else {
        REAL px = X_ROTATED(parent->x, parent->y, -sinL, cosL);
        REAL py = Y_ROTATED(parent->x, parent->y, -sinL, cosL);

        // npt outside the field
        int up    = CheckIntersection2<REAL>(nx, ny, px, py, 0.0, 0.0, GetWidth(), 0.0);
        int down  = CheckIntersection2<REAL>(nx, ny, px, py, 0.0, GetHeight(), GetWidth(), GetHeight());
        int right = CheckIntersection2<REAL>(nx, ny, px, py, GetWidth(), 0.0, GetWidth(), GetHeight());
        int left  = CheckIntersection2<REAL>(nx, ny, px, py, 0.0, 0.0, 0.0, GetHeight());
        //int up    = CheckIntersection2<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, 0.0, GetWidth(), 0.0);
        //int down  = CheckIntersection2<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, GetHeight(), GetWidth(), GetHeight());
        //int right = CheckIntersection2<REAL>(npt->x, npt->y, parent->x, parent->y, GetWidth(), 0.0, GetWidth(), GetHeight());
        //int left  = CheckIntersection2<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, 0.0, 0.0, GetHeight());

        /// FIXME (Rafel#1#): what if point is on border ??

        REAL cx, cy;
        if (up != 0  &&  up != 5) {
            CalculateIntersection<REAL>(nx, ny, px, py, 0.0, 0.0, GetWidth(), 0.0, cx, cy);
        }
        else if (down != 0  &&  down != 5) {
            CalculateIntersection<REAL>(nx, ny, px, py, 0.0, GetHeight(), GetWidth(), GetHeight(), cx, cy);
        }
        else if (right != 0  &&  right != 5) {
            CalculateIntersection<REAL>(nx, ny, px, py, GetWidth(), 0.0, GetWidth(), GetHeight(), cx, cy);
        }
        else if (left != 0  &&  left != 5) {
            CalculateIntersection<REAL>(nx, ny, px, py, 0.0, 0.0, 0.0, GetHeight(), cx, cy);
        }
        else assert(false);

        npt->org_x = cx;
        npt->org_y = cy;

        npt->x = X_ROTATED(cx, cy, sinL, cosL);
        npt->y = Y_ROTATED(cx, cy, sinL, cosL);

        npt->n2 = NULL;
        npt->l2 = 0.0;
        npt->type = PT_DeathOnBorder;

        cout << " ---->  new point : " << npt << endl;
        // new event
        DeathEvent * e = new DeathEvent(npt, nseg);
        bool ans = evts->Insert(e);
        if (! ans)
        {
            delete nseg;
            delete e;
            return false;
        }
    }

    pair<SweepIterator, bool> res = line->Insert(parent, nseg);
    assert(res.ND == true);

    // check the segments above and below for intersections
    SweepIterator ita = line->Above(res.ST);
    SweepIterator itb = line->Below(res.ST);
    //if (! line->IsNull(ita)  &&  ita->GetSegment()->GetP() != parent)
    if (! line->IsNull(ita))
    {
        Point<REAL> * cpt = DetectPossibleCollision (nseg, (*ita)->GetSegment(), id);
        if (cpt)
        {
            DeathEvent * de = new DeathEvent(cpt, nseg, (*ita)->GetSegment());
            evts->Insert(de);
        }
    }
    //if (! line->IsNull(itb)  &&  itb->GetSegment()->GetP() != parent)
    if (! line->IsNull(itb))
    {
        Point<REAL> * cpt = DetectPossibleCollision (nseg, (*itb)->GetSegment(), id);
        if (cpt)
        {
            DeathEvent * de = new DeathEvent(cpt, nseg, (*itb)->GetSegment());
            evts->Insert(de);
        }
    }
    return true;
}
#undef X_ROTATED
#undef Y_ROTATED


template <class REAL>
inline
void
PMF<REAL> :: ProcessBirthEvent (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id, REAL sinL, REAL cosL)
{
    using namespace Probability;
    Point<REAL> * newpt1 = NULL, * newpt2 = NULL;

    Point<REAL> * pt = ev->GetPoint();
    while (true)
    {
        REAL upperLength = Exp<REAL>(2.0);
        REAL lowerLength = Exp<REAL>(2.0);
        REAL upperAngle, lowerAngle;
        DetermineBirthAngles (upperAngle, lowerAngle);

        if (! newpt1)
        {
            newpt1 = pt->GenerateNeighbour(1, upperAngle, id, upperLength);
            bool ans = ArrangeNewEvent(newpt1, evts, line, id, sinL, cosL);
            if (! ans)
            {
                delete newpt1;
                newpt1 = NULL;
                continue;
            }
        }
        if (! newpt2)
        {
            newpt2 = pt->GenerateNeighbour(2, lowerAngle, id, lowerLength);
            bool ans = ArrangeNewEvent(newpt2, evts, line, id, sinL, cosL);
            if (! ans)
            {
                delete newpt2;
                newpt2 = NULL;
                continue;
            }
        }
        break;
    }
    return;
}



template <class REAL>
inline
void
PMF<REAL> :: ProcessUpdateEvent (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id, REAL sinL, REAL cosL)
{
    using namespace Probability;

    Point<REAL> * pt = ev->GetPoint();
    Segment<REAL> * seg = ev->GetSegment();
    if (seg)
    {
        line->Erase(seg);
        delete seg;
    }
    while(true)
    {
        REAL angle, newAngle;
        if (pt->type == PT_Update)  angle = atan((pt->y - pt->n1->y) / (pt->x - pt->n1->x));
        else  angle = 0.0;
        DetermineUpdateAngle<REAL> (newAngle);

        newAngle += angle;
        if (newAngle > M_PI_2)  newAngle -= M_PI;
        if (newAngle < -M_PI_2) newAngle += M_PI;

        if (pt->type == PT_BirthOnBorder  &&  pt->y == 0.0  &&  newAngle < 0)  newAngle = -newAngle;
        if (pt->type == PT_BirthOnBorder  &&  pt->y == GetHeight()  &&  newAngle > 0)  newAngle = -newAngle;

        int whichNeighbour = (pt->type == PT_Update) ? 2 : 1;
        Point<REAL> * newpt = pt->GenerateNeighbour(whichNeighbour, newAngle, id, Exp<REAL> (2.0));
        cout << " -> new point  :  " << newpt << endl;
        if (ArrangeNewEvent(newpt, evts, line, id, sinL, cosL)) break;

        delete newpt;
    }
    return;
}


template <class REAL>
inline
void
PMF<REAL> :: CorrectCollisionStartPoints (Point<REAL> * pt, long id1, long id2)
{
    wxLogMessage(wxString::Format(_("Correcting collision point of ends %i and %i"), id1, id2));
    if (pt->n1->n1 != NULL  &&  pt->n1->n1->id == id1)
    {
        PMFLogV("########### 1");
        pt->n1->n1 = pt;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id2)  pt->n2->n1 = pt;
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id2)  pt->n2->n2 = pt;
    }
    if (pt->n1->n2 != NULL  &&  pt->n1->n2->id == id1)
    {
        PMFLogV("########### 2");
        pt->n1->n2 = pt;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id2)  pt->n2->n1 = pt;
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id2)  pt->n2->n2 = pt;
    }
    if (pt->n1->n1 != NULL  &&  pt->n1->n1->id == id2)
    {
        PMFLogV("########### 3");
        pt->n1->n1 = pt;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id1)  pt->n2->n1 = pt;
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id1)  pt->n2->n2 = pt;
    }
    if (pt->n1->n2 != NULL  &&  pt->n1->n2->id == id2)
    {
        PMFLogV("########### 4");
        pt->n1->n2 = pt;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id1)  pt->n2->n1 = pt;
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id1)  pt->n2->n2 = pt;
    }
}


template <class REAL>
inline
void
PMF<REAL> :: ProcessDeathEvent (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id, REAL sinL, REAL cosL)
{
    typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;

    Segment<REAL> * s1, * s2;
    s1 = ev->GetSegment();
    s2 = ev->GetSegment(false);

    set<Segment<REAL> *> nn;

    nn.insert( s1 );
    SweepIterator it_1 = line->Find( s1 );
    SweepIterator it1a = line->Above( it_1 );
    SweepIterator it1b = line->Below( it_1 );
    if (! line->IsNull(it1a))  nn.insert( (*it1a)->GetSegment() );
    if (! line->IsNull(it1b))  nn.insert( (*it1b)->GetSegment() );
    if (s2)
    {
        nn.insert( s2 );
        SweepIterator it_2 = line->Find( s2 );
        SweepIterator it2a = line->Above( it_2 );
        SweepIterator it2b = line->Below( it_2 );
        if (! line->IsNull(it2a))  nn.insert( (*it2a)->GetSegment() );
        if (! line->IsNull(it2b))  nn.insert( (*it2b)->GetSegment() );
    }
    nn.erase( s1 );
    nn.erase( s2 );
    assert(nn.size() <= 2);
    //*/
    if (s1 && s2)
    {
        long id1 = s1->GetQ()->id;
        long id2 = s2->GetQ()->id;
        CorrectCollisionStartPoints (ev->GetPoint(), id1, id2);
    }

    PMFLogV("Death with %i status neighbour(s) !!!", nn.size());

    if (nn.size() == 2)
    {
        Point<REAL> * cpt = DetectPossibleCollision (*nn.begin(), *nn.rbegin(), id);
        if (cpt)
        {
            cout << endl << "COLLISION : " << cpt << endl;

            cout << *nn.begin() << endl;
            cout << *nn.rbegin() << endl;

            DeathEvent * de = new DeathEvent(cpt, *nn.begin(), *nn.rbegin());
            evts->Insert(de);
        }
    }
    line->Erase( s1 );

    if (s2)  line->Erase( s2 );
    else   delete s1;

    return;
}


#endif // COMMON_HPP_INCLUDED
