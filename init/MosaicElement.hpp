#ifndef MOSAICELEMENT_HPP_INCLUDED
#define MOSAICELEMENT_HPP_INCLUDED

#include "../init/MosaicSegment.hpp"


template <typename REAL> struct MosaicSweepLineElement
{
    public:
        typedef MosaicSegment<REAL> SEGMENT;


        MosaicSweepLineElement(const REAL & x, SEGMENT * s) : _x0(x), _seg(s)
        {
            OnInit();
            _y0 = CalculateLinearFunctionValue(_seg, _x0);
        }

        virtual ~MosaicSweepLineElement() { } //--pmf_element_counter; }


        SEGMENT * GetSegment() const { return _seg; }

        REAL GetX0() const { return _x0; }
        REAL GetY0() const { return _y0; }
        /*
            REAL y0(const REAL & x)
            {
                if (_x0 == x) return _y0;
                _x0 = x;
                _y0 = CalculateLinearFunctionValue(_seg, _x0);
                return _y0;
            }
        //*/
        REAL yy0(const REAL & x) const { return CalculateLinearFunctionValue(_seg, x); }

        friend ostream & operator << (ostream & out, const MosaicSweepLineElement & el)
        {
            out << "{ " << el.GetSegment() << "\t ~ \t" << el.GetX0() << " , " << el.GetY0() << " ; " << el.yy0(el.GetX0()) << " }";
            //out << "{" << el->GetSegment() << "}";
            return out;
        }


    private:
        REAL  _x0, _y0;
        SEGMENT * _seg;

        void OnInit() { } //++pmf_element_counter; }
        REAL CalculateLinearFunctionValue (SEGMENT * s, REAL x) const;

};


template <typename REAL>
inline
REAL
MosaicSweepLineElement<REAL> :: CalculateLinearFunctionValue (SEGMENT * s, REAL x) const
{
    /*
    if (s->isVertical())  return s->GetP()->y;
    assert(! s->isVertical());
    // */
    REAL _slope = s->slope();
    REAL _b = s->GetLeftPoint()->y - _slope * s->GetLeftPoint()->x;
    return _slope * x + _b;
}


#endif // MOSAICELEMENT_HPP_INCLUDED
