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
    using namespace Geometry;

    if (cf->IsEmpty()) return;

    REAL offset = Exp<REAL>(GetHeight() * M_PI);
    if (offset < EPSILON)  offset = 2. * EPSILON;
    Point<REAL> * prev = cf->SeeLastPoint();
    Point<REAL> * pt = ev->GetPoint();
    if (prev->x + offset < pt->x)
    {
        REAL tmpY = Uniform<REAL> (0.0, GetHeight());
        if (IsZero(GetHeight() - tmpY)) tmpY = GetHeight() - 2. * EPSILON;
        else if (IsZero(tmpY)) tmpY = 2. * EPSILON;

        Point<REAL> * newPt = new Point<REAL>(prev->x + offset, tmpY, 0.0, 0.0, ++id, PT_BirthInField);
        evts->InsertBirthEvent(newPt);
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
                    ProcessBirthEvent(evt, evts, line, id, 0., 1.);
                    break;;
            case BorderBirth :
                    PMFLogV("-> BIRTH EVENT");
            case PointUpdate :
                    PMFLogV("-> UPDATE EVENT");
                    ProcessUpdateEvent(evt, evts, line, id, 0., 1.);
                    break;;
            case NormalDeath :
            case BorderDeath :
                    PMFLogV("-> DEATH EVENT");
                    ProcessDeathEvent(evt, evts, line, id, 0., 1.);
                    break;;
            default :
                    assert(false);
        }
        evts->Erase(evt);
        cout << "-------------------------------------------------" << endl;
    }
    delete line;
    delete evts;
    return;
}


#endif // GENERATE_HPP_INCLUDED
