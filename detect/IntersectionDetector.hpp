#ifndef INTERSECTIONDETECTOR_HPP
#define INTERSECTIONDETECTOR_HPP

#include <vector>

#include "DetectorSchedule.hpp"


class IntersectionDetector
{
    typedef double REAL;
    typedef std::pair<REAL, REAL> Point;
    typedef std::pair<Point, Point> Segment;

    public:
        typedef  std::vector<Segment>::iterator Iterator;
        typedef  std::vector<Segment>::const_iterator ConstIterator;

        IntersectionDetector();
        virtual ~IntersectionDetector();

        inline ConstIterator begin() const { return segments.begin(); }
        inline ConstIterator end()   const { return segments.end(); }

        void AddSegment(REAL, REAL, REAL, REAL);
        bool CheckIntersectionExistance();
        void ClearSegments();

    protected:

    private:
        std::vector<Segment> segments;
        DetectorSchedule schedule;
};

#endif // INTERSECTIONDETECTOR_HPP
