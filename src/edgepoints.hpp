#ifndef EDGEPOINTS_HPP_INCLUDED
#define EDGEPOINTS_HPP_INCLUDED

#include <iostream>
#include <vector>
#include "point.hpp"
#include "edgeelement.hpp"


template <class T_REAL>
class EdgePoints
{
    private :
        //long PointsNumber;
        T_REAL size_x, size_y;
        int angleType;

        std::vector<edge_element<T_REAL> *> PointsVector;


    public :
        EdgePoints();
        virtual ~EdgePoints();

        bool LoadData(const char *);
        void PrintData(std::ostream &);

        int getPointIndex(T_REAL, T_REAL);
        long hasPointWithID(long);

        inline long getPointsNumber() { return PointsVector.size(); }
        inline edge_element<T_REAL> * get(int i) { return PointsVector[i]; }

};


#include "edgepoints.cpp"


#endif // EDGEPOINTS_HPP_INCLUDED
