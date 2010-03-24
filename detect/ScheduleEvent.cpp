#include "ScheduleEvent.hpp"

ScheduleEvent::ScheduleEvent(POINT * pt, SEGMENT * sg, DetectorEventType type)
: _pt(pt), _sg(sg), _type(type)
{
    //ctor
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
    //out << evt->_sg;
    return out;
}

