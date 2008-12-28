#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

#define VAR(V,N)    __typeof(N) V = (N)
#define SIZE(X)     ((int)(X).size())

#define FOR(X,B,E)      for(int X = B; X <= (E); ++X)
#define REP(X,N)        for(int X = 0; X < (N); ++X)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)

#define PB  push_back
#define ST  first
#define ND  second

std::ostream & operator << (std::ostream& s, const pair<int,int> & p)
{
    s << "( " << p.first << " , " << p.second << " )";
    return s;
}

std::ostream & operator << (std::ostream& s, const vector<pair<int,int> > & vp)
{
    s << "[] : ";
    REP(i,SIZE(vp))  s << " " << vp[i];
    return s;
}


class ClosestPair
{
    private :
        vector<pair<int, int> > _pts;
        double distance;
        pair<int, int> P, Q;

        struct comp_map {
            bool operator() (const int& lhs, const int& rhs) const { return lhs<rhs; }
        };

        pair<int, int> GetPointFromVector(vector<pair<int, int> > & vp, const pair<int,int> & p)
        {
            pair<int,int> res = vp[0];
            FOR(i,1,SIZE(vp))
                if (abs(vp[i].ST - p.ST) < abs(res.ST - p.ST))  res = vp[i];
            return res;
        }
        void InsertPoint(map<int, vector<pair<int, int> >, comp_map> & ppion, pair<int,int> & p)
        {
            map<int, vector<pair<int, int> >, comp_map>::iterator it = ppion.find(p.second);
            if (it == ppion.end()) {
                vector<pair<int,int> > vp(0);
                vp.PB(p);
                ppion.insert( pair<int, vector<pair<int, int> > >(p.second, vp) );
            }
            else {
                (it->second).PB(p);
            }
        }

    public :
        ClosestPair(int n = 0) : _pts(n), distance(-1) { }
        ~ClosestPair() { }

        pair<int, int> & operator[] (const int index) { return _pts[index]; }

        void PushBack(pair<int, int> & pp) { _pts.push_back(pp); }
        void SortPoints() { sort(_pts.begin(), _pts.end()); }

#define DIST2(P1, P2) ((P1.second-P2.second)*(P1.second-P2.second)+(P1.first-P2.first)*(P1.first-P2.first))
        void Execute(double delta = 10000)
        {
            map<int, vector<pair<int, int> >, comp_map> pion;

            //for (int i = 0; i < _pts.size(); i++) pion.insert( pair<int, pair<int, int> >(_pts[i].second, _pts[i]));

            int current = 0;
            int firstActive = current;

            while (current < SIZE(_pts))
            {
                pair<int, int> p = _pts[current];
                current++;

                cout << endl << "[ POINT " << current << " ] : " << p << endl;
                // print content:
                map<int, vector<pair<int, int> > >::iterator it;
                cout << "[ MAP ] ~ " << current << " : " << endl;
                for ( it=pion.begin() ; it != pion.end(); it++ )
                    cout << (*it).first << " => " << (*it).second << endl;
                cout << "\\___" << endl;
                //*/
                map<int, vector<pair<int, int> > >::iterator itlow, itup;

                itlow = pion.lower_bound(p.second);
                itup = pion.upper_bound(p.second);

                if (itlow != pion.end()) cout << "[ L ] : " << itlow->ND << endl;
                if (itup != pion.end()) cout << "[ U ] : " << itup->ND << endl;
                //*
                int i = 0;
                while (itlow != pion.end() and i < 4)
                {
                    pair<int, int> pp = GetPointFromVector(itlow->second, p);
                    cout << " " << itlow->second << endl;
                    if (sqrt(DIST2(p, pp)) < delta) {
                        cout << "... changing distance (L): " << p << " and " << pp << endl;
                        delta = sqrt(DIST2(p, pp));
                        cout << "... changed to " << delta << endl;
                        P = p;  Q = pp;
                    }
                    itlow--;
                    i++;
                }
                cout << " ." << endl;

                i = 0;
                while (itup != pion.end()  &&  i < 4) {
                    pair<int, int> pp = GetPointFromVector(itup->second, p);
                    cout << " " << itup->second << endl;
                    if (sqrt(DIST2(p, pp)) < delta) {
                        cout << "... changing distance (U): " << p << " and " << pp << endl;
                        delta = sqrt(DIST2(p, pp));
                        cout << "... changed to " << delta << endl;
                        P = p;  Q = pp;
                    }
                    itup++;
                    i++;
                }
                cout << " ." << endl;
                //*/
                //*
                cout << " activeBefore = " << firstActive << endl;
                int q = firstActive;
                while ((p.first-_pts[q].first) > delta)
                {
                    firstActive++;
                    pion.erase(_pts[q].second);
                    q = firstActive;
                }
                //*/
                InsertPoint(pion, p);
                cout << " active After = " << firstActive << endl;
                //*/
            }
            distance = delta;
        }
#undef DIST2

        void Write()
        {
            printf("[ RESULT ] :  distance = %.6lf  ", distance);
            printf("by  ( %i , %i )  ", P.first, P.second);
            printf("and  ( %i , %i )\n", Q.first, Q.second);
        }
};


int main()
{
    ifstream fin("tests/in10245.txt");  cin.rdbuf(fin.rdbuf());
    int N;
    while(true) {
        cin >> N;
        if (N == 0) break;

        ClosestPair cp;
        for (int i = 0; i < N; i++) {
            pair<int, int> pp;
            cin >> pp.first;
            cin >> pp.second;
            cp.PushBack(pp);
        }
        cp.SortPoints();
        for (int i = 0; i < N; i++) cout << " " << cp[i].first << "x" << cp[i].second;  cout << endl;
        cp.Execute();
        cp.Write();
    }
    return 0;
}
