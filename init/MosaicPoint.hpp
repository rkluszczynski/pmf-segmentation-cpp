#ifndef MOSAICPOINT_HPP_INCLUDED
#define MOSAICPOINT_HPP_INCLUDED

#include <vector>

using namespace std;


    template <typename REAL> struct MosaicPoint
    {
        REAL x, y;
        vector<MosaicPoint<REAL> *> n;


        MosaicPoint (REAL xx, REAL yy) : x(xx), y(yy)  { OnInit(); }

        virtual ~MosaicPoint()  { }

        void OnInit()  { }

        bool operator< (const MosaicPoint<REAL> * & point) const { return(x < point->x); }

        friend std::ostream& operator << (std::ostream& out, const MosaicPoint<REAL> * pt)
        {
            //++pmf_point_counter; // To log copy constructor
            out <<  "P" << "=";
            out.precision(20);
            out << "(" << pt->x << "," << pt->y << ")";
            return out;
        }
    };


#endif // MOSAICPOINT_HPP_INCLUDED
