#ifndef SEGMENT_HPP_INCLUDED
#define SEGMENT_HPP_INCLUDED

#include "../headers/macros.hpp"
#include "../cmd/point.hpp"
#include "../cmd/geometry.hpp"


namespace pmf
{
    //long long   pmf_segment_counter = 0;

    extern std::ostream out;

    template <typename REAL> struct Segment
    {
        typedef Point<REAL> * POINT;

        public :
            //Segment() {}
            Segment(POINT pp, POINT qq, NumericalParameters & np) : p(pp), q(qq) { OnInit(np); }
            ~Segment()
            {
                //out << "[ DEALLOCATING SEGMENT ]  :  " << this << endl;
                //out << "[ DEALLOCATING SEGMENT ]  :  ending at " << q << endl;
                ///--pmf_segment_counter;
            }

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
            REAL epsilon;
            REAL infinity;

            void OnInit(NumericalParameters & np)
            {
                epsilon = np.GetAxisEpsilon();
                assert(p->x <= q->x  ||  Geometry::IsZero(q->x - p->x, epsilon));
                //out << "[ _ALLOCATING_ SEGMENT ]  :  " << this << endl;
                ///++pmf_segment_counter;

                infinity = 1. / epsilon;
            }

    };


    //template <typename REAL> REAL Segment<REAL>::infinity = 0.;
}


#endif // SEGMENT_HPP_INCLUDED
