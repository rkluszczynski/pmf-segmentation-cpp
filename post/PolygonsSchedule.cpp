#include "PolygonsSchedule.h"

#define VAR(V,N)        __typeof(N) V = (N)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)


PolygonsSchedule::PolygonsSchedule()
{
    //ctor
}

PolygonsSchedule::~PolygonsSchedule()
{
    //dtor
}


bool
PolygonsSchedule::Insert(POINT pt, SEGMENT s1, SEGMENT s2 = NULL)
{
    switch (pt->type)
    {
        case pmf::PT_BirthInField:
                                _events.insert(new PolygonsBeginEvent(pt, s1, s2));
                                break;;
        case pmf::PT_BirthOnBorder:
                                _events.insert(new PolygonsBorderBeginEvent(pt, s1));
                                break;;
        case pmf::PT_Update:
                                _events.insert(new PolygonsUpdateEvent(pt, s1, s2));
                                break;;
        case pmf::PT_Collision:
                                _events.insert(new PolygonsEndEvent(pt, s1, s2));
                                break;;
        case pmf::PT_DeathOnBorder:
                                _events.insert(new PolygonsBorderEndEvent(pt, s1));
                                break;;
        default :
                    assert("ZLE ZLE ZLE" and false);
    }
    return true;
}


void
PolygonsSchedule::Erase(Event evt)
{
    _events.erase(evt);
    delete evt;
}


std::ostream & operator << (std::ostream & out, const PolygonsSchedule & evt)
{
    out << "[ PolygonsSchedule ] : " << endl;
    FOREACH(it, evt._events)  out << " ::> " << (*it)->GetPoint()->id << endl;
    return out;
}

