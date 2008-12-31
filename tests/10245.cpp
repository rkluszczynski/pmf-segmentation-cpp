/* Keywords: closest pair problem, plane sweep
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <cstdio>

using namespace std;

#define SIZE(X)     ((int)(X).size())
#define FOR(X,B,E)  for(int X = B; X <= (E); ++X)
#define REP(X,N)    for(int X = 0; X < (N); ++X)

#define TYPE double

template <class Real = double>
struct Point
{
    private :
        Real x, y;

    public :
        Point() : x(-1), y(-1) { }
        Point(Real xx, Real yy) : x(xx), y(yy) { }

        const Real GetX() const { return x; }
        const Real GetY() const { return y; }
        /*
        Point & operator = (const Point<Real> & other)
        {
            if (this != &other)
            {
                x = other.GetX();
                y = other.GetY();
            }
            return *this;
        }
        //*/
        //bool operator ==(const Point<Real> & pt) const { return pt.x == x && pt.y == y; }
        bool operator < (const Point<Real> & pt) const { return pt.x < x || (pt.x == x  &&  pt.y < y); }

        friend ostream & operator << (ostream & out, const Point<Real> pt)
        {
            out << "( " << pt.GetX() << " , " << pt.GetY() << " )";
            return out;
        }
};


class ClosestPair
{
    typedef Point<TYPE> POINT;
    typedef map<TYPE, vector<POINT > > _MAP_TYPE;

    private :
        vector<POINT> _pts;
        double distance;
        POINT P, Q;

        POINT GetPointFromVector(vector<POINT> & vp, const POINT & p)
        {
            POINT res;
            vector<POINT>::iterator it = lower_bound(vp.begin(), vp.end(), p);
            if (it != vp.end()) {
                res = *it;
                if (it != vp.begin()) {
                    --it;
                    if (abs(it->GetX() - p.GetX()) < abs(res.GetX() - p.GetX()))  res = *it;
                }
            }
            else { res = vp.back(); }
            return res;
        }
        void InsertPoint(_MAP_TYPE & ppion, POINT & p)
        {
            _MAP_TYPE::iterator it = ppion.find(p.GetY());
            if (it == ppion.end()) {
                vector<POINT> vp(0);
                vp.push_back(p);
                ppion.insert( pair<TYPE, vector<POINT> >(p.GetY(), vp) );
            }
            else { (it->second).push_back(p); }
        }
        void UpdateDistance(double & delta, POINT & p1, POINT & p2, char c = '-')
        {
            double dist = sqrt((p1.GetY()-p2.GetY())*(p1.GetY()-p2.GetY())+(p1.GetX()-p2.GetX())*(p1.GetX()-p2.GetX()));
            if (dist < delta) {
                //cout << "... changing distance (" << c << "): " << p1 << " and " << p2 << endl;
                delta = dist;
                //cout << "... changed to " << delta << endl;
                P = p1;  Q = p2;
                //cout << " ... between " << P << " and " << Q << endl;
            }
        }

    public :
        ClosestPair(int n = 0) : _pts(n), distance(-1) { }
        ~ClosestPair() { }

        POINT & operator[] (const int index) { return _pts[index]; }

        void PushBack(POINT & pp) { _pts.push_back(pp); }
        void SortPoints() { sort(_pts.begin(), _pts.end()); }

        void Execute(double delta = 10000)
        {
            _MAP_TYPE sweep;
            int current = 0;
            int firstActive = current;

            while (current < SIZE(_pts))
            {
                POINT p = _pts[current];
                current++;

                _MAP_TYPE::iterator itlow, itup, iteq;

                if ((iteq = sweep.find(p.GetY())) != sweep.end())
                {
                    POINT pp = GetPointFromVector(iteq->second, p);
                    UpdateDistance(delta, p, pp, 'E');
                }
                if (sweep.begin() != sweep.end())
                {
                    itlow = sweep.lower_bound(p.GetY());
                    itup = itlow;
                    if (iteq != sweep.end()) itup++;

                    int i = 0;
                    while (itlow != sweep.begin() and i < 4)
                    {
                        --itlow;
                        POINT pp = GetPointFromVector(itlow->second, p);
                        UpdateDistance(delta, p, pp, 'L');
                        i++;
                    }

                    i = 0;
                    while (itup != sweep.end()  &&  i < 4) {
                        POINT pp = GetPointFromVector(itup->second, p);
                        UpdateDistance(delta, p, pp, 'U');
                        itup++;
                        i++;
                    }

                    int q = firstActive;
                    while ((p.GetX()-_pts[q].GetX()) > delta)
                    {
                        firstActive++;
                        sweep.erase(_pts[q].GetY());
                        q = firstActive;
                    }
                }
                InsertPoint(sweep, p);
            }
            distance = delta;
        }

        void WriteResult(double delta = 10000)
        {
            if (distance < delta) printf("%.4lf\n", distance); else printf("INFINITY\n");
            /*
            printf("[ RESULT ] :  distance = %.6lf  ", distance);
            printf("by  ( %.7lf , %.7lf )  ", P.GetX(), P.GetY());
            printf("and  ( %.7lf , %.7lf )\n", Q.GetX(), Q.GetY());
            //*/
        }
};


int main()
{
#ifndef ONLINE_JUDGE
    freopen("tests/in10245.txt", "r", stdin);
#endif
    int N;
    while(true) {
        scanf("%i", &N);
        if (N == 0) break;

        ClosestPair cp;
        for (int i = 0; i < N; i++) {
            double x, y;
            scanf("%lf%lf", &x, &y);
            Point<TYPE> pt(x, y);
            cp.PushBack(pt);
        }
        cp.SortPoints();
        cp.Execute();
        cp.WriteResult();
    }
    return 0;
}
