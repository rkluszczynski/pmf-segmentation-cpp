#include "DetectorSweepLine.hpp"

#define VAR(V,N)        __typeof(N) V = (N)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)


DetectorSweepLine::DetectorSweepLine()
{
    //ctor
    slec = new SweepLineEntryComparator(*this);
    _st = new STATUS(*slec);
}

DetectorSweepLine::~DetectorSweepLine()
{
    //dtor
    FOREACH(it, (*_st)) delete *it;
    delete _st;
    delete slec;
}





std::ostream & operator << (std::ostream & out, const DetectorSweepLine * dsl)
{
    out << "[ STATUS SEGMENTS ] : at position " << dsl->_x0 << std::endl;
    FOREACH(it, (*dsl)) out << " " << *(*it) << std::endl;
    out << "\\__________________ : left at position " << dsl->_x0 << std::endl;
    out << std::endl;
    return out;
}


bool
DetectorSweepLine::BelowComparator(ENTRY * const & e1, ENTRY * const & e2) const
{
    //SEGMENT * s1 = e1->GetSegment();
    //SEGMENT * s2 = e2->GetSegment();

    double y1 = e1->yy0(_x0);
    double y2 = e2->yy0(_x0);

    bool res = (y1 < y2);
    return res;
}
