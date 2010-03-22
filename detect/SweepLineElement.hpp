#ifndef SWEEPLINEELEMENT_HPP
#define SWEEPLINEELEMENT_HPP

#include "DetectorSegment.hpp"


class SweepLineElement
{
    public:
        typedef double REAL;
        typedef DetectorSegment<REAL> SEGMENT;

        SweepLineElement(const REAL &, SEGMENT *);
        virtual ~SweepLineElement();

        inline SEGMENT * GetSegment() const { return _seg; }

        inline REAL GetX0() const { return _x0; }
        inline REAL GetY0() const { return _y0; }

        inline REAL yy0(const REAL & x) const { return CalculateLinearFunctionValue(_seg, x); }

        friend std::ostream & operator << (std::ostream &, const SweepLineElement &);


    private:
        REAL  _x0, _y0;
        SEGMENT * _seg;

        void OnInit();
        REAL CalculateLinearFunctionValue (SEGMENT * s, REAL x) const;
};

#endif // SWEEPLINEELEMENT_HPP
