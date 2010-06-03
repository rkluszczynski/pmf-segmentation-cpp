#ifndef POLYGONSSWEEPLINE_H
#define POLYGONSSWEEPLINE_H

#include "geometry.hpp"

#include "PolygonsElement.hpp"


class PolygonsSweepLine
{
    typedef PolygonsSweepLineElement<double> ENTRY;
    typedef ENTRY::POINT POINT;
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
        typedef STATUS::iterator Iterator;
        typedef STATUS::const_iterator ConstIterator;

        PolygonsSweepLine(double = 1e-9);
        virtual ~PolygonsSweepLine();

        void SetSweepLinePosition(double x)
        {
            assert(x >= _x0);
            _x0 = x;
        }

        pair<Iterator, bool> Insert(const POINT * pt, SEGMENT * seg)
        {
            if (pt) SetSweepLinePosition(pt->x + _epsilon);
            return _st.insert( new ENTRY(_x0, seg) );
        }
        inline
        void Erase(SEGMENT * seg)
        {
            Iterator it = Find(seg);
            assert(! IsNull(it));
            Erase(it);
        }
        void Erase(Iterator it)
        {
            assert(! IsNull(it));
            ENTRY * ent = *it;
            _st.erase(it);
            delete ent;
        }

        Iterator Find(SEGMENT * seg)
        {
            ENTRY * entry = new ENTRY(_x0, seg);
            Iterator it = _st.find( entry );
            delete entry;
            return it;
        }
        bool IsNull(Iterator it) { return it == _st.end(); }

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
        const double _epsilon;
};

#endif // POLYGONSSWEEPLINE_H
