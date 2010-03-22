#ifndef DETECTORPOINT_HPP_INCLUDED
#define DETECTORPOINT_HPP_INCLUDED

#include <iostream>


template <typename REAL>
struct DetectorPoint
{
    REAL x, y;

    DetectorPoint (REAL xx, REAL yy) : x(xx), y(yy)  { OnInit(); }
    virtual ~DetectorPoint()  { }

    void OnInit()  { }

    bool operator < (const DetectorPoint<REAL> * & pt) const { return (x < pt->x); }

    friend std::ostream& operator << (std::ostream& out, const DetectorPoint<REAL> * pt)
    {
        //++pmf_point_counter; // To log copy constructor
        out <<  "P" << "=";
        out.precision(20);
        out << "(" << pt->x << "," << pt->y << ")";
        return out;
    }
};


#endif // DETECTORPOINT_HPP_INCLUDED
