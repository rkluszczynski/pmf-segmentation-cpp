/* Keywords: closest pair problem, plane sweep
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <cstdio>

using namespace std;

#define VAR(V,N)    __typeof(N) V = (N)
#define SIZE(X)     ((int)(X).size())

#define FOR(X,B,E)      for(int X = B; X <= (E); ++X)
#define REP(X,N)        for(int X = 0; X < (N); ++X)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)

#define PB  push_back
#define ST  first
#define ND  second

#define KEY double
#define POINT pair<KEY,KEY>

std::ostream & operator << (std::ostream& s, const POINT & p)
{
    s << "( " << p.first << " , " << p.second << " )";
    return s;
}

std::ostream & operator << (std::ostream& s, const vector<POINT> & vp)
{
    s << "[] : ";
    REP(i,SIZE(vp))  s << " " << vp[i];
    return s;
}


class ClosestPair
{
    private :
        vector<POINT> _pts;
        double distance;
        POINT P, Q;

        struct comp_map {
            bool operator() (const KEY & lhs, const KEY & rhs) const { return lhs<rhs; }
        };

        POINT GetPointFromVector(vector<POINT> & vp, const POINT & p)
        {
            POINT res = vp[0];
            //FOR(i,1,SIZE(vp))  if (abs(vp[i].ST - p.ST) < abs(res.ST - p.ST))  res = vp[i];
            //*
            vector<POINT>::iterator it = lower_bound(vp.begin(), vp.end(), p);
            if (it != vp.end()) {
                res = *it;
                if (it != vp.begin()) {
                    --it;
                    if (abs(it->ST - p.ST) < abs(res.ST - p.ST))  res = *it;
                }
            }
            else { res = vp.back(); }
            //*/
            return res;
        }
        void InsertPoint(map<KEY, vector<POINT>, comp_map> & ppion, POINT & p)
        {
            map<KEY, vector<POINT>, comp_map>::iterator it = ppion.find(p.second);
            if (it == ppion.end()) {
                vector<POINT> vp(0);
                vp.PB(p);
                ppion.insert( pair<KEY, vector<POINT> >(p.second, vp) );
            }
            else {
                (it->second).PB(p);
            }
        }
#define DIST2(P1, P2) ((P1.second-P2.second)*(P1.second-P2.second)+(P1.first-P2.first)*(P1.first-P2.first))
        void UpdateDistance(double & delta, POINT & p1, POINT & p2, char c = '-')
        {
            double dist = sqrt(DIST2(p1, p2));
            if (dist < delta) {
                //cout << "... changing distance (" << c << "): " << p1 << " and " << p2 << endl;
                delta = dist;
                //cout << "... changed to " << delta << endl;
                P = p1;  Q = p2;
            }
        }
#undef DIST2

    public :
        ClosestPair(int n = 0) : _pts(n), distance(-1) { }
        ~ClosestPair() { }

        POINT & operator[] (const int index) { return _pts[index]; }

        void PushBack(POINT & pp) { _pts.push_back(pp); }
        void SortPoints() { sort(_pts.begin(), _pts.end()); }

        void Execute(double delta = 10000)
        {
            map<KEY, vector<POINT>, comp_map> pion;

            int current = 0;
            int firstActive = current;

            while (current < SIZE(_pts))
            {
                POINT p = _pts[current];
                current++;
                //cout << endl << "[ POINT " << current << " ] : " << p << endl;

                /*/ print content:
                map<int, vector<pair<int, int> > >::iterator it;
                cout << "[ MAP ] ~ " << current << " : " << endl;
                for ( it=pion.begin() ; it != pion.end(); it++ )
                    cout << (*it).first << " => " << (*it).second << endl;
                cout << "\\___" << endl;
                //*/
                map<KEY, vector<POINT > >::iterator itlow, itup, iteq;
                //*
                if ((iteq = pion.find(p.second)) != pion.end())
                {
                    POINT pp = GetPointFromVector(iteq->second, p);
                    UpdateDistance(delta, p, pp, 'E');
                }
                //*/
                if (pion.begin() != pion.end())
                {
                    itlow = pion.lower_bound(p.second);
                    itup = pion.upper_bound(p.second);

                    //if (itlow != pion.end()) cout << "[ L ] : " << itlow->ND << endl;
                    //if (itup != pion.end()) cout << "[ U ] : " << itup->ND << endl;

                    int i = 0;
                    while (itlow != pion.begin() and i < 4)
                    {
                        --itlow;
                        POINT pp = GetPointFromVector(itlow->second, p);
                        //cout << " " << itlow->second << endl;
                        UpdateDistance(delta, p, pp, 'L');
                        i++;
                    }
                    //cout << " ." << endl;

                    i = 0;
                    while (itup != pion.end()  &&  i < 4) {
                        POINT pp = GetPointFromVector(itup->second, p);
                        //cout << " " << itup->second << endl;
                        UpdateDistance(delta, p, pp, 'U');
                        itup++;
                        i++;
                    }
                    //cout << " ." << endl;

                    //cout << " activeBefore = " << firstActive << endl;
                    int q = firstActive;
                    while ((p.first-_pts[q].first) > delta)
                    {
                        firstActive++;
                        pion.erase(_pts[q].second);
                        q = firstActive;
                    }
                }
                InsertPoint(pion, p);
                //cout << " active After = " << firstActive << endl;
            }
            distance = delta;
        }

        void WriteResult(double delta = 10000)
        {
            if (distance < delta) printf("%.4lf\n", distance); else printf("INFINITY\n");
            //printf("[ RESULT ] :  distance = %.6lf  ", distance);
            //printf("by  ( %i , %i )  ", P.first, P.second);
            //printf("and  ( %i , %i )\n", Q.first, Q.second);
        }
};


int main()
{
    //ifstream fin("tests/in10245.txt");  cin.rdbuf(fin.rdbuf());
    freopen("tests/in10245.txt", "r", stdin);
    int N;
    while(true) {
        scanf("%i", &N);
        //cin >> N;
        if (N == 0) break;

        ClosestPair cp;
        for (int i = 0; i < N; i++) {
            POINT pp;
            scanf("%lf%lf", &(pp.ST), &(pp.ND));
            //cin >> pp.first >> pp.second;
            cp.PushBack(pp);
        }
        cp.SortPoints();
        //for (int i = 0; i < N; i++) cout << " " << cp[i].first << "x" << cp[i].second;  cout << endl;
        cp.Execute();
        cp.WriteResult();
    }
    return 0;
}
