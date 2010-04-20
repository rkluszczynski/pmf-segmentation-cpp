#include "MosaicLinesDeterminer.hpp"

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)

#define SHOW_COMMANDS 1


MosaicLinesDeterminer::MosaicLinesDeterminer(bool debug) : debug(debug)
{
    //ctor
    Epsilon = MosaicConstants::GetEpsilon();
}


MosaicLinesDeterminer::~MosaicLinesDeterminer()
{
    //dtor
}


void
MosaicLinesDeterminer::LoadSegmentsFromFile (const char * filename)
{
    ifstream fin;
    fin.open(filename);
    int amount;
    //double width, height;
    //fin >> width;
    //fin >> height;
    fin >> amount;
    for (int i = 0; i < amount; ++i)
    {
        double x1, y1, x2, y2;
        fin >> x1 >> y1 >> x2 >> y2;

        MosaicPoint<double> * p1 = new MosaicPoint<double>(x1, y1);
        MosaicPoint<double> * p2 = new MosaicPoint<double>(x2, y2);
        if (p1->x > p2->x  or  (p1->x == p2->x  and  p1->y > p2->y)) swap(p1, p2);

        MosaicSegment<double> * s = new MosaicSegment<double>(p1, p2);
        mosaic.push_back(s);

        if (debug)  cout << " added segment : " << s << endl;
    }
    fin.close();
}


    struct classcompx
    {
        bool operator() (const MosaicPoint<double> * lhs, const MosaicPoint<double> * rhs) const
        {
            return lhs->x < rhs->x;
        }
    };
    struct classcompy
    {
        bool operator() (const MosaicPoint<double> * lhs, const MosaicPoint<double> * rhs) const
        {
            return lhs->y < rhs->y;
        }
    };

#define UNIFORM(A, B)   (((double(rand()) + 1.) / (double(RAND_MAX) + 2.)) * ((B) - (A)) + (A))
#define DIST(AX, AY, BX, BY)   (sqrt( ((BX)-(AX))*((BX)-(AX)) + ((BY)-(AY))*((BY)-(AY)) ))
void
MosaicLinesDeterminer::GenerateRandomSegmentsByPolarParameters (unsigned int amount, double fieldHeight, double fieldWidth)
{
#if SHOW_COMMANDS
    int pointCounter = 0;
    ofstream fout("output/lines-ggb-cmds.txt");
#endif
    mosaic.clear();
    //*
    std::set<MosaicPoint<double> *, classcompx> setx;
    std::set<MosaicPoint<double> *, classcompy> sety;

    while (mosaic.size() < amount)
    {
        double alpha = UNIFORM(-M_PI_2, M_PI);
        if (fabs(alpha - M_PI_2) < Epsilon) continue;

        double maxD = 0.;
        double tgL = tan(alpha);
        if (alpha <= 0.)
            maxD = fabs(fieldWidth) / sqrt(tgL*tgL + 1);  // IV quarter (w * cosL)
        else if (alpha <= M_PI_2)
            maxD = fabs(fieldHeight * tgL + fieldWidth) / sqrt(tgL*tgL + 1);  // I quarter
        else if (alpha < M_PI)
            maxD = fabs(tgL * fieldHeight) / sqrt(tgL*tgL + 1);  // II quarter
        else
            assert("WRONG alpha !" && false);

        double d = UNIFORM(0., maxD);


        double a = - 1. / tan(alpha);
        double b = d / cos(M_PI_2 - alpha);

        double tmpx, tmpy;
        vector<pair<double, double> > seg;
        // left
        if (0. < b  and  b < fieldHeight)  seg.push_back(make_pair(0., b));
        // up
        tmpx = (fieldHeight - b) / a;
        if (0. < tmpx  and  tmpx < fieldWidth)  seg.push_back(make_pair(tmpx, fieldHeight));
        // right
        tmpy = a * fieldWidth + b;
        if (0. < tmpy  and  tmpy < fieldHeight)  seg.push_back(make_pair(fieldWidth, tmpy));
        // down
        tmpx = - b / a;
        if (0. < tmpx  and  tmpx < fieldWidth)  seg.push_back(make_pair(tmpx, 0.));

        assert(seg.size() == 2);

        MosaicPoint<double> * p1 = new MosaicPoint<double>(seg[0].first, seg[0].second);
        std::set<MosaicPoint<double> *, classcompx>::iterator it1x = setx.lower_bound(p1);
        if (it1x != setx.end())
            if (DIST(p1->x, p1->y, (*it1x)->x, (*it1x)->y) < Epsilon)
            {
                delete p1;
                continue;
            }
        it1x = setx.upper_bound(p1);
        if (it1x != setx.end())
            if (DIST(p1->x, p1->y, (*it1x)->x, (*it1x)->y) < Epsilon)
            {
                delete p1;
                continue;
            }
        std::set<MosaicPoint<double> *, classcompy>::iterator it1y = sety.lower_bound(p1);
        if (it1y != sety.end())
            if (DIST(p1->x, p1->y, (*it1y)->x, (*it1y)->y) < Epsilon)
            {
                delete p1;
                continue;
            }
        it1y = sety.upper_bound(p1);
        if (it1y != sety.end())
            if (DIST(p1->x, p1->y, (*it1y)->x, (*it1y)->y) < Epsilon)
            {
                delete p1;
                continue;
            }

        MosaicPoint<double> * p2 = new MosaicPoint<double>(seg[1].first, seg[1].second);
        std::set<MosaicPoint<double> *, classcompx>::iterator it2x = setx.lower_bound(p2);
        if (it2x != setx.end())
            if (DIST(p2->x, p2->y, (*it2x)->x, (*it2x)->y) < Epsilon)
            {
                delete p1;
                delete p2;
                continue;
            }
        it2x = setx.upper_bound(p2);
        if (it2x != setx.end())
            if (DIST(p2->x, p2->y, (*it2x)->x, (*it2x)->y) < Epsilon)
            {
                delete p1;
                delete p2;
                continue;
            }
        std::set<MosaicPoint<double> *, classcompy>::iterator it2y = sety.lower_bound(p2);
        if (it2y != sety.end())
            if (DIST(p2->x, p2->y, (*it2y)->x, (*it2y)->y) < Epsilon)
            {
                delete p1;
                delete p2;
                continue;
            }
        it2y = sety.upper_bound(p2);
        if (it2y != sety.end())
            if (DIST(p2->x, p2->y, (*it2y)->x, (*it2y)->y) < Epsilon)
            {
                delete p1;
                delete p2;
                continue;
            }

        setx.insert(p1);
        sety.insert(p1);
        setx.insert(p2);
        sety.insert(p2);

        if (p1->x > p2->x  or  (p1->x == p2->x  and  p1->y > p2->y)) swap(p1, p2);
        MosaicSegment<double> * s = new MosaicSegment<double>(p1, p2);
        mosaic.push_back(s);

#if SHOW_COMMANDS
        fout << "P" << pointCounter++ << "=(" << p1->x << "," << p1->y << ")" << endl;
        fout << "P" << pointCounter++ << "=(" << p2->x << "," << p2->y << ")" << endl;
        fout << "odcinek[P" << pointCounter-2 << ",P" << pointCounter-1 << "]" << endl;
#endif
        if (debug)  cout << " added segment : " << s << endl;
    }
}
#undef DIST
#undef UNIFORM


#define UNIFORM(A, B)   (((double(rand()) + 1.) / (double(RAND_MAX) + 2.)) * ((B) - (A)) + (A))
void
MosaicLinesDeterminer::GenerateRandomSegmentsByPolarParameters2 (unsigned int amount, double fieldHeight, double fieldWidth)
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

