#include "SweepLineElement.hpp"


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
    /*
    if (s->isVertical())  return s->GetP()->y;
    assert(! s->isVertical());
    // */
    REAL _slope = s->slope();
    REAL _b = s->GetBeginPoint()->y() - _slope * s->GetEndPoint()->x();
    return _slope * x + _b;
}


void
SweepLineElement::OnInit()
{
}

std::ostream & operator << (std::ostream & out, const SweepLineElement & el)
{
    out << "{ " << el.GetSegment() << "\t ~ \t" << el.GetX0() << " , " << el.GetY0() << " ; " << el.yy0(el.GetX0()) << " }";
    //out << "{" << el->GetSegment() << "}";
    return out;
}

