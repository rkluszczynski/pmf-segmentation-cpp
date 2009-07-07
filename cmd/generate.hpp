#ifndef GENERATE_HPP_INCLUDED
#define GENERATE_HPP_INCLUDED

#include "../cmd/point.hpp"
#include "../cmd/geometry.hpp"
#include "../cmd/probability.hpp"
#include "../cmd/schedule.hpp"
#include <wx/string.h>


template <class REAL>
inline
long
PMF<REAL> :: GenerateInitialBirths (EventsSchedule<REAL> * evts)
{
    using Probability::Exp;
    using Probability::Uniform;

    REAL halfPI = M_PI_2;
    REAL fieldHeight = GetHeight();
    REAL fieldWidth  = GetWidth();
    long id = 0;

    // Generating left birth sites ...
    PMFLogV("[ INFO ] : Generating boundary left birth sites ... ");
    REAL vertical = Exp<REAL>(2.0);
    while (vertical < fieldHeight) {
        //REAL angle = Uniform<REAL> ( -polowaPI, polowaPI);
        Point<REAL> * pt = new Point<REAL>(0.0, vertical, 0.0, 0.0, ++id, PT_BirthOnBorder);
        evts->InsertBirthEvent(pt);
        vertical += Exp<REAL>(2.0);
    }

    // Generating upper birth sites ...
    PMFLogV("[ INFO ] : Generating upper birth sites ...  ");
    REAL horizontal = Exp<REAL>(2.0);
    while (horizontal <= fieldWidth) {
        REAL angle = Uniform<REAL> ( -halfPI, halfPI);
        if (angle < 0.0) {
            Point<REAL> * pt = new Point<REAL>(horizontal, 0.0, 0.0, 0.0, ++id, PT_BirthOnBorder);
            evts->InsertBirthEvent(pt);
        }
        horizontal += Exp<REAL>(2.0);
    }

    // Generating lower birth sites ...
    PMFLogV("[ INFO ] : Generating lower birth sites ...  ");
    horizontal = Exp<REAL>(2.0);
    while (horizontal <= fieldWidth) {
        REAL angle = Uniform<REAL> ( -halfPI, halfPI);
        if (angle > 0.0) {
            Point<REAL> * pt = new Point<REAL>(horizontal, fieldWidth, 0.0, 0.0, ++id, PT_BirthOnBorder);
            evts->InsertBirthEvent(pt);
        }
        horizontal += Exp<REAL>(2.0);
    }
    return id;
}



template <class REAL>
inline
void
PMF<REAL> :: CheckNewBirthSite (Event * ev, EventsSchedule<REAL> * evts, long & id)
{
    using namespace Probability;

    if (cf->IsEmpty()) return;

    REAL offset = Exp<REAL>(GetHeight() * M_PI);
    Point<REAL> * prev = cf->SeeLastPoint();
    Point<REAL> * pt = ev->GetPoint();
    if (prev->x + offset < pt->x)
    {
        REAL tmpY = Uniform<REAL> (0.0, GetHeight());
        Point<REAL> * newPt = new Point<REAL>(prev->x + offset, tmpY, 0.0, 0.0, ++id, PT_BirthInField);
        evts->InsertBirthEvent(newPt);
    }
    return;
}


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
    int collision = CheckIntersection<REAL>( seg1->GetP()->x, seg1->GetP()->y, seg1->GetQ()->x, seg1->GetQ()->y,
                                             seg2->GetP()->x, seg2->GetP()->y, seg2->GetQ()->x, seg2->GetQ()->y );
    if (collision > 0)
    {
        pair<REAL, REAL> cpt = CalculateIntersection<REAL> (
                                            seg1->GetP()->x, seg1->GetP()->y, seg1->GetQ()->x, seg1->GetQ()->y,
                                            seg2->GetP()->x, seg2->GetP()->y, seg2->GetQ()->x, seg2->GetQ()->y );
/// FIXME (klusi#2#): possible BUG : when collision point is an EPSILON further then two update points very near to each other (less then EPSILON)
        if (IsPointInsideTheField(cpt.ST, cpt.ND))
            result = new Point<REAL> (cpt.ST, cpt.ND, seg1->GetP(), seg2->GetP(), 0.0, 0.0, ++id, PT_Collision);
    }
    return result;
}


