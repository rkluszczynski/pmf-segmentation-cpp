#include <iostream>
#include <fstream>
#include <vector>

#include "avl.hpp"

using namespace std;

//AVL<pair<int, int> > avl;
vector<pair<int, int> > pts;
int N;

#define DIST2(P1, P2) ((P1.second-P2.second)*(P1.second-P2.second)+(P1.first-P2.first)*(P1.first-P2.first))
int main()
{
    ifstream fin("tests/in10245.txt");  cin.rdbuf(fin.rdbuf());
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
        for (int i = 0; i < N; i++) cout << " " << pts[i].first;  cout << endl;

        AVL<pair<int, int> > avl;
        int current = 0;
        int firstActive = current;
        double delta = 100000000;
        pair<int, int> P, Q;

        while (current < pts.size()) {
            pair<int, int> p = pts[current];
            current++;
            struct Tnode<pair<int, int> > * r = avl.find(p);
            struct Tnode<pair<int, int> > * l = (r) ? r->pred : NULL;

            int i = 0;
            while (r  &&  i < 4) {
                if (DIST2(p, r->key) < delta) {
                    delta = DIST2(p, r->key);
                    P = p;  Q = r->key;
                }
                r = r->next;
                i++;
            }

            i = 0;
            while (l  &&  i < 4) {
                if (DIST2(p, l->key) < delta) {
                    delta = DIST2(p, l->key);
                    P = p;  Q = l->key;
                }
                l = l->prev;
                i++;
            }

            int q = firstActive;
            while (((pts[p].first-pts[q].first)*(pts[p].first-pts[q].first)) > delta)
            {
                firstActive++;
                avl.remove(pts[q]);
                q = firstActive;
            }

            avl.insert(p);
        }
    }
    return 0;
}
