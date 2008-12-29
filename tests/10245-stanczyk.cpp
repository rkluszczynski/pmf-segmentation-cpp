/* Keywords: closest pair problem, divide-and-conquer
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>

using namespace std;

#define VAR(V,N)    __typeof(N) V = (N)
#define SIZE(X)     ((int)(X).size())
#define ALL(C)      (C).begin(), (C).end()

#define FOR(X,B,E)      for(int X = B; X <= (E); ++X)
#define REP(X,N)        for(int X = 0; X < (N); ++X)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)

#define PB  push_back

#define sqr(X) ((X)*(X))
#define INF 10000

#define TYPE double

struct POINT
{
    TYPE x, y;
    POINT(TYPE x = 0, TYPE y = 0) : x(x), y(y) { }
    bool operator ==(POINT & a) { return a.x == x && a.y == y; }

};
// Operator okreslajacy liniowy porzadek na zbiorze punkt�w po wsp�rzednych (x, y)
bool OrdXY(POINT * a, POINT * b) { return a->x == b->x ? a->y < b->y : a->x < b->x; }
// Operator okreslajacy liniowy porzadek na zbiorze punkt�w po wsp�rzednych (y, x)
bool OrdYX(POINT * a, POINT * b) { return a->y == b->y ? a->x < b->x : a->y < b->y; }


struct NearestPoints
{
    vector<POINT*> l;
    POINT *p1, *p2;
    double dist;

    // Funkcja usuwa z listy l wszystkie punkty, kt�rych odleg�osc
    // od prostej x=p jest wieksza od odleg�osci miedzy para aktualnie
    // znalezionych najblizszych punkt�w
    void Filter(vector<POINT*> &l, double p)
    {
        int s = 0;
        REP(x, SIZE(l))  if (sqr(l[x]->x - p) <= dist) l[s++] = l[x];
        l.resize(s);
    }

    // Funkcja realizuje faze dziel i zwyciezaj dla zbioru punkt�w z wektora l od pozycji p do k.
    // Wektor ys zawiera punkty z przetwarzanego zbioru posortowane w kolejnosci niemalejacych wsp�rzednych y
    void Calc(int p, int k, vector<POINT*> &ys)
    {
        //Jesli zbi�r zawiera wiecej niz jeden punkt, to nastepuje faza podzia�u
        if (k - p > 1)
        {
            vector<POINT*> lp, rp;
            // Wyznacz punkt podzia�u zbioru
            int c = (k + p - 1) / 2;
            // Podziel wektor ys na dwa zawierajace odpowiednio punkty na lewo oraz na prawo od punktu l[c]
            FOREACH(it, ys)
                if (OrdXY(l[c], *it)) rp.PB(*it); else lp.PB(*it);
            // Wykonaj faze podzia��w
            Calc(p, c + 1, lp);
            Calc(c + 1, k, rp);
            // Nastepuje faza scalania. Najpierw z wektor�w l i r usuwane
            // sa punkty po�ozone zbyt daleko od prostej wyznaczajacej podzia� zbior�w
            Filter(lp, l[c]->x);
            Filter(rp, l[c]->x);

            int p = 0; double k;
            // Faza wyznaczania odleg�osci pomiedzy kolejnymi parami punkt�w, kt�re moga polepszyc aktualny wynik
            FOREACH(it, lp)
            {
                while (p < SIZE(rp) - 1 && rp[p + 1]->y < (*it)->y) p++;
                FOR(x, max(0, p - 2), min(SIZE(rp) - 1, p + 1))
                // Jesli odleg�osc miedzy para przetwarzanych punkt�w jest mniejsza od aktualnego wyniku,
                // to zaktualizuj wynik
                if (dist > (k = sqr((*it)->x - rp[x]->x) + sqr((*it)->y - rp[x]->y)))
                {
                    dist = k;
                    p1 = *it;
                    p2 = rp[x];
                }
            }
        }
    }

    NearestPoints(vector<POINT> &p)
    {
        // Wype�nij wektor l wskaznikami do punkt�w z wektora p
        // oraz posortuj te wskazniki w kolejnosci niemalejacych wsp�rzednych x
        FOREACH(it, p) l.PB(&(*it));
        sort(ALL(l), OrdXY);
        // Jesli w zbiorze istnieja dwa punkty o tych samych wsp�rzednych, to punkty te sa poszukiwanym wynikiem
        FOR(x, 1, SIZE(l) - 1)
            if (l[x - 1]->x == l[x]->x && l[x - 1]->y == l[x]->y)
            {
                dist = 0;
                p1 = l[x - 1];
                p2 = l[x];
                return;
            }

        dist = double(INF) * double(INF);
        // Skonstruuj kopie wektora wskaznik�w do punkt�w i posortuj w kolejnosci niemalejacych wsp�rzednych y
        vector<POINT*> v = l;
        sort(ALL(v), OrdYX);
        // Wykonaj faze dziel i rzadz dla wszystkich punkt�w ze zbioru
        Calc(0, SIZE(l), v);
        dist = sqrt(dist);
    }
};


int main()
{
    //freopen("tests/in10245.txt", "r", stdin);
    int N;
    while(true) {
        scanf("%i", &N);
        if (N == 0) break;

        vector<POINT> l;
        POINT p;
        REP(i, N) {
            scanf("%lf%lf", &(p.x), &(p.y));
            l.PB(p);
        }

        NearestPoints str(l);
        if (str.dist < INF) printf("%.4lf\n", str.dist); else printf("INFINITY\n");
        /*
        cout << "Wyznaczona odleglosc: " << str.dist << endl;
        cout << "Znaleziona para najblizszych punktow:" << endl;
        cout << " (" << (str.p1)->x << ";" << (str.p1)->y << ") ";
        cout << " (" << (str.p2)->x << ";" << (str.p2)->y << ") ";
        cout << endl;
        //*/
    }
    return 0;
}
