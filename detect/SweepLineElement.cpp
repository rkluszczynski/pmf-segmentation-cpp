#include "SweepLineElement.hpp"

#include <cassert>


SweepLineElement::SweepLineElement(const REAL & x, SEGMENT * s)
: _x0(x), _seg(s)
{
    //ctor
    OnInit();
    _y0 = CalculateLinearFunctionValue(_seg, _x0);
}


SweepLineElement::~SweepLineElement()
{
    //dtor
}


SweepLineElement::REAL
SweepLineElement::CalculateLinearFunctionValue (SEGMENT * s, REAL x) const
{
    //*
    if (s->isVertical())  return s->GetBeginPoint()->y();
    assert(! s->isVertical());
    // */
    REAL _slope = s->slope();
    REAL _b = s->GetBeginPoint()->y() - _slope * s->GetBeginPoint()->x();
    return _slope * x + _b;
}


void
SweepLineElement::OnInit()
{
}

std::ostream & operator << (std::ostream & out, const SweepLineElement & el)
{
    out << "{ " << *el.GetSegment() << " }";
    out.precision(22);
    //out << "\t ~ \t" << el.GetX0() << " , " << el.GetY0() << " ; " << el.yy0(el.GetX0()) << " }";
    return out;
}

