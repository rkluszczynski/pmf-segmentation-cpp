#include "NearestPointsDistance.h"

namespace pmf {



NearestPointsDistance::NearestPointsDistance()
{
    //ctor
}

NearestPointsDistance::~NearestPointsDistance()
{
    //dtor
}


        inline
        bool
        _orderXY (const NearestPointsDistance::POINT * const p1, const NearestPointsDistance::POINT * const p2)
        { return (p1->first == p2->first) ? (p1->second < p2->second) : (p1->first < p2->first); }

        inline
        bool
        _orderYX (const NearestPointsDistance::POINT * const p1, const NearestPointsDistance::POINT * const p2)
        { return (p1->second == p2->second) ? (p1->first < p2->first) : (p1->second < p2->second); }

#define VAR(V,N)    __typeof(N) V = (N)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)
NearestPointsDistance::DISTTYPE
NearestPointsDistance::determineNearestPointsSquareDistance(std::pair<POINT*,POINT*> * nearestPair)
{
    const unsigned n = pts.size();
    std::vector<POINT*> hpts;
    FOREACH(it, pts)  hpts.push_back( &(*it) );
    sort(hpts.begin(), hpts.end(), _orderXY);
    for(unsigned i = 1u; i < hpts.size(); ++i)
        if (hpts[i-1]->first == hpts[i]->first  and  hpts[i-1]->second == hpts[i]->second)
        {
            if (nearestPair) nearestPair->first = nearestPair->second = hpts[i];
            return 0;
        }
    std::vector<POINT*> vpts(hpts);
    sort(vpts.begin(), vpts.end(), _orderYX);

    if (n < 2u) return std::numeric_limits<DISTTYPE>::max();
    DISTTYPE squareDist = pointsSquareDistance(hpts[0], hpts[1]);
    DISTTYPE dist = sqrt(squareDist);
    if (nearestPair)
    {
        nearestPair->first = hpts[0];
        nearestPair->second = hpts[1];
    }
    divideAndConquer(hpts, 0u, n, vpts, squareDist, dist, nearestPair);
    return squareDist;
}


#define VAR(V,N)    __typeof(N) V = (N)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)
void
NearestPointsDistance::divideAndConquer (
                                         std::vector<POINT*> & hpts, unsigned l, unsigned r,
                                         std::vector<POINT*> & vpts,
                                         DISTTYPE & squareDist, DISTTYPE & distance, std::pair<POINT*,POINT*> * nearestPair)
{
    if (r - l < 2) return;

    std::vector<POINT*> lpart, rpart;
    unsigned c = (r + l - 1) / 2;
    FOREACH(it, vpts)  if (_orderXY(hpts[c], *it)) rpart.push_back(*it); else lpart.push_back(*it);

    divideAndConquer(hpts, l, c+1, lpart, squareDist, distance, nearestPair);
    divideAndConquer(hpts, c+1, r, rpart, squareDist, distance, nearestPair);

    unsigned s = 0u;
    for(unsigned i = 0u; i < lpart.size(); ++i) if (std::abs(lpart[i]->first - hpts[c]->first) < distance) { lpart[s] = lpart[i]; ++s; }
    lpart.resize(s);
    s = 0u;
    for(unsigned i = 0u; i < rpart.size(); ++i) if (std::abs(rpart[i]->first - hpts[c]->first) < distance) { rpart[s] = rpart[i]; ++s; }
    rpart.resize(s);

    int p = 0;
    DISTTYPE tmp;
    FOREACH(it, lpart)
    {
        while (p < int(rpart.size())-1  and  rpart[p + 1]->second < (*it)->second) ++p;

        int end = std::min( int(rpart.size()) - 1, p+2 );
        for (int i = std::max(0,p-3); i <= end; ++i)
        //FOR(i, max(0, p - 2), min(int(rpart.size()-1), p + 1))
            if (squareDist > (tmp = pointsSquareDistance(*it, rpart[i])))
            {
                squareDist = tmp;
                distance = sqrt(squareDist);
                if (nearestPair)
                {
                    nearestPair->first = *it;
                    nearestPair->second = rpart[i];
                }
            }
    }
}
#undef FOREACH
#undef VAR



} // namespace pmf
