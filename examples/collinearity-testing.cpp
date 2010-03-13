#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>

#include <iostream>
#include <utility>

using namespace std;

#define VAR(V,N)    __typeof(N) V = (N)
#define SIZE(X)     ((int)(X).size())
#define ALL(C)      (C).begin(), (C).end()

#define FOR(X,B,E)      for(int X = B; X <= (E); ++X)
#define REP(X,N)        for(int X = 0; X < (N); ++X)
#define FOREACH(I,C)    for(VAR(I, (C).begin()); I != (C).end(); ++I)


#define L2(_PX, _PY, _QX, _QY) sqrt((_PX-_QX)*(_PX-_QX)+(_PY-_QY)*(_PY-_QY))
void collinear(double xp, double yp, double xq, double yq, double xr, double yr)
{
    double epsilon = 1e-7;

    double D = (xq - xp)*(yr - yp) - (yq - yp)*(xr - xp);
    double M = abs(xq - xp)*abs(yr - yp) + abs(yq - yp)*abs(xr - xp);
    double b = max( max( L2(xp, yp, xq, yq), L2(xq, yq, xr, yr) ), L2(xr, yr, xp, yp) );

    double h = abs(0.5 * D) / b;
    double dh = 7. * epsilon * (M / b);

    printf(" E = %.25lf\n", epsilon);
    printf(" M = %.25lf\n", M);
    printf(" b = %.25lf\n", b);
    printf(" D = %.25lf\n", D);
    printf(" h = %.25lf\n", h);
    printf("dh = %.25lf\n", dh);
    printf("\n");
}
#undef L2


void solveCase()
{
    collinear( 0., 0.,    2., 0.,    1., 0.000000001);

    collinear( 0.34713217089273, 2.043234904132231,    0.347270127118174, 2.043202484831586,    0.34723120641024, 2.043615096879833);

    collinear( 1., 0.9999999,    2., 1.99999999999999,    3.000000000000000001, 3.00000000041);
    collinear( 1., 1.0000001,    2., 1.99999999999999,    3.000000000000000001, 3.00000000041);
}


int main()
{
    solveCase();
    return 0;
}
