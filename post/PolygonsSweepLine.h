#ifndef POLYGONSSWEEPLINE_H
#define POLYGONSSWEEPLINE_H

#include "geometry.hpp"

#include "PolygonsElement.hpp"


class PolygonsSweepLine
{
    typedef PolygonsSweepLineElement<double> ENTRY;
    typedef ENTRY::SEGMENT SEGMENT;

    class PolygonsSweepLineComparator
    {
        public :
            PolygonsSweepLineComparator(const PolygonsSweepLine * sl) : _sweepline(sl) {}
            bool operator() (ENTRY * const & lhs, ENTRY * const & rhs) const { return _sweepline->PolygonsSweepLineBelowComparator(lhs, rhs); }
        private :
            const PolygonsSweepLine * const _sweepline;
    };

    //typedef std::set<ENTRY *, bool(*)(ENTRY*,ENTRY*)> STATUSLINE;
    typedef std::set<ENTRY *, PolygonsSweepLineComparator> STATUS;


    public:
        PolygonsSweepLine();
        virtual ~PolygonsSweepLine();


        inline
        bool PolygonsSweepLineBelowComparator(ENTRY * e1, ENTRY * e2) const
        {
            using pmf::Geometry::IsZero;

            SEGMENT * s1 = e1->GetSegment();
            SEGMENT * s2 = e2->GetSegment();

            double y1 = e1->yy0(_x0);
            double y2 = e2->yy0(_x0);
                //bool res = true;
            bool res = (y1 < y2);

            return res;
        }


    protected:

    private:
        STATUS _st;
        double _x0;

};

#endif // POLYGONSSWEEPLINE_H
