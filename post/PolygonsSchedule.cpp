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
PolygonsSchedule::Insert(POINT pt)
{
    _events.insert(pt);
    return true;
}


void
PolygonsSchedule::Erase(POINT pt)
{
    _events.erase(pt);
}


std::ostream & operator << (std::ostream & out, const PolygonsSchedule & evt)
{
    FOREACH(it, evt._events)  out << " ==> " << &(**it) << endl;
    return out;
}

