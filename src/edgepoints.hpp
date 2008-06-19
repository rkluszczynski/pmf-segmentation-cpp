#ifndef EDGEPOINTS_HPP_INCLUDED
#define EDGEPOINTS_HPP_INCLUDED


template <class T_REAL>
class EdgePoints
{
    private :
        long PointsNumber = 0;

    public :
        EdgePoints();
        virtual ~EdgePoints();

        bool LoadData(const char *);

};


#include "edgepoints.cpp"


#endif // EDGEPOINTS_HPP_INCLUDED
