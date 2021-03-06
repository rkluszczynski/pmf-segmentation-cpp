#ifndef ELEMENT_HPP_INCLUDED
#define ELEMENT_HPP_INCLUDED

#include "../headers/macros.hpp"
#include "../cmd/segment.hpp"

namespace pmf
{
    long long   pmf_element_counter = 0;

    template <typename REAL> struct SweepLineElement
    {
        public:
            typedef Segment<REAL> SEGMENT;


            SweepLineElement(const REAL & x, SEGMENT * s) : _x0(x), _seg(s)
            {
                OnInit();
                _y0 = CalculateLinearFunctionValue(_seg, _x0);
            }
            virtual ~SweepLineElement() { --pmf_element_counter; }

            SEGMENT * GetSegment() const { return _seg; }
            REAL GetX0() const { return _x0; }
            REAL GetY0() const { return _y0; }

            REAL y0(const REAL & x)
            {
                if (_x0 == x) return _y0;
                _x0 = x;
                _y0 = CalculateLinearFunctionValue(_seg, _x0);
                return _y0;
            }
            REAL yy0(const REAL & x) const { return CalculateLinearFunctionValue(_seg, x); }
            //*
            friend ostream & operator << (ostream & out, const SweepLineElement & el)
            {
                out << "{ " << el.GetSegment() << "\t ~ \t" << el.GetX0() << " , " << el.GetY0() << " ; " << el.yy0(el.GetX0()) << " }";
                //out << "{" << el->GetSegment() << "}";
                return out;
            }
            // */

        private:
            REAL  _x0, _y0;
            SEGMENT * _seg;

            void OnInit() { ++pmf_element_counter; }
            REAL CalculateLinearFunctionValue (SEGMENT * s, REAL x) const;
    };


    template <typename REAL>
    inline
    REAL
    SweepLineElement<REAL> :: CalculateLinearFunctionValue (SEGMENT * s, REAL x) const
    {
        if (s->isVertical())  return s->GetP()->y;
        assert(! s->isVertical());
        REAL sslope = s->slope();
        REAL b = s->GetP()->y - sslope * s->GetP()->x;
        return sslope * x + b;
    }

}

#endif // ELEMENT_HPP_INCLUDED
