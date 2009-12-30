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
    //*
    while (linesParameters.size() < n)
    {
        double alpha = UNIFORM(-M_PI_2, M_PI);
        if (fabs(alpha - M_PI_2) < 1e-7) continue;

        double maxD = 0.;
        double a = tan(alpha);
        if (alpha <= 0.)
            maxD = fabs(w) / sqrt(a*a + 1);  // IV quarter (w * cosL)
        else if (alpha <= M_PI_2)
            maxD = fabs(h * a + w) / sqrt(a*a + 1);  // I quarter
        else if (alpha < M_PI)
            maxD = fabs(a * h) / sqrt(a*a + 1);  // II quarter
        else
            assert("WRONG alpha !" && false);

        double d = UNIFORM(0., maxD);
        linesParameters.insert( make_pair(alpha, d) );

        ///printf("... added :   L = %6.3lf   D = %6.3lf   d = %6.3lf \n", alpha, maxD, d);
    }
    // */
    /*
    linesParameters.insert(make_pair(0.75 * M_PI, 1));
    linesParameters.insert(make_pair(0.25 * M_PI, 1));
    linesParameters.insert(make_pair(-0.25 * M_PI, 1));
    // */

    vector<MosaicSegment<double> *> mosaic;

    FOREACH(it, linesParameters)
    {
        ///cout << endl << " ->   L = " << it->ST << "   d = " << it->ND << endl;

        double a = - 1. / tan(it->ST);
        double b = it->ND / cos(M_PI_2 - it->ST);
        /*
        if (it->ST < 0.)
            b = it->ND / cos(M_PI_2 - it->ST);
        else if (it->ST < M_PI_2)
            b = it->ND / cos(M_PI_2 - it->ST);
        else if (it->ST < M_PI)
            b = it->ND / cos(it->ST - M_PI_2);
        else
            assert(false);
        // */
        ///printf("  >   y  =  %.2lf  *  x  +  %.2lf\n", a, b);

        double tmpx, tmpy;
        vector<pair<double, double> > seg;
        // left
        if (0. < b  and  b < h)
        {
            ///cout << " ( " << 0. << " , " << b << " )" << endl;
            seg.push_back(make_pair(0., b));
        }
        // up
        tmpx = (h - b) / a;
        if (0. < tmpx  and  tmpx < w)
        {
            ///cout << " ( " << tmpx << " , " << h << " )" << endl;
            seg.push_back(make_pair(tmpx, h));
        }
        // right
        tmpy = a * w + b;
        if (0. < tmpy  and  tmpy < h)
        {
            ///cout << " ( " << w << " , " << tmpy << " )" << endl;
            seg.push_back(make_pair(w, tmpy));
        }
        // down
        tmpx = - b / a;
        if (0. < tmpx  and  tmpx < w)
        {
            ///cout << " ( " << tmpx << " , " << 0. << " )" << endl;
            seg.push_back(make_pair(tmpx, 0.));
        }
        assert(seg.size() == 2);

        MosaicPoint<double> * p1 = new MosaicPoint<double>(seg[0].ST, seg[0].ND);
        MosaicPoint<double> * p2 = new MosaicPoint<double>(seg[1].ST, seg[1].ND);
        if (p1 > p2) swap(p1, p2);

        MosaicSegment<double> * s = new MosaicSegment<double>(p1, p2);
        mosaic.push_back(s);
    }


    MosaicEventsSchedule<double> * evts = new MosaicEventsSchedule<double>();
    FOREACH(it, mosaic)
    {
        MosaicSegment<double> * seg = * it;

        BeginEvent * e1 = new BeginEvent(seg->GetLeftPoint(), seg);
        EndEvent * e2 = new EndEvent(seg->GetRightPoint(), seg);

        evts->Insert(e1);
        evts->Insert(e2);
    }

    cout << evts << endl;
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
