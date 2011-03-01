#ifndef COMMON_HPP_INCLUDED
#define COMMON_HPP_INCLUDED


template <class REAL>
inline
bool
PMF<REAL> :: IsPointInsideTheField (REAL x, REAL y)
{
    return ( 0.0 < x  &&  x < GetWidth()  &&  0.0 < y  &&  y < GetHeight() );
}


#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (CCOS) - (YY) * (SSIN))
#define Y_ROTATED(XX,YY,SSIN,CCOS) ((XX) * (SSIN) + (YY) * (CCOS))
template <class REAL>
inline
Point<REAL> *
PMF<REAL> :: DetectPossibleCollision (Segment<REAL> * seg1, Segment<REAL> * seg2, long & id, REAL sinL, REAL cosL)
{
    using namespace Geometry;

    if (seg1->GetP()->id == seg2->GetP()->id) return NULL;
    if (seg1->GetQ()->id == seg2->GetQ()->id) return NULL;

    Point<REAL> * result = NULL;
    int collision = CheckIntersection2<REAL>( seg1->GetP()->x, seg1->GetP()->y, seg1->GetQ()->x, seg1->GetQ()->y,
                                              seg2->GetP()->x, seg2->GetP()->y, seg2->GetQ()->x, seg2->GetQ()->y );
    out << "COLLISION VALUE = " << collision << endl;
    if (collision > 0)
    {
        pair<REAL, REAL> cpt = CalculateIntersection<REAL> (
                                            seg1->GetP()->x, seg1->GetP()->y, seg1->GetQ()->x, seg1->GetQ()->y,
                                            seg2->GetP()->x, seg2->GetP()->y, seg2->GetQ()->x, seg2->GetQ()->y );
        /// TODO (klusi#2#): possible BUG : when collision point is an EPSILON further then two update points very near to each other (less then EPSILON) //
        /// NOTE (klusi#1#): note in previous line probably fixed by ordering //

        REAL orgx = X_ROTATED(cpt.ST, cpt.ND, -sinL, cosL);
        REAL orgy = Y_ROTATED(cpt.ST, cpt.ND, -sinL, cosL);
        if (IsPointInsideTheField(orgx, orgy))
        {
            out << " seg1 : " << seg1 << endl;
            out << " seg2 : " << seg2 << endl;

            result = new Point<REAL> (cpt.ST, cpt.ND, seg1->GetP(), seg2->GetP(), 0.0, 0.0, ++id, PT_Collision);
            result->org_x = orgx;
            result->org_y = orgy;

            out << "# NEW COLLISION POINT #> " << result << endl;
        }
    }
    return result;
}


template <class REAL>
void
PMF<REAL> :: InsertNewSegmentIntoSweep (Point<REAL> * parent, Segment<REAL> * nseg, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id, REAL sinL, REAL cosL)
{
    typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;

    //out << parent->x << endl;
    //out << parent->x + EPSILON << endl;
    //out << line->GetX0() << endl;
    out << " ... checking intersections" << endl;
    out << line << endl;
    pair<SweepIterator, bool> res = line->Insert(parent, nseg);
    if (! res.ND)
    {
        if (parent)
        out << "[  PARENT ] : " << parent << endl;
        out << "[ SEGMENT ] : " << nseg << endl;
        out << line << endl;
    }
    assert(res.ND == true);

    // check the segments above and below for intersections
    SweepIterator ita = line->Above(res.ST);
    SweepIterator itb = line->Below(res.ST);
    if (! line->IsNull(ita))
    {
        out << " --->  checking (a) : " << endl;
        Point<REAL> * cpt = DetectPossibleCollision (nseg, (*ita)->GetSegment(), id, sinL, cosL);
        if (cpt)
        {
            DeathEvent * de = new DeathEvent(cpt, nseg, (*ita)->GetSegment());
            out << " --->  cpt (a) : " << cpt << endl;
            evts->Insert(de);
        }
    }
    if (! line->IsNull(itb))
    {
        out << " --->  checking (b) : " << endl;
        Point<REAL> * cpt = DetectPossibleCollision (nseg, (*itb)->GetSegment(), id, sinL, cosL);
        if (cpt)
        {
            DeathEvent * de = new DeathEvent(cpt, nseg, (*itb)->GetSegment());
            out << " --->  cpt (b) : " << cpt << endl;
            evts->Insert(de);
        }
    }
    out << " ... intersections checked" << endl;
}


