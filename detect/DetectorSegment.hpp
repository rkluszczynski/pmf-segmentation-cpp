#ifndef DETECTORSEGMENT_HPP_INCLUDED
#define DETECTORSEGMENT_HPP_INCLUDED

#include "DetectorPoint.hpp"

template <typename REAL>
struct DetectorSegment
{
    typedef DetectorPoint<REAL> * POINT;

    public :
        DetectorSegment(POINT p, POINT q) : bpt(p), ept(q) { OnInit(); }
        virtual ~DetectorSegment()
        {
                //out << "[ DEALLOCATING SEGMENT ]  :  " << this << endl;
                //out << "[ DEALLOCATING SEGMENT ]  :  ending at " << q << endl;
                //--pmf_segment_counter;
        }

        const POINT GetBeginPoint()  const { return bpt; }
        const POINT GetEndPoint()    const { return ept; }

        //bool isVertical()   const { return Geometry::IsZero(q->x - p->x); }
        //bool isHorizontal() const { return Geometry::IsZero(q->y - p->y); }
        //*
        REAL slope() const
        {
                //if (Geometry::IsZero(q->x - p->x)) return infinity;
                return (ept->y() - bpt->y()) / (ept->x() - bpt->x());
        }
        // */
        friend std::ostream & operator << (std::ostream & out, const DetectorSegment & s)
        {
            out << *s.GetBeginPoint() << "-" << *s.GetEndPoint();
            return out;
        }

    private :
        POINT bpt, ept;

        void OnInit()
        {
            //assert(p->x <= q->x  ||  Geometry::IsZero(q->x - p->x));
            //out << "[ _ALLOCATING_ SEGMENT ]  :  " << this << endl;
            //++pmf_segment_counter;
        }
        //static REAL infinity;
};


#endif // DETECTORSEGMENT_HPP_INCLUDED
