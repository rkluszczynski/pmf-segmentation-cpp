#ifndef TYPES_HPP_INCLUDED
#define TYPES_HPP_INCLUDED

#define EPS 1e-9

class Types
{
    public :
        typedef double NT;
        typedef double FT;

        struct Point
        {
            private :
                NT _x, _y;

            public :
                Point() : _x(0), _y(0) {}
                Point(NT xx, NT yy) : _x(xx), _y(yy) {}

                const NT GetX() const { return _x; }
                const NT GetY() const { return _y; }

                const NT x() const { return _x; }
                const NT y() const { return _y; }

                friend ostream & operator << (ostream & out, const Point pt)
                {
                    out << "(" << pt.GetX() << ";" << pt.GetY() << ")";
                    return out;
                }
        };

        struct Segment
        {
            private :
                Point p, q;

            public :
                Segment() {}
                Segment(Point pp, Point qq) : p(pp), q(qq) {}

                Point * GetPointerP() { return &p; }
                Point * GetPointerQ() { return &q; }

                const Point GetP() const { return p; }
                const Point GetQ() const { return q; }

                bool isVertical() const { return q.x() == p.x(); }
                bool isHorizontal() const { return q.y() == p.y(); }
                FT slope() const
                {
                    if (q.x() == p.x()) return 1 / EPS;
                    return (q.y() - p.y()) / (q.x() - p.x());
                }

                friend ostream & operator << (ostream & out, const Segment seg)
                {
                    out << seg.GetP() << "-" << seg.GetQ();
                    return out;
                }
        };


        static FT linear_function_value(const Segment & s, const NT & x)
        {
            assert(! s.isVertical());
            FT sslope = s.slope();
            FT b = s.GetP().y() - sslope * s.GetP().x();
            return sslope * x + b;
        }

        typedef Point POINT;
        typedef Segment SEGMENT;


        static inline bool IsZero(FT x) { return x > -EPS && x < EPS; }

        static int sgnDet(const POINT & p, const POINT & q, const POINT & r )
        {
            FT tmp = p.x()*q.y() + r.x()*p.y() + q.x()*r.y() - r.x()*q.y() - q.x()*p.y() - p.x()*r.y();
            if(IsZero(tmp)) return 0;
            else if(tmp > 0.0) return(1);
            else return(-1);
        }

        static int isOnSegment(const POINT & p, const POINT & q, const POINT & r )
            /* Return values:
             *   1 - point r belongs to a segment p-q
             *   0 - point is outside the segment
             */
        {
            if( sgnDet(p, q, r) == 0
                &&  min(p.x(),q.x()) <= r.x()  &&  r.x() <= max(p.x(),q.x())
                &&  min(p.y(),q.y()) <= r.y()  &&  r.y() <= max(p.y(),q.y())
            )
                return(1);
            return(0);
        }

        static bool AreCollinear(const Segment & s1, const Segment & s2)
        {
            /// TODO
            return false;
        }

        static int cross3(const SEGMENT & seg1 /* Line 1 ( p-q ) */, const SEGMENT & seg2 /* Line 2 ( r-s ) */)
            /* Returning values:
             *    0  - lines do not cross
             *    1  - lines crosses each others
             *  2..5 - end of one line belongs to other line
             */
        {
            int sgnDetPQR, sgnDetPQS, sgnDetRSP, sgnDetRSQ;

            sgnDetPQR = sgnDet(seg1.GetP(), seg1.GetQ(), seg2.GetP());
            sgnDetPQS = sgnDet(seg1.GetP(), seg1.GetQ(), seg2.GetQ());
            sgnDetRSP = sgnDet(seg2.GetP(), seg2.GetQ(), seg1.GetP());
            sgnDetRSQ = sgnDet(seg2.GetP(), seg2.GetQ(), seg1.GetQ());

            if( (sgnDetPQR != 0  ||  sgnDetPQS != 0)  &&
                (sgnDetRSP != 0  ||  sgnDetRSQ != 0)  &&
                sgnDetPQR == - sgnDetPQS  &&  sgnDetRSP == - sgnDetRSQ
            )
                return(1);
            if(isOnSegment(seg1.GetP(), seg1.GetQ(), seg2.GetP()) == 1) return(2);
            if(isOnSegment(seg1.GetP(), seg1.GetQ(), seg2.GetQ()) == 1) return(3);
            if(isOnSegment(seg2.GetP(), seg2.GetQ(), seg1.GetP()) == 1) return(4);
            if(isOnSegment(seg2.GetP(), seg2.GetQ(), seg1.GetQ()) == 1) return(5);
            return(0);
        }

        static Point * AnalizeSegments(const SEGMENT & seg1 /* Line 1 ( p-q ) */, const SEGMENT & seg2 /* Line 2 ( r-s ) */)
        {
            if (cross3(seg1, seg2) != 1)  return NULL;
            return crosspoint2(seg2.GetP(), seg2.GetQ(), seg1.GetP(), seg1.GetQ());
        }


        static POINT * crosspoint2(const POINT & p3, const POINT & p4, const POINT & p1, const POINT & p2 )
        {
            FT a, b, c, d, x, y;
            if( p1.x() != p2.x() )
            {
                a = (p1.y() - p2.y()) / (p1.x() - p2.x());
                b = p1.y() - a * p1.x();
                if( p3.x() != p4.x() )
                {
                    c = (p3.y() - p4.y()) / (p3.x() - p4.x());
                    d = p3.y() - c * p3.x();

                    x = (d - b)/(a - c);
                    y = a * (x) + b;
                }
                else {
                    x = p3.x();
                    y = a * (x) + b;
                }
            }
            else {
                c = (p3.y() - p4.y()) / (p3.x() - p4.x());
                d = p3.y() - c * p3.x();

                x = p1.x();
                y = c * (x) + d;
            }
            POINT * cpt = new POINT(x, y);
            return cpt;
        }
};


        typedef Types::Segment SEGMENT;
        typedef Types::Point   POINT;
        typedef Types::NT      NT;
        typedef Types::FT      FT;

        typedef enum
        {
            SOURCE = 0,
            TARGET = 1,
            INTERSECTION = 2
        }
        EventType;

#endif // TYPES_HPP_INCLUDED
