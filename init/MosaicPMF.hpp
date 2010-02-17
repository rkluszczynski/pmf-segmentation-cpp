#ifndef MOSAICPMF_HPP
#define MOSAICPMF_HPP

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cassert>

#include <iostream>
#include <ostream>
#include <set>

using namespace std;

//#include "../init/MosaicEvents.hpp"
//#include "../init/MosaicSegment.hpp"
#include "../init/MosaicSchedule.hpp"
#include "../init/MosaicStatus.hpp"


class MosaicPMF
{
    public:
        MosaicPMF(double, double, unsigned int);
        virtual ~MosaicPMF();
        MosaicPMF(const MosaicPMF& other);
        MosaicPMF& operator=(const MosaicPMF& other);

        friend ostream & operator << (ostream & out, const MosaicPMF * mpmf)
        {
            out << "[ MOSAIC ]" << endl;
            return out;
        }


    protected:

    private:

        void
        AnalyzeAndPredictIntersection (MosaicSegment<double> *, MosaicSegment<double> *, MosaicEventsSchedule<double> *);
        void
        CheckIntersectionsAfterSwap (
                                        MosaicSweepLineStatus<double> *,
                                        MosaicSweepLineStatus<double>::Iterator &,
                                        MosaicSweepLineStatus<double>::Iterator &,
                                        MosaicSegment<double> *,
                                        MosaicEventsSchedule<double> *,
                                        int
                                    );

        double fieldWidth;
        double fieldHeight;
        unsigned int linesNumber;

        set<pair<double, double> > linesParameters;

};

#endif // MOSAICPMF_HPP