template <class REAL>
bool
PMF<REAL> :: ArrangeNewEvent (Point<REAL> * npt, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id, REAL sinL, REAL cosL, bool old = false)
{
    typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;
    using  namespace Geometry;

    Point<REAL> * parent = npt->n1;
    Segment<REAL> * nseg = new Segment<REAL>(parent, npt);

    REAL nx = X_ROTATED(npt->x, npt->y, -sinL, cosL);
    REAL ny = Y_ROTATED(npt->x, npt->y, -sinL, cosL);

    out.precision(12);
    out << " ---->  new point : " << npt << "  ( " << npt->org_x << " ; " << npt->org_y << " )" << endl;
    out << "       nx = " << nx << "   ;    ny = " << ny << endl;
    out << " ---->    parent  : " << parent << endl;

    // determine type of event (update or death)
    if (IsPointInsideTheField(nx, ny))
    //if (IsPointInsideTheField(npt->x, npt->y))
    {
        // npt inside the field
        npt->org_x = nx;
        npt->org_y = ny;

        // new event
        //out << " ---->  new point : " << npt << endl;
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
        out << "       px = " << px << "   ;    py = " << py << endl;

        // npt outside the field
        int up    = CheckIntersection2<REAL>(nx, ny, px, py, 0.0, 0.0, GetWidth(), 0.0);
        int down  = CheckIntersection2<REAL>(nx, ny, px, py, 0.0, GetHeight(), GetWidth(), GetHeight());
        int right = CheckIntersection2<REAL>(nx, ny, px, py, GetWidth(), 0.0, GetWidth(), GetHeight());
        int left  = CheckIntersection2<REAL>(nx, ny, px, py, 0.0, 0.0, 0.0, GetHeight());
        //int up    = CheckIntersection2<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, 0.0, GetWidth(), 0.0);
        //int down  = CheckIntersection2<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, GetHeight(), GetWidth(), GetHeight());
        //int right = CheckIntersection2<REAL>(npt->x, npt->y, parent->x, parent->y, GetWidth(), 0.0, GetWidth(), GetHeight());
        //int left  = CheckIntersection2<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, 0.0, 0.0, GetHeight());

        out << " up=" << up << "  ;   down=" << down << "  ;   right=" << right << "  ;   left=" << left << endl;
        /// FIXME (Rafel#1#): what if point is on border ?? //

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

        // new event
        //out << " ---->  new point : " << npt << endl;
        DeathEvent * e = new DeathEvent(npt, nseg);
        bool ans = evts->Insert(e);
        if (! ans)
        {
            out << " ---->  point is in events' queue : " << npt << endl;
            delete nseg;
            delete e;
            return false;
        }
    }
    out << "  old ? : " << (old ? "TRUE" : "FALSE") << endl;
    out << (old ? npt : parent) << endl;
    InsertNewSegmentIntoSweep ((old ? NULL : parent), nseg, evts, line, id, sinL, cosL);

    return true;
}
#undef X_ROTATED
#undef Y_ROTATED


