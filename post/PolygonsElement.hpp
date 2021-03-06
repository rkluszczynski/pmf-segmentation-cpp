#ifndef POLYGONSELEMENT_HPP_INCLUDED
#define POLYGONSELEMENT_HPP_INCLUDED

#include "segment.hpp"


    template <typename REAL> struct PolygonsSweepLineElement
    {
        public:
            typedef pmf::Point<REAL> POINT;
            typedef pmf::Segment<REAL> SEGMENT;


            PolygonsSweepLineElement(const REAL & x, SEGMENT * s, long id) : _x0(x), _seg(s)
            {
                OnInit();
                _up_area = id;
                _y0 = CalculateLinearFunctionValue(_seg, _x0);
            }
            virtual ~PolygonsSweepLineElement() { }


            SEGMENT * GetSegment() const { return _seg; }
            REAL GetX0() const { return _x0; }
            REAL GetY0() const { return _y0; }

            inline
            long GetUpperAreaNumber() { return _up_area; }
            inline
            void SetUpperAreaNumber(long num) { _up_area = num; }

            REAL y0(const REAL & x)
            {
                if (_x0 == x) return _y0;
                _x0 = x;
                _y0 = CalculateLinearFunctionValue(_seg, _x0);
                return _y0;
            }
            REAL yy0(const REAL & x) const { return CalculateLinearFunctionValue(_seg, x); }
            //*
            friend ostream & operator << (ostream & out, const PolygonsSweepLineElement & el)
            {
                out << "{ " << el.GetSegment() << "\t ~ \t" << el.GetX0() << " , " << el.GetY0() << " ; " << el.yy0(el.GetX0()) << " }";
                //out << "{" << el->GetSegment() << "}";
                return out;
            }
            // */

        private:
            REAL  _x0, _y0;
            long  _up_area;
            SEGMENT * _seg;

            void OnInit() { }
            REAL CalculateLinearFunctionValue (SEGMENT * s, REAL x) const;
    };


    template <typename REAL>
    inline
    REAL
    PolygonsSweepLineElement<REAL> :: CalculateLinearFunctionValue (SEGMENT * s, REAL x) const
    {
        if (s->isVertical())  return s->GetP()->y;
        assert(! s->isVertical());
        REAL sslope = s->slope();
        REAL b = s->GetP()->y - sslope * s->GetP()->x;
        return sslope * x + b;
    }


#endif // POLYGONSELEMENT_HPP_INCLUDED
