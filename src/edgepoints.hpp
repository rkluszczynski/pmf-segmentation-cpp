#ifndef EDGEPOINTS_HPP_INCLUDED
#define EDGEPOINTS_HPP_INCLUDED

#include <iostream>
#include <vector>
#include "point.hpp"


template <class T_REAL>
class EdgePoints
{
    private :
        //long PointsNumber;
        T_REAL size_x, size_y;
        int angleType;

        std::vector<pmf_point<T_REAL> *> PointsVector;
        std::vector<T_REAL> xx;
        std::vector<T_REAL> yy;
        std::vector<T_REAL> aa;


        inline long getPointsNumber() { return PointsVector.size(); }


    public :
        EdgePoints();
        virtual ~EdgePoints();

        bool LoadData(const char *);
        void PrintData(std::ostream &);

};


#include "edgepoints.cpp"


#endif // EDGEPOINTS_HPP_INCLUDED
