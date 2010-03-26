#include "ScheduleEvent.hpp"

#define VAR(V,N)        __typeof(N) V = (N)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)


ScheduleEvent::ScheduleEvent(POINT * pt, SEGMENT * sg, DetectorEventType type)
: _pt(pt), _type(type)
{
    //ctor
    _segs.push_back(sg);
    OnInit();
}


ScheduleEvent::~ScheduleEvent()
{
    //dtor
}

void
ScheduleEvent::OnInit()
{
}


std::ostream & operator << (std::ostream & out, const ScheduleEvent * evt)
{
    out << ((evt->_type == BeginSegment) ? " {BeginSegment} : " : " {EndOfSegment} : ");
    out << *evt->_pt;
    FOREACH(it, evt->_segs)
        out << " , " << **it;//evt->_segs[0];
    return out;
}

