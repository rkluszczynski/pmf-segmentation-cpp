#include "MosaicLinesDeterminer.hpp"

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)


MosaicLinesDeterminer::MosaicLinesDeterminer(bool debug) : debug(debug)
{
    //ctor
    Epsilon = MosaicConstants::GetEpsilon();
}


MosaicLinesDeterminer::~MosaicLinesDeterminer()
{
    //dtor
}


#define UNIFORM(A, B)   (((double(rand()) + 1.) / (double(RAND_MAX) + 2.)) * ((B) - (A)) + (A))
void
MosaicLinesDeterminer::GenerateRandomSegmentsByPolarParameters (unsigned int amount, double fieldHeight, double fieldWidth)
{
    std::set<std::pair<double, double> > linesParameters;
    //*
    while (linesParameters.size() < amount)
    {
        double alpha = UNIFORM(-M_PI_2, M_PI);
        if (fabs(alpha - M_PI_2) < Epsilon) continue;

        double maxD = 0.;
        double a = tan(alpha);
        if (alpha <= 0.)
            maxD = fabs(fieldWidth) / sqrt(a*a + 1);  // IV quarter (w * cosL)
        else if (alpha <= M_PI_2)
            maxD = fabs(fieldHeight * a + fieldWidth) / sqrt(a*a + 1);  // I quarter
        else if (alpha < M_PI)
            maxD = fabs(a * fieldHeight) / sqrt(a*a + 1);  // II quarter
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

    mosaic.clear();
    FOREACH(it, linesParameters)
    {
        ///cout << endl << " ->   L = " << it->ST << "   d = " << it->ND << endl;

        double a = - 1. / tan(it->first);
        double b = it->second / cos(M_PI_2 - it->first);
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
        if (0. < b  and  b < fieldHeight)
        {
            ///cout << " ( " << 0. << " , " << b << " )" << endl;
            seg.push_back(make_pair(0., b));
        }
        // up
        tmpx = (fieldHeight - b) / a;
        if (0. < tmpx  and  tmpx < fieldWidth)
        {
            ///cout << " ( " << tmpx << " , " << h << " )" << endl;
            seg.push_back(make_pair(tmpx, fieldHeight));
        }
        // right
        tmpy = a * fieldWidth + b;
        if (0. < tmpy  and  tmpy < fieldHeight)
        {
            ///cout << " ( " << w << " , " << tmpy << " )" << endl;
            seg.push_back(make_pair(fieldWidth, tmpy));
        }
        // down
        tmpx = - b / a;
        if (0. < tmpx  and  tmpx < fieldWidth)
        {
            ///cout << " ( " << tmpx << " , " << 0. << " )" << endl;
            seg.push_back(make_pair(tmpx, 0.));
        }
        assert(seg.size() == 2);

        MosaicPoint<double> * p1 = new MosaicPoint<double>(seg[0].first, seg[0].second);
        MosaicPoint<double> * p2 = new MosaicPoint<double>(seg[1].first, seg[1].second);
        if (p1->x > p2->x  or  (p1->x == p2->x  and  p1->y > p2->y)) swap(p1, p2);

        MosaicSegment<double> * s = new MosaicSegment<double>(p1, p2);
        mosaic.push_back(s);

        if (debug)  cout << " added segment : " << s << endl;
    }
}
#undef UNIFORM

