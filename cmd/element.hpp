#ifndef ELEMENT_HPP_INCLUDED
#define ELEMENT_HPP_INCLUDED

#include "../headers/macros.hpp"
#include "../cmd/segment.hpp"

namespace pmf
{

    template <typename REAL> struct SweepLineElement
    {
        public:
            typedef Segment<REAL> SEGMENT;


            SweepLineElement(const REAL & x, SEGMENT * s) : _x0(x), _seg(s)
            {
                _y0 = CalculateLinearFunctionValue(_seg, _x0);
            }

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

            friend ostream & operator << (ostream & out, const SweepLineElement * el)
            {
                out << "{" << el->GetSegment() << "~" << el->GetX0() << "," << el->GetY0() << "}";
                //out << "{" << el->GetSegment() << "}";
                return out;
            }


        private:
            REAL  _x0, _y0;
            SEGMENT * _seg;


            REAL CalculateLinearFunctionValue(SEGMENT * s, REAL x);
    };


    template <typename REAL>
    inline
    REAL
    SweepLineElement<REAL> :: CalculateLinearFunctionValue (SEGMENT * s, REAL x)
    {
        assert(! s.isVertical());
        REAL sslope = s.slope();
        REAL b = s->GetP()->y - sslope * s->GetP()->x;
        return sslope * x + b;
    }

}

#endif // ELEMENT_HPP_INCLUDED
