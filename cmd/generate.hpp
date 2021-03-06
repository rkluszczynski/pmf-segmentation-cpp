#ifndef GENERATE_HPP_INCLUDED
#define GENERATE_HPP_INCLUDED

#include "../cmd/point.hpp"
#include "../cmd/geometry.hpp"
#include "../cmd/probability.hpp"
#include "../cmd/schedule.hpp"
#include <wx/string.h>
#include <wx/log.h>


template <class REAL>
inline
long
PMF<REAL> :: GenerateInitialBirths (EventsSchedule<REAL> * evts)
{
    using Probability::_Exp;
    using Probability::_Uniform;

    REAL halfPI = M_PI_2;
    REAL fieldHeight = GetHeight();
    REAL fieldWidth  = GetWidth();
    long id = 0;

    // Generating left birth sites ...
    PMFLogV("[ INFO ] : Generating boundary left birth sites ... ");
    ///REAL vertical = Exp<REAL>(2.0);
    REAL vertical = GetPRNG()->GetExp(2.);
    while (vertical < fieldHeight) {
        //REAL angle = Uniform<REAL> ( -polowaPI, polowaPI);
        Point<REAL> * pt = new Point<REAL>(0.0, vertical, 0.0, 0.0, ++id, PT_BirthOnBorder);
        evts->InsertBirthEvent(pt);
        vertical += GetPRNG()->GetExp(2.);
    }

    // Generating upper birth sites ...
    PMFLogV("[ INFO ] : Generating upper birth sites ...  ");
    REAL horizontal = GetPRNG()->GetExp(2.0);
    while (horizontal <= fieldWidth) {
        REAL angle = GetPRNG()->GetUniform( -halfPI, halfPI);
        if (angle < 0.0) {
            Point<REAL> * pt = new Point<REAL>(horizontal, 0.0, 0.0, 0.0, ++id, PT_BirthOnBorder);
            evts->InsertBirthEvent(pt);
        }
        horizontal += GetPRNG()->GetExp(2.0);
    }

    // Generating lower birth sites ...
    PMFLogV("[ INFO ] : Generating lower birth sites ...  ");
    horizontal = GetPRNG()->GetExp(2.0);
    while (horizontal <= fieldWidth) {
        ///REAL angle = Uniform<REAL> ( -halfPI, halfPI);
        REAL angle = GetPRNG()->GetUniform ( -halfPI, halfPI);
        if (angle > 0.0) {
            Point<REAL> * pt = new Point<REAL>(horizontal, fieldWidth, 0.0, 0.0, ++id, PT_BirthOnBorder);
            evts->InsertBirthEvent(pt);
        }
        ///horizontal += Exp<REAL>(2.0);
        horizontal += GetPRNG()->GetExp(2.0);
    }
    return id;
}



template <class REAL>
inline
void
PMF<REAL> :: CheckNewBirthSite (Event * ev, EventsSchedule<REAL> * evts, long & id)
{
    using namespace Probability;
    using namespace Geometry;

    if (cf->IsEmpty()) return;

    REAL offset = GetPRNG()->GetExp(GetHeight() * M_PI);
    REAL epsilon = nparams.GetAxisEpsilon();
    if (offset < epsilon)  offset = 2. * epsilon;
    Point<REAL> * prev = cf->SeeLastPoint();
    Point<REAL> * pt = ev->GetPoint();
    if (prev->x + offset < pt->x)
    {
        REAL tmpY = GetPRNG()->GetUniform (0.0, GetHeight());
        if (IsZero(GetHeight() - tmpY, epsilon)) tmpY = GetHeight() - 2. * epsilon;
        else if (IsZero(tmpY, epsilon)) tmpY = 2. * epsilon;

        Point<REAL> * newPt = new Point<REAL>(prev->x + offset, tmpY, 0.0, 0.0, ++id, PT_BirthInField);
        evts->InsertBirthEvent(newPt);
    }
    return;
}


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
        REAL upperLength = GetPRNG()->GetExp(2.0);
        REAL lowerLength = GetPRNG()->GetExp(2.0);
        REAL upperAngle, lowerAngle;
        GetPRNG()->DetermineBirthAngles (upperAngle, lowerAngle);

        if (! newpt1)
        {
            newpt1 = pt->GenerateNeighbour(GetPRNG(), 1, upperAngle, id, upperLength);
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
            newpt2 = pt->GenerateNeighbour(GetPRNG(), 2, lowerAngle, id, lowerLength);
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
bool
PMF<REAL> :: IsTheEventInvalid (Event * ev, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line )
{
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
        if ( (! line->HasSegmentWithEndId(ev->GetSegment(true)->GetQ()->id))
            || (! line->HasSegmentWithEndId(ev->GetSegment(false)->GetQ()->id)) )
        {
            Point<REAL> * tmp = ev->GetPoint();
            evts->Erase(ev);
            delete tmp;
            return true;
        }
    }
    return false;
}


template <class REAL>
void
PMF<REAL> :: GenerateField ()
{
    EventsSchedule<REAL> * evts = new EventsSchedule<REAL>(nparams.GetAxisEpsilon());
    SweepLineStatus<REAL> * line = new SweepLineStatus<REAL>(nparams);

    long id = GenerateInitialBirths(evts);
    while (! evts->IsEmpty())
    {
        //out << endl << "_________________________________________________" << endl;
        Event * evt = evts->SeeFirst();
        /*
        out << endl;
        out << evts << endl;
        out << "... event at point " << evt->GetPoint() << endl;
        out << line << endl;
        //*/
        if (IsTheEventInvalid(evt, evts, line)) continue;

        if (evt->GetType() != NormalBirth  &&  evt->GetType() != BorderBirth)  CheckNewBirthSite(evt, evts, id);
        evt = evts->SeeFirst();
        cf->PushBack(evt->GetPoint());
        /*
        out << "... event at point " << evt->GetPoint() << endl;
        //*/
        switch (evt->GetType())
        {
            case NormalBirth :
                    ProcessBirthEvent(evt, evts, line, id, 0., 1.);
                    break;;
            case BorderBirth :
            case PointUpdate :
                    ProcessUpdateEvent(evt, evts, line, id, 0., 1.);
                    break;;
            case NormalDeath :
            case BorderDeath :
                    ProcessDeathEvent(evt, evts, line, id, 0., 1.);
                    break;;
            default :
                    assert("WRONG EVENT TYPE DURING GENERATE METHOD" && false);
        }
        evts->Erase(evt);
        //out << "-------------------------------------------------" << endl;
    }
    delete line;
    delete evts;
    return;
}


#endif // GENERATE_HPP_INCLUDED
