#include "MosaicPMF.hpp"

//#include "../headers/probability.hpp"

#define ST first
#define ND second

#define REP(X, N)       for(int X = 0; X < (N); ++X)
#define FOR(X, B, E)    for(int X = B; X <= (E); ++X)
#define FORD(X, B, E)   for(int X = B; X >= (E); --X)

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)

#define UNIFORM(A, B)   (((double(rand()) + 1.) / (double(RAND_MAX) + 2.)) * ((B) - (A)) + (A))



MosaicPMF::MosaicPMF(double w, double h, unsigned int n) : fieldWidth(w), fieldHeight(h), linesNumber(n)
{
    //ctor
    while (linesParameters.size() < n)
    {
        double alpha = UNIFORM(-M_PI_2, M_PI);
        if (fabs(alpha - M_PI_2) < 1e-7) continue;

        double maxD = 0.;
        double a = tan(alpha);
        if (alpha <= 0.)
            maxD = fabs(w) / sqrt(a*a + 1);  // IV quarter
        else if (alpha <= M_PI_2)
            maxD = fabs(h * a + w) / sqrt(a*a + 1);  // I quarter
        else if (alpha < M_PI)
            maxD = fabs(a * h) / sqrt(a*a + 1);  // II quarter
        else
            assert("WRONG alpha !" && false);

        double d = UNIFORM(0., maxD);

        pair<double, double> line = make_pair(alpha, d);
        linesParameters.insert(line);
    }

    FOREACH(it, linesParameters)
    {
        cout << " ->   L = " << it->ST << "   d = " << it->ND << endl;

        double a = - 1. / tan(it->ST);
        double b;
        if (it->ST < 0.  or  M_PI_2 < it->ST)  b = it->ND / cos(it->ST);
        else  b = - it->ND / cos(it->ST);

        printf("  >   y  =  %.2lf  *  x  +  %.2lf\n", a, b);

        double tmpx, tmpy;
        int check = 0;
        // left
        if (0. < b  and  b < h)
        {
            cout << " ( " << 0. << " , " << b << " )" << endl;
            ++check;
        }
        // up
        tmpx = (h - b) / a;
        if (0. < tmpx  and  tmpx < w)
        {
            cout << " ( " << tmpx << " , " << h << " )" << endl;
            ++check;
        }
        // right
        tmpy = a * w + b;
        if (0. < tmpy  and  tmpy < h)
        {
            cout << " ( " << w << " , " << tmpy << " )" << endl;
            ++check;
        }
        // down
        tmpx = - b / a;
        if (0. < tmpx  and  tmpx < w)
        {
            cout << " ( " << tmpx << " , " << 0. << " )" << endl;
            ++check;
        }
        //assert(check == 2);


        cout << endl;
    }
}

MosaicPMF::~MosaicPMF()
{
    //dtor
}

MosaicPMF::MosaicPMF(const MosaicPMF& other)
{
    //copy ctor
}

MosaicPMF& MosaicPMF::operator=(const MosaicPMF& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
