#include "DetectorEvent.hpp"

DetectorEvent::DetectorEvent(POINT * pt, SEGMENT * sg, DetectorEventType type)
: _pt(pt), _sg(sg), _type(type)
{
    //ctor
    OnInit();
}

DetectorEvent::~DetectorEvent()
{
    //dtor
}

void
DetectorEvent::OnInit()
{
}
