#ifndef LINEELEMENT_HPP_INCLUDED
#define LINEELEMENT_HPP_INCLUDED

struct LineElement
{
    public:
        typedef Types::Segment SEGMENT;
        typedef Types::Point   POINT;
        typedef Types::NT      NT;
        typedef Types::FT      FT;

        LineElement(const NT & x, SEGMENT * s) : _x0(x), _seg(s)
        {
            _y0 = Types::linear_function_value(*_seg, _x0);
        }

        SEGMENT * segment() const { return _seg; }

        NT y0(const NT & x)
        {
            if (_x0 == x) return _y0;
            _x0 = x;
            _y0 = Types::linear_function_value(*_seg, _x0);
            return _y0;
        }

        NT yy0(const NT & x) const { return Types::linear_function_value(*_seg, x); }

        friend ostream & operator << (ostream & out, const LineElement le)
        {
            //out << "{" << *le.segment() << "~" << le.getX0() << "," << le.getY0() << "}";
            out << "{" << *le.segment() << "}";
            return out;
        }

        FT getX0() const { return _x0; }
        FT getY0() const { return _y0; }

    private:
        NT     _x0, _y0;
        SEGMENT *  _seg;
};

#endif // LINEELEMENT_HPP_INCLUDED
