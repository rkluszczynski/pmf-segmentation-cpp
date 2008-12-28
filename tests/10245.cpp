#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

#include "avl.hpp"

using namespace std;


std::ostream & operator << (std::ostream& s, const pair<int,int> & p)
{
    s << "( " << p.first << " , " << p.second << " )";
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


    public :
        ClosestPair(int n = 0) : _pts(n), distance(-1) { }
        ~ClosestPair() { }

        pair<int, int> & operator[] (const int index) { return _pts[index]; }

        void PushBack(pair<int, int> & pp) { _pts.push_back(pp); }
        void SortPoints() { sort(_pts.begin(), _pts.end()); }
        void Execute()
        {
#define DIST2(P1, P2) ((P1.second-P2.second)*(P1.second-P2.second)+(P1.first-P2.first)*(P1.first-P2.first))
            map<int, pair<int, int>, comp_map> pion;

            //for (int i = 0; i < _pts.size(); i++) pion.insert( pair<int, pair<int, int> >(_pts[i].second, _pts[i]));

            int current = 0;
            int firstActive = current;
            double delta = 10000;

            while (current < _pts.size())
            {
                pair<int, int> p = _pts[current];
                current++;

                /*/ print content:
                map<int, pair<int, int> >::iterator it;
                cout << "[ MAP ] ~ " << current << " : " << endl;
                for ( it=pion.begin() ; it != pion.end(); it++ )
                    cout << (*it).first << " => " << (*it).second << endl;
                cout << "\\___" << endl;
                //*/
                map<int, pair<int, int> >::iterator itlow, itup;

                itlow = pion.lower_bound(p.second);
                itup = pion.upper_bound(p.second);

                int i = 0;
                while (itlow != pion.end() and i < 4)
                {
                    //cout << " " << itlow->second.first << "x" << itlow->second.second;
                    if (sqrt(DIST2(p, itlow->second)) < delta) {
                        cout << "... changing distance (L): " << p << " and " << itlow->second << endl;
                        delta = sqrt(DIST2(p, itlow->second));
                        cout << "... changed to " << delta << endl;
                        P = p;  Q = itlow->second;
                    }
                    itlow--;
                    i++;
                }
                //cout << " ." << endl;

                i = 0;
                while (itup != pion.end()  &&  i < 4) {
                    if (sqrt(DIST2(p, itup->second)) < delta) {
                        cout << "... changing distance (U): " << p << " and " << itup->second << endl;
                        delta = sqrt(DIST2(p, itup->second));
                        cout << "... changed to " << delta << endl;
                        P = p;  Q = itup->second;
                    }
                    itup++;
                    i++;
                }

                cout << " activeBefore = " << firstActive << endl;
                int q = firstActive;
                //double sqrt_delta = sqrt(delta);
                while ((p.first-_pts[q].first) > delta)
                {
                    firstActive++;
                    pion.erase(_pts[q].second);
                    //avl.remove(pts[q]);
                    q = firstActive;
                }
                pion.insert( pair<int, pair<int, int> >(p.second, p));
                cout << " active After = " << firstActive << endl;
            }
#undef DIST2
            distance = delta;
        }

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
