#include "DetectorSchedule.hpp"

#include <cassert>

#define VAR(V,N)        __typeof(N) V = (N)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)

DetectorSchedule::DetectorSchedule()
{
    //ctor
}

DetectorSchedule::~DetectorSchedule()
{
    //dtor
}

bool
DetectorSchedule::Insert(EventPoint e)
{
    Iterator it = _events.find(e);
    bool res = _events.insert(e).second;
    assert(res and "NEW EVENT ADDED");
    return res;
}

void
DetectorSchedule::Erase(EventPoint e)
{
    long _erased = _events.erase(e);
    assert(_erased > 0);
    delete e;
}

std::ostream & operator << (std::ostream & out, const DetectorSchedule * ds)
{
    out << "[ EVENTS ] :>" << std::endl;
    //*
    FOREACH(it, ds->_events)  {
        DetectorSchedule::POINT * pt = (*it)->GetPoint();
        out << " -[" << (*it)->GetType() << "]->  " << pt << std::endl;
    }
    // */
    return out;
}
