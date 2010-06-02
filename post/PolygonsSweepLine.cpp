#include "PolygonsSweepLine.h"

#include <climits>


PolygonsSweepLine::PolygonsSweepLine() : _st(PolygonsSweepLineComparator(this)), _x0(-std::numeric_limits<double>::max())
{
    //ctor
}

PolygonsSweepLine::~PolygonsSweepLine()
{
    //dtor
}
