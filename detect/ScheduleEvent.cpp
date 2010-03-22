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
