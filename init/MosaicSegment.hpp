#ifndef MOSAICSEGMENT_HPP_INCLUDED
#define MOSAICSEGMENT_HPP_INCLUDED

#include "../init/MosaicPoint.hpp"


    template <typename REAL> struct MosaicSegment
    {
        typedef MosaicPoint<REAL> * POINT;

        public :
            //Segment() {}
            MosaicSegment(POINT pp, POINT qq) : p(pp), q(qq) { OnInit(); }
            ~MosaicSegment()
            {
                //out << "[ DEALLOCATING SEGMENT ]  :  " << this << endl;
                //out << "[ DEALLOCATING SEGMENT ]  :  ending at " << q << endl;
                //--pmf_segment_counter;
            }

            const POINT GetLeftPoint() const { return p; }
            const POINT GetRightPoint() const { return q; }

            //bool isVertical()   const { return Geometry::IsZero(q->x - p->x); }
            //bool isHorizontal() const { return Geometry::IsZero(q->y - p->y); }
            //*
            REAL slope() const
            {
                //if (Geometry::IsZero(q->x - p->x)) return infinity;
                return (q->y - p->y) / (q->x - p->x);
            }
            // */
            friend ostream & operator << (ostream & out, const MosaicSegment * seg)
            {
                out << seg->GetLeftPoint() << "-" << seg->GetRightPoint();
                return out;
            }


        private :
            POINT p, q;

            void OnInit()
            {
                //assert(p->x <= q->x  ||  Geometry::IsZero(q->x - p->x));
                //out << "[ _ALLOCATING_ SEGMENT ]  :  " << this << endl;
                //++pmf_segment_counter;
            }

        //static REAL infinity;
    };


#endif // MOSAICSEGMENT_HPP_INCLUDED
