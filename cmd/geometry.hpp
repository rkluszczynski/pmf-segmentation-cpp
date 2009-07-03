#ifndef GEOMETRY_HPP_INCLUDED
#define GEOMETRY_HPP_INCLUDED

#include "../headers/macros.hpp"

namespace pmf
{
    namespace Geometry
    {
        template <class REAL>
        inline
        bool
        IsZero(REAL x) { return x >= -EPSILON && x <= EPSILON; }


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
            /*
             if( sgnDet(xp, yp, xq, yq, xr, yr) == 0
                 &&  MIN(xp,xq) <= xr  &&  xr <= MAX(xp,xq)
                 &&  MIN(yp,yq) <= yr  &&  yr <= MAX(yp,yq) )
                 return(1);
             */
             if( SgnDet(xp, yp, xq, yq, xr, yr) == 0 )
             {
                if( min(xp,xq) <= xr  &&  xr <= max(xp,xq)
                    &&  min(yp,yq) <= yr  &&  yr <= max(yp,yq) ) return true;

                if(abs(xp-xq) < EPSILON)
                {
                    if(abs(yp-yq) < EPSILON)
                    {
                        if(abs(xp-xr) < EPSILON  &&  abs(xr-xq) < EPSILON
                            &&  abs(yp-yr) < EPSILON  &&  abs(yr-yq) < EPSILON) return true;
                    }
                    else {
                        if(abs(xp-xr) < EPSILON  &&  abs(xr-xq) < EPSILON
                            &&  min(yp,yq) <= yr  &&  yr <= max(yp,yq) ) return true;
                    }
                }
                else {
                    if(min(xp,xq) <= xr  &&  xr <= max(xp,xq))
                    {
                        if(abs(yp-yq) < EPSILON)
                        {
                            if (abs(yp-yr) < EPSILON  &&  abs(yr-yq) < EPSILON) return true;
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


        template <class REAL>
        void
        CalculateIntersection ( REAL x3, REAL y3, REAL x4, REAL y4,
                                REAL x1, REAL y1, REAL x2, REAL y2, REAL & x, REAL & y )
        {
            REAL a, b, c, d;
            if( x1 != x2 )
            {
                a = (y1 - y2)/(x1 - x2);
                b = y1 - a * x1;
                if( x3 != x4 )
                {
                    c = (y3 - y4)/(x3 - x4);
                    d = y3 - c * x3;

                    x = (d - b)/(a - c);
                    y = a * (x) + b;
                }
                else {
                    x = x3;
                    y = a * (x) + b;
                }
            }
            else {
                c = (y3 - y4)/(x3 - x4);
                d = y3 - c * x3;

                x = x1;
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

    } // namespace pmf::Geometry
}

#endif // GEOMETRY_HPP_INCLUDED