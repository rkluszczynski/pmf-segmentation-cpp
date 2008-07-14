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


        inline long getPointsNumber() { return PointsVector.size(); }


    public :
        EdgePoints();
        virtual ~EdgePoints();

        bool LoadData(const char *);
        void PrintData(std::ostream &);

};


#include "edgepoints.cpp"


#endif // EDGEPOINTS_HPP_INCLUDED