template <class REAL>
inline
void
PMF<REAL> :: ProcessUpdateEvent (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id, REAL sinL, REAL cosL)
{
    using namespace Probability;
    using Geometry::IsZero;

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
        ///DetermineUpdateAngle<REAL> (newAngle);
        newAngle = Probability::PRNG->DetermineUpdateAngle();

        newAngle += angle;
        if (newAngle > M_PI_2)  newAngle -= M_PI;
        if (newAngle < -M_PI_2) newAngle += M_PI;

        out << " newAngle = " << newAngle << endl;
        if (pt->type == PT_BirthOnBorder)
        {
            ///while (IsZero(newAngle)) newAngle = Uniform<REAL>(EPSILON-M_PI_2, M_PI_2-EPSILON);
            while (IsZero(newAngle)) newAngle = Probability::PRNG->GetUniform(EPSILON-M_PI_2, M_PI_2-EPSILON);
            if (pt->y == 0.0  &&  newAngle < 0)  newAngle = -newAngle;
            if (pt->y == GetHeight()  &&  newAngle > 0)  newAngle = -newAngle;
        }
        out << " newAngle = " << newAngle << endl;

        int whichNeighbour = (pt->type == PT_Update) ? 2 : 1;
        ///Point<REAL> * newpt = pt->GenerateNeighbour(whichNeighbour, newAngle, id, Exp<REAL> (2.0));
        Point<REAL> * newpt = pt->GenerateNeighbour(whichNeighbour, newAngle, id, Probability::PRNG->GetExp(2.));
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
    PMFLogV("Correcting collision point of ends %i and %i", id1, id2);
    out << "  pt : " << pt << endl;
    out << "  n1 : " << pt->n1 << endl;
    out << "  n2 : " << pt->n2 << endl;
    if (pt->n1->n1 != NULL  &&  pt->n1->n1->id == id1)
    {
        PMFLogV("########### 1");
        pt->n1->n1 = pt;
        pt->l1 = pt->n1->l1;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id2)  { pt->n2->n1 = pt;  pt->l2 = pt->n2->l1; }
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id2)  { pt->n2->n2 = pt;  pt->l2 = pt->n2->l2; }
    }
    if (pt->n1->n2 != NULL  &&  pt->n1->n2->id == id1)
    {
        PMFLogV("########### 2");
        pt->n1->n2 = pt;
        pt->l1 = pt->n1->l2;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id2)  { pt->n2->n1 = pt;  pt->l2 = pt->n2->l1; }
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id2)  { pt->n2->n2 = pt;  pt->l2 = pt->n2->l2; }
    }
    if (pt->n1->n1 != NULL  &&  pt->n1->n1->id == id2)
    {
        PMFLogV("########### 3");
        pt->n1->n1 = pt;
        pt->l1 = pt->n1->l1;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id1)  { pt->n2->n1 = pt;  pt->l2 = pt->n2->l1; }
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id1)  { pt->n2->n2 = pt;  pt->l2 = pt->n2->l2; }
    }
    if (pt->n1->n2 != NULL  &&  pt->n1->n2->id == id2)
    {
        PMFLogV("########### 4");
        pt->n1->n2 = pt;
        pt->l1 = pt->n1->l2;
        if (pt->n2->n1 != NULL  &&  pt->n2->n1->id == id1)  { pt->n2->n1 = pt;  pt->l2 = pt->n2->l1; }
        if (pt->n2->n2 != NULL  &&  pt->n2->n2->id == id1)  { pt->n2->n2 = pt;  pt->l2 = pt->n2->l2; }
    }
    out << "  n1 : " << pt->n1 << endl;
    out << "  n2 : " << pt->n2 << endl;
}


template <class REAL>
inline
void
PMF<REAL> :: CheckIntersectionAfterDeath (Segment<REAL> * s1, Segment<REAL> * s2, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id, REAL sinL, REAL cosL)
{
    typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;
    //*
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
    // */
    //PMFLogV("Death with %i status neighbour(s) !!!", nn.size());
    if (nn.size() == 2)
    {
        Point<REAL> * cpt = DetectPossibleCollision (*nn.begin(), *nn.rbegin(), id, sinL, cosL);
        if (cpt)
        {
            DeathEvent * de = new DeathEvent(cpt, *nn.begin(), *nn.rbegin());
            evts->Insert(de);
        }
    }
}


template <class REAL>
inline
void
PMF<REAL> :: ProcessDeathEvent (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id, REAL sinL, REAL cosL)
{
    Segment<REAL> * s1, * s2;
    s1 = ev->GetSegment();
    s2 = ev->GetSegment(false);
    if (s1 && s2)
    {
        long id1 = s1->GetQ()->id;
        long id2 = s2->GetQ()->id;

        int wh1 = s1->GetP()->WhichNeighbourHasID (id1);
        int wh2 = s2->GetP()->WhichNeighbourHasID (id2);
        assert(wh1 > 0);
        assert(wh2 > 0);

        Point<REAL> * pt = ev->GetPoint();
        REAL len1, len2;

        if (wh1 == 1) { s1->GetP()->n1 = pt;  len1 = s1->GetP()->l1; }
        else  { s1->GetP()->n2 = pt;  len1 = s1->GetP()->l2; }

        if (wh2 == 1) { s2->GetP()->n1 = pt;  len2 = s2->GetP()->l1; }
        else  { s2->GetP()->n2 = pt;  len2 = s2->GetP()->l2; }

        int wh0 = pt->WhichNeighbourHasID (s1->GetP()->id);
        assert(wh0 > 0);
        if (wh0 == 2) std::swap(len1, len2);

        pt->l1 = len1;
        pt->l2 = len2;

        //CorrectCollisionStartPoints (ev->GetPoint(), id1, id2);
    }
    CheckIntersectionAfterDeath (s1, s2, evts, line, id, sinL, cosL);
    line->Erase( s1 );
    if (s2) line->Erase( s2 );  else  delete s1;
    return;
}


#endif // COMMON_HPP_INCLUDED