template <class REAL>
inline
void
PMF<REAL> :: ArrangeNewEvent (Point<REAL> * npt, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id)
{
    typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;
    using  namespace Geometry;

    Point<REAL> * parent = npt->n1;
    Segment<REAL> * nseg = new Segment<REAL>(parent, npt);

    // determine type of event (update or death)
    if (IsPointInsideTheField(npt->x, npt->y))
    {
        // npt inside the field
        // new event
        UpdateEvent * e = new UpdateEvent(npt, nseg);
        evts->Insert(e);
    }
    else {
        // npt outside the field
        int up    = CheckIntersection<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, 0.0, GetWidth(), 0.0);
        int down  = CheckIntersection<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, GetHeight(), GetWidth(), GetHeight());
        int right = CheckIntersection<REAL>(npt->x, npt->y, parent->x, parent->y, GetWidth(), 0.0, GetWidth(), GetHeight());
        int left  = CheckIntersection<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, 0.0, 0.0, GetHeight());

        /// FIXME (Rafel#1#): what if point is on border ??

        REAL cx, cy;
        if (up != 0  &&  up != 5) {
            CalculateIntersection<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, 0.0, GetWidth(), 0.0, cx, cy);
        }
        else if (down != 0  &&  down != 5) {
            CalculateIntersection<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, GetHeight(), GetWidth(), GetHeight(), cx, cy);
        }
        else if (right != 0  &&  right != 5) {
            CalculateIntersection<REAL>(npt->x, npt->y, parent->x, parent->y, GetWidth(), 0.0, GetWidth(), GetHeight(), cx, cy);
        }
        else if (left != 0  &&  left != 5) {
            CalculateIntersection<REAL>(npt->x, npt->y, parent->x, parent->y, 0.0, 0.0, 0.0, GetHeight(), cx, cy);
        }
        else assert(false);

        npt->x = cx;
        npt->y = cy;
        npt->n2 = NULL;
        npt->l2 = 0.0;
        npt->type = PT_DeathOnBorder;

        // new event
        DeathEvent * e = new DeathEvent(npt, nseg);
        evts->Insert(e);
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

    return;
}


template <class REAL>
inline
void
PMF<REAL> :: ProcessBirthEvent (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id)
{
    using namespace Probability;

    //if (ev->GetPoint()->type == PT_BirthOnBorder)  {  ProcessUpdateEvent(ev, evts, id); return; }

    Point<REAL> * pt = ev->GetPoint();
    REAL upperLength = Exp<REAL>(2.0);
    REAL lowerLength = Exp<REAL>(2.0);
    REAL upperAngle, lowerAngle;
    DetermineBirthAngles (upperAngle, lowerAngle);

    Point<REAL> * newpt1 = pt->GenerateNeighbour(1, upperAngle, id, upperLength);
    //pmf_store_points_in_blocks (newpt1, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);
    ArrangeNewEvent(newpt1, evts, line, id);

    Point<REAL> * newpt2 = pt->GenerateNeighbour(2, lowerAngle, id, lowerLength);
    //pmf_store_points_in_blocks (newpt2, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);
    ArrangeNewEvent(newpt2, evts, line, id);

    return;
}



template <class REAL>
inline
void
PMF<REAL> :: ProcessUpdateEvent (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id)
{
    using namespace Probability;

    Point<REAL> * pt = ev->GetPoint();
    Segment<REAL> * seg = ev->GetSegment();
    if (seg)
    {
        cout << endl << " ##################### " << endl << line << endl;
        line->Erase(seg);
        delete seg;
    }

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
    //pmf_store_points_in_blocks (newPt, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);
    cout << " -> new point  :  " << newpt << endl;
    ArrangeNewEvent(newpt, evts, line, id);

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
PMF<REAL> :: ProcessDeathEvent (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, long & id)
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
    /*
            pmf_correct_intersection_point (pt, id1, id2);
            crossList->remove_intersection_with_one_id_of (id1, id2, blocksLists);
            birthList->remove_point_with_id (id1, blocksLists);
            birthList->remove_point_with_id (id2, blocksLists);
    //*/
    wxLogMessage( wxString::Format(_("Death with %i status neighbour(s) !!!"), nn.size()) );
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
    //delete s1;
    if (s2)
    {
        line->Erase( s2 );
        //delete s2;
    }
    else {
        delete s1;
    }
    return;
}


template <class REAL>
void
PMF<REAL> :: GenerateField ()
{
    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>();
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>();

    long id = GenerateInitialBirths(evts);
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

        if (evt->GetType() != NormalBirth  &&  evt->GetType() != BorderBirth)  CheckNewBirthSite(evt, evts, id);
        evt = evts->SeeFirst();
        cf->PushBack(evt->GetPoint());

        //cout << endl;
        //cout << evts << endl;
        cout << "... event at point " << evt->GetPoint() << endl;
        //cout << line << endl;

        switch (evt->GetType())
        {
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
    //cf->ShowConfiguration(cout);
    cf->SaveToFile("output/gen-cmd.txt");

    delete line;
    delete evts;
    return;
}
#undef __CONTINUE_BLOCK


#endif // GENERATE_HPP_INCLUDED
