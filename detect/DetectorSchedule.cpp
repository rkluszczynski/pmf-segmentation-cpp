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
    if (it == _events.end())
    {
        _events.insert(e).second;
    }
    else
    {
        (*it)->AddSegment(e->GetSegment());
        assert((*it)->GetSegmentsSize() <= 2);
    }
    return true;
}

void
DetectorSchedule::Erase(EventPoint e)
{
    Iterator it = _events.find(e);
    assert(it != _events.end());

    if ((*it)->GetSegmentsSize() > 1)
    {
        assert((*it)->GetSegmentsSize() <= 2);
        (*it)->EraseSegment(e->GetSegment());
        assert((*it)->GetSegmentsSize() <= 1);
    }
    else
    {
        _events.erase(it);
        //assert(_erased > 0);
    }
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
