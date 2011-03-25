#ifndef GEOMETRY_HPP_INCLUDED
#define GEOMETRY_HPP_INCLUDED

#include "../headers/macros.hpp"
#include "NumericParameters.h"

namespace pmf
{
    namespace Geometry
    {
        template <class REAL>
        inline
        bool
        ///IsZero(REAL x) { return x >= -EPSILON && x <= EPSILON; }
        IsZero(REAL x) { return x >= -NumericParameters::GetEpsilonDistance() && x <= NumericParameters::GetEpsilonDistance(); }


        template <class REAL>
        int
        SgnDet(REAL xp, REAL yp, REAL xq, REAL yq, REAL xr, REAL yr)
        {
            REAL tmp = xp*yq + xr*yp + xq*yr - xr*yq - xq*yp - xp*yr;
            //if(abs(tmp) < EPSILON) return(0);
            if (IsZero(tmp)) return(0);
            else if(tmp > 0.0) return(1);
            else return(-1);
        }


        template <class REAL>
        bool
        IsOnSegment(REAL xp, REAL yp, REAL xq, REAL yq, REAL xr, REAL yr)
        /* Return values:
         *    true - point (xr,yr) belongs to a segment (xp,yp)-(xq,yq)
         *   false - point is outside the segment
         */
        {
            ///printf(" is on (%f,%f)--(%f,%f)  point  (%f,%f) ???\n", xp, yp, xq, yq, xr, yr);
            /*
             if( sgnDet(xp, yp, xq, yq, xr, yr) == 0
                 &&  MIN(xp,xq) <= xr  &&  xr <= MAX(xp,xq)
                 &&  MIN(yp,yq) <= yr  &&  yr <= MAX(yp,yq) )
                 return(1);
             */
             if( SgnDet(xp, yp, xq, yq, xr, yr) == REAL(0) )
             {
                if( min(xp,xq) <= xr  &&  xr <= max(xp,xq)
                    &&  min(yp,yq) <= yr  &&  yr <= max(yp,yq) ) return true;

                ///if(abs(xp-xq) < EPSILON)
                REAL epsilon = NumericParameters::GetEpsilon();
                if(abs(xp-xq) < epsilon)
                {
                    if(abs(yp-yq) < epsilon)
                    {
                        if(abs(xp-xr) < epsilon  &&  abs(xr-xq) < epsilon
                            &&  abs(yp-yr) < epsilon  &&  abs(yr-yq) < epsilon) return true;
                    }
                    else {
                        if(abs(xp-xr) < epsilon  &&  abs(xr-xq) < epsilon
                            &&  min(yp,yq) <= yr  &&  yr <= max(yp,yq) ) return true;
                    }
                }
                else {
                    if(min(xp,xq) <= xr  &&  xr <= max(xp,xq))
                    {
                        if(abs(yp-yq) < epsilon)
                        {
                            if (abs(yp-yr) < epsilon  &&  abs(yr-yq) < epsilon) return true;
                        }
                        else {
                            if(min(yp,yq) <= yr  &&  yr <= max(yp,yq)) return true;
                        }
                    }
                }
             }
             return false;
        }


        template <class REAL>
        int
        CheckIntersection ( REAL xp, REAL yp, REAL xq, REAL yq /* Line 1 ( p-q ) */,
                            REAL xr, REAL yr, REAL xs, REAL ys /* Line 2 ( r-s ) */ )
        /* Returning values:
         *    0  - lines do not cross
         *    1  - lines crosses each others
         *  2..5 - end of one line belongs to other line
         */
        {
            int sgnDetPQR, sgnDetPQS, sgnDetRSP, sgnDetRSQ;

            sgnDetPQR = SgnDet(xp, yp, xq, yq, xr, yr);
            sgnDetPQS = SgnDet(xp, yp, xq, yq, xs, ys);
            sgnDetRSP = SgnDet(xr, yr, xs, ys, xp, yp);
            sgnDetRSQ = SgnDet(xr, yr, xs, ys, xq, yq);

            if ( (sgnDetPQR != 0  ||  sgnDetPQS != 0)  &&
                 (sgnDetRSP != 0  ||  sgnDetRSQ != 0)  &&
                sgnDetPQR == - sgnDetPQS  &&  sgnDetRSP == - sgnDetRSQ
               )
                return(1);
            if (IsOnSegment(xp, yp, xq, yq, xr, yr)) return(2);
            if (IsOnSegment(xp, yp, xq, yq, xs, ys)) return(3);
            if (IsOnSegment(xr, yr, xs, ys, xp, yp)) return(4);
            if (IsOnSegment(xr, yr, xs, ys, xq, yq)) return(5);
            return(0);
        }

extern bool qq;

