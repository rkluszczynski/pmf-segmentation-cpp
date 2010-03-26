#ifndef INTERSECTIONDETECTOR_HPP
#define INTERSECTIONDETECTOR_HPP

#include <vector>

#include "DetectorSchedule.hpp"


class IntersectionDetector
{
    typedef double REAL;

    public:
        typedef std::pair<REAL, REAL> Point;
        typedef std::pair<Point, Point> Segment;

        typedef  std::vector<Segment>::iterator Iterator;
        typedef  std::vector<Segment>::const_iterator ConstIterator;

        IntersectionDetector(double = 1e-9);
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
        REAL epsilon;

        bool DoSegmentsIntersect(DetectorSegment<REAL> *, DetectorSegment<REAL> *);
        Point CalculateIntersection(REAL, REAL, REAL, REAL, REAL, REAL, REAL, REAL);
};

#endif // INTERSECTIONDETECTOR_HPP
