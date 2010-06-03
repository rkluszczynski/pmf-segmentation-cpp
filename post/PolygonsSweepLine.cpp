#include "PolygonsSweepLine.h"

#include <climits>


PolygonsSweepLine::PolygonsSweepLine(double epsilon)
: _st(PolygonsSweepLineComparator(this)), _x0(-std::numeric_limits<double>::max()), _epsilon(epsilon)
{
    //ctor
}

PolygonsSweepLine::~PolygonsSweepLine()
{
    //dtor
}
