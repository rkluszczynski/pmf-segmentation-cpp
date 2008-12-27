#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "avl.hpp"

using namespace std;

#include <map>
class ClosestPair
{
    private :
        vector<pair<int, int> > _pts;

        struct comp_map {
            bool operator() (const int& lhs, const int& rhs) const { return lhs<rhs; }
        };


    public :
        ClosestPair(int n = 0) : _pts(n) { }

        void PushBack(pair<int, int> & pp) { _pts.push_back(pp); }
        void SortPoints() { sort(_pts.begin(), _pts.end()); }
        void Execute()
        {
            map<int, pair<int, int> > pion;

            int current = 0;
            int firstActive = current;
            double delta = 100000000;
            pair<int, int> P, Q;

            while (current < _pts.size())
            {
                pair<int, int> p = _pts[current];
                current++;

                map<int, pair<int, int> >::iterator itlow, itup;

                itlow = pion.lower_bound(p.second);
                itup = pion.upper_bound(p.second);

                int i = 0;
                /*
                while (r  &&  i < 4) {
                    if (sqrt(DIST2(p, r->key)) < delta) {
                        cout << "... changing distance : " << p << " and " << r->key << endl;
                        delta = sqrt(DIST2(p, r->key));
                        cout << "... changed to " << delta << endl;
                        P = p;  Q = r->key;
                    }
                    r = r->next;
                    i++;
                }
                //*/
            }
/*


            i = 0;
            while (l  &&  i < 4) {
                if (sqrt(DIST2(p, l->key)) < delta) {
                    cout << "... changing distance : " << p << " and " << l->key << endl;
                    delta = sqrt(DIST2(p, l->key));
                    cout << "... changed to " << delta << endl;
                    P = p;  Q = l->key;
                }
                l = l->prev;
                i++;
            }

            cout << " activeBefore = " << firstActive << endl;
            int q = firstActive;
            //double sqrt_delta = sqrt(delta);
            while ((p.first-pts[q].first) > delta)
            {
                firstActive++;
                avl.remove(pts[q]);
                q = firstActive;
            }
            avl.insert(p);

            cout << " active After = " << firstActive << endl;
            avl.print();
        }

        avl.print();
        cout << " RESULT = " << delta << "  ... by " << P << " and " << Q << endl;
//*/
        }

        pair<int, int> & operator[] (const int index) { return _pts[index]; }
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

    }
    return 0;
}




class PAVL : public AVL<pair<int, int> >
{
    private :
        virtual inline bool __less_then (pair<int, int> p1, pair<int, int> p2)
        {
            return p1.second < p2.second;
        };
};

std::ostream & operator << (std::ostream& s, const pair<int,int> & p)
{
    s << p.first << " x " << p.second;
    return s;
}


//AVL<pair<int, int> > avl;
vector<pair<int, int> > pts;

#define DIST2(P1, P2) ((P1.second-P2.second)*(P1.second-P2.second)+(P1.first-P2.first)*(P1.first-P2.first))
int main2()
{
    ifstream fin("tests/in10245.txt");  cin.rdbuf(fin.rdbuf());
    int N;
    while(true) {
        cin >> N;
        if (N == 0) break;

        pts.clear();
        for (int i = 0; i < N; i++) {
            pair<int, int> pp;
            cin >> pp.first;
            cin >> pp.second;
            pts.push_back(pp);
        }
        sort(pts.begin(), pts.end());
        for (int i = 0; i < N; i++) cout << " " << pts[i].first << "x" << pts[i].second;  cout << endl;

        PAVL avl;
        int current = 0;
        int firstActive = current;
        double delta = 100000000;
        pair<int, int> P, Q;

        while (current < pts.size())
        {
            pair<int, int> p = pts[current];
            current++;
            Tnode<pair<int, int> > * l = avl.findLE(p);
            Tnode<pair<int, int> > * r = (l) ? l->next : avl.get_min();

            cout << " p = " << p.first << " x " << p.second << endl;
            cout << " current = " << current << endl;
            cout << " l = "; avl.print_node(l);
            cout << " r = "; avl.print_node(r);
            cout << " delta = " << delta << endl;

            int i = 0;
            while (r  &&  i < 4) {
                if (sqrt(DIST2(p, r->key)) < delta) {
                    cout << "... changing distance : " << p << " and " << r->key << endl;
                    delta = sqrt(DIST2(p, r->key));
                    cout << "... changed to " << delta << endl;
                    P = p;  Q = r->key;
                }
                r = r->next;
                i++;
            }

            i = 0;
            while (l  &&  i < 4) {
                if (sqrt(DIST2(p, l->key)) < delta) {
                    cout << "... changing distance : " << p << " and " << l->key << endl;
                    delta = sqrt(DIST2(p, l->key));
                    cout << "... changed to " << delta << endl;
                    P = p;  Q = l->key;
                }
                l = l->prev;
                i++;
            }

            cout << " activeBefore = " << firstActive << endl;
            int q = firstActive;
            //double sqrt_delta = sqrt(delta);
            while ((p.first-pts[q].first) > delta)
            {
                firstActive++;
                avl.remove(pts[q]);
                q = firstActive;
            }
            avl.insert(p);

            cout << " active After = " << firstActive << endl;
            avl.print();
        }

        avl.print();
        cout << " RESULT = " << delta << "  ... by " << P << " and " << Q << endl;
    }
    return 0;
}
