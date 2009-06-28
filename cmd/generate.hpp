#ifndef GENERATE_HPP_INCLUDED
#define GENERATE_HPP_INCLUDED

#include "../cmd/point.hpp"
#include "../cmd/geometry.hpp"
#include "../cmd/probability.hpp"
#include "../cmd/schedule.hpp"


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
void
PMF<REAL> :: ArrangeNewEvent (Point<REAL> * npt, EventsSchedule<REAL> * evts, long & id)
{
    using namespace Geometry;

    Point<REAL> * parent = npt->n1;

    if ( 0.0 < npt->x  &&  npt->x < GetWidth()  &&  0.0 < npt->y  &&  npt->y < GetHeight() )
    {
        // npt inside the field

        // new event
        UpdateEvent * e = new UpdateEvent(npt);
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
        DeathEvent * e = new DeathEvent(npt);
        evts->Insert(e);
    }
    return;
}



template <class REAL>
inline
void
PMF<REAL> :: ProcessBirthEvent (Event * ev, EventsSchedule<REAL> * evts, long & id)
{
    using namespace Probability;

    if (ev->GetPoint()->type == PT_BirthOnBorder)  {  ProcessUpdateEvent(ev, evts, id); return; }

    Point<REAL> * pt = ev->GetPoint();
    REAL upperLength = Exp<REAL>(2.0);
    REAL lowerLength = Exp<REAL>(2.0);
    REAL upperAngle, lowerAngle;
    DetermineBirthAngles (upperAngle, lowerAngle);

    Point<REAL> * newpt1 = pt->GenerateNeighbour(1, upperAngle, id, upperLength);
    //pmf_store_points_in_blocks (newpt1, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);
    ArrangeNewEvent(newpt1, evts, id);

    Point<REAL> * newpt2 = pt->GenerateNeighbour(2, lowerAngle, id, lowerLength);
    //pmf_store_points_in_blocks (newpt2, birthList, crossList, pt, id, fieldHeight, fieldWidth, blocksLists);
    ArrangeNewEvent(newpt2, evts, id);

    return;
}



template <class REAL>
inline
void
PMF<REAL> :: ProcessUpdateEvent (Event * ev, EventsSchedule<REAL> * evts, long & id)
{
    using namespace Probability;

    Point<REAL> * pt = ev->GetPoint();
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
    ArrangeNewEvent(newpt, evts, id);

    return;
}



template <class REAL>
inline
void
PMF<REAL> :: ProcessDeathEvent (Event * ev, EventsSchedule<REAL> * evts, long & id)
{
    /*
            pmf_correct_intersection_point (pt, id1, id2);
            crossList->remove_intersection_with_one_id_of (id1, id2, blocksLists);
            birthList->remove_point_with_id (id1, blocksLists);
            birthList->remove_point_with_id (id2, blocksLists);
    //*/
    return;
}



template <class REAL>
void
PMF<REAL> :: GenerateField ()
{
    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>();

    long id = GenerateInitialBirths(evts);
    while (! evts->IsEmpty())
    {
        cout << endl << evts << endl;

        Event * evt = evts->SeeFirst();
        if(evt->GetType() != BIRTH)  CheckNewBirthSite(evt, evts, id);

        evt = evts->SeeFirst();
        cf->PushBack(evt->GetPoint());

        switch (evt->GetType())
        {
            case BIRTH :
                    PMFLogV("-> BIRTH EVENT");
                    ProcessBirthEvent(evt, evts, id);
                    break;;
            case UPDATE :
                    PMFLogV("-> UPDATE EVENT");
                    ProcessUpdateEvent(evt, evts, id);
                    break;;
            case DEATH :
                    PMFLogV("-> DEATH EVENT");
                    ProcessDeathEvent(evt, evts, id);
                    break;;
            default :
                    assert(false);
        }
        evts->Erase(evt);
    }
    //cf->ShowConfiguration(cout);
    cf->SaveToFile("output/gen-cmd.txt");

    delete evts;
    return;
}


#endif // GENERATE_HPP_INCLUDED
