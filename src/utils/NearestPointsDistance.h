#ifndef NEARESTPOINTSDISTANCE_H
#define NEARESTPOINTSDISTANCE_H

#include <cmath>
#include <vector>
#include <algorithm>
#include <utility>


namespace pmf
{

    class NearestPointsDistance
    {
        public:
            typedef double TYPE;
            typedef double DISTTYPE;
            typedef std::pair<TYPE, TYPE> POINT;

            NearestPointsDistance();
            virtual ~NearestPointsDistance();

            inline
            void
            addPoint (TYPE x, TYPE y) { pts.push_back( std::make_pair(x, y) ); }
            inline
            void
            clearPoints () { pts.clear(); }

            DISTTYPE
            determineNearestPointsDistance ( std::pair<POINT*,POINT*> * = NULL );

        protected:

            void
            divideAndConquer ( std::vector<POINT*> &, unsigned, unsigned, std::vector<POINT*> &, DISTTYPE &, std::pair<POINT*,POINT*> * );

        private:
            std::vector<POINT> pts;

            inline
            DISTTYPE
            pointsDistance(POINT * p1, POINT * p2)
            {
                DISTTYPE dx = static_cast<DISTTYPE>(p1->first - p2->first);
                DISTTYPE dy = static_cast<DISTTYPE>(p1->second - p2->second);
                return dx * dx + dy * dy;
            }

    };

} // namespace pmf

#endif // NEARESTPOINTSDISTANCE_H
