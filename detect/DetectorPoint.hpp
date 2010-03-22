#ifndef DETECTORPOINT_HPP_INCLUDED
#define DETECTORPOINT_HPP_INCLUDED

#include <iostream>


template <typename REAL>
struct DetectorPoint
{
    REAL _x, _y;

    DetectorPoint (REAL xx, REAL yy) : _x(xx), _y(yy)  { OnInit(); }
    virtual ~DetectorPoint()  { }

    inline REAL x() const { return _x; }
    inline REAL y() const { return _y; }

    void OnInit()  { }

    bool operator < (const DetectorPoint<REAL> * & pt) const { return (x() < pt->x()); }

    friend std::ostream& operator << (std::ostream& out, const DetectorPoint<REAL> * pt)
    {
        //++pmf_point_counter; // To log copy constructor
        out <<  "P" << "=";
        out.precision(20);
        out << "(" << pt->x() << "," << pt->y() << ")";
        return out;
    }
};


#endif // DETECTORPOINT_HPP_INCLUDED
