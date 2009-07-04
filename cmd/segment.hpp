#ifndef SEGMENT_HPP_INCLUDED
#define SEGMENT_HPP_INCLUDED

#include "../headers/macros.hpp"
#include "../cmd/point.hpp"
#include "../cmd/geometry.hpp"


namespace pmf
{
    long long   pmf_segment_counter = 0;

    template <typename REAL> struct Segment
    {
        typedef Point<REAL> * POINT;

        public :
            //Segment() {}
            Segment(POINT pp, POINT qq) : p(pp), q(qq) { OnInit(); }
            ~Segment() { --pmf_segment_counter; }

            const POINT GetP() const { return p; }
            const POINT GetQ() const { return q; }

            bool isVertical()   const { return Geometry::IsZero(q->x - p->x); }
            bool isHorizontal() const { return Geometry::IsZero(q->y - p->y); }
            REAL slope() const
            {
                if (Geometry::IsZero(q->x - p->x)) return infinity;
                return (q->y - p->y) / (q->x - p->x);
            }

            friend ostream & operator << (ostream & out, const Segment * seg)
            {
                out << seg->GetP() << "-" << seg->GetQ();
                return out;
            }


        private :
            POINT p, q;

            void OnInit()
            {
                assert(p->x <= q->x);
                ++pmf_segment_counter;
            }

        static REAL infinity;
    };


    template <typename REAL> REAL Segment<REAL>::infinity = 1 / EPSILON;
}


#endif // SEGMENT_HPP_INCLUDED
