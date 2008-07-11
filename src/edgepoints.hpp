#ifndef EDGEPOINTS_HPP_INCLUDED
#define EDGEPOINTS_HPP_INCLUDED

#include <iostream>
#include <vector>
#include "point.hpp"


template <class T_REAL>
class EdgePoints
{
    private :
        long PointsNumber = 0;
        T_REAL size_x, size_y;
        int angleType;

        std::vector<pmf_point *> PointsVector;

    public :
        EdgePoints();
        virtual ~EdgePoints();

        bool LoadData(const char *);

};


#include "edgepoints.cpp"


#endif // EDGEPOINTS_HPP_INCLUDED
