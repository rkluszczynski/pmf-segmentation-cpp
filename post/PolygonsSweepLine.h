#ifndef POLYGONSSWEEPLINE_H
#define POLYGONSSWEEPLINE_H

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
            return true;
        }


    protected:

    private:
        STATUS _st;
        double _x0;

};

#endif // POLYGONSSWEEPLINE_H