        template <class REAL>
        int
        CheckIntersection2 ( REAL xp, REAL yp, REAL xq, REAL yq /* Line 1 ( p-q ) */,
                             REAL xr, REAL yr, REAL xs, REAL ys /* Line 2 ( r-s ) */ )
        /* Returning values:
         *    0  - lines do not cross
         *    1  - lines crosses each others
         */
        {
            pair<REAL, REAL> ipt = CalculateIntersection(xp, yp, xq, yq, xr, yr, xs, ys);
            REAL x = ipt.ST;
            REAL y = ipt.ND;

            REAL xmin = max( min(xp, xq), min(xr, xs) );
            REAL xmax = min( max(xp, xq), max(xr, xs) );
            REAL ymin = max( min(yp, yq), min(yr, ys) );
            REAL ymax = min( max(yp, yq), max(yr, ys) );
            /*
            if (qq)
                {
            cout.precision(20);
            cout << endl;
            cout << " @@@@@@@@@@@@@ x = " << x << endl;
            cout << " @@@@@@@@@@@@@ y = " << y << endl;
            cout << " @@@@@@@@@@@@@ xmin = " << xmin << endl;
            cout << " @@@@@@@@@@@@@ xmax = " << xmax << endl;
            cout << " @@@@@@@@@@@@@ ymin = " << ymin << endl;
            cout << " @@@@@@@@@@@@@ ymax = " << ymax << endl;
                }
            //*/
            if (IsZero(xmax - xmin))
            {
                if (IsZero(ymax - ymin))
                {
                    if (!IsZero(x - xmax) || !IsZero(x - xmin) || !IsZero(y - ymax) || !IsZero(y - ymin)) return 0;
                }
                else {
                    if (!IsZero(x - xmax) || !IsZero(x - xmin) || y < ymin || ymax < y) return 0;
                }
            }
            else {
                if (IsZero(ymax - ymin))
                {
                    if (x < xmin || xmax < x || !IsZero(y - ymax) || !IsZero(y - ymin)) return 0;
                }
                else {
                    if (x < xmin || xmax < x || y < ymin || ymax < y) return 0;
                }
            }
            if (IsZero(xq-x) && IsZero(yq-y)) return 5;
            return 1;
        }


        template <class REAL>
        void
        CalculateIntersection ( REAL x3, REAL y3, REAL x4, REAL y4,
                                REAL x1, REAL y1, REAL x2, REAL y2, REAL & x, REAL & y )
        {
            REAL a, b, c, d;
            if(! IsZero(x1 - x2))
            //if( x1 != x2 )
            {
                a = (y1 - y2)/(x1 - x2);
                b = y1 - a * x1;
                if(! IsZero(x3 - x4))
                {
                    c = (y3 - y4)/(x3 - x4);
                    d = y3 - c * x3;

                    x = (d - b)/(a - c);
                    y = a * (x) + b;
                }
                else {
                    x = 0.5 * (x3 + x4);
                    y = a * (x) + b;
                }
            }
            else {
                c = (y3 - y4)/(x3 - x4);
                d = y3 - c * x3;

                x = 0.5 * (x1 + x2);
                y = c * (x) + d;
            }
        }


        template <class REAL>
        pair<REAL, REAL>
        CalculateIntersection ( REAL x3, REAL y3, REAL x4, REAL y4,
                                REAL x1, REAL y1, REAL x2, REAL y2)
        {
            pair<REAL, REAL> result;
            CalculateIntersection(x3, y3, x4, y4, x1, y1, x2, y2, result.first, result.second);
            return result;
        }


        template <class REAL>
        inline
        REAL
        RadiansToDegree(REAL x) { return(x * 180.0 / M_PI); }


        template <class REAL>
        inline
        REAL
        DegreeToRadians(REAL x) { return(x * M_PI / 180.0); }


        template <class REAL>
        inline
        REAL
        PointsSquareDistance(REAL x1, REAL y1, REAL x2, REAL y2) { return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2); }


        template <class REAL>
        inline
        REAL
        PointsDistance(REAL x1, REAL y1, REAL x2, REAL y2) { return sqrt( PointsSquareDistance(x1, y1, x2, y2) ); }

    } // namespace pmf::Geometry
}

#endif // GEOMETRY_HPP_INCLUDED
