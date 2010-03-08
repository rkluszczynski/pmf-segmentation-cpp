#ifndef MOSAICPMF_HPP
#define MOSAICPMF_HPP

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cassert>

#include <iostream>
#include <ostream>
#include <stack>
#include <set>
#include <map>

using namespace std;

//#include "../init/MosaicEvents.hpp"
//#include "../init/MosaicSegment.hpp"
#include "../init/MosaicSchedule.hpp"
#include "../init/MosaicStatus.hpp"
#include "../init/MosaicGraph.hpp"
#include "../init/MosaicDualGraph.hpp"
#include "../cmd/grayimage.h"


class MosaicPMF
{
    public:
        MosaicPMF(double, double, unsigned int, pmf::GrayscaleImage &);
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
        CalculateAreas (
                            MosaicGraph *,
                            std::vector<std::vector<int> > &,
                            std::vector<std::vector<int> > *
                        );
        MosaicGraph *
        GenerateSegmentsGraph (
                                MosaicEventsSchedule<double> *
                            );
        void
        GenerateRandomSegmentsByPolarParameters (
                                                    unsigned int, vector<MosaicSegment<double> *> &
                                                );
        void
        ProcessBeginSegmentEvent (
                                  MosaicSweepLineStatus<double> *,
                                  MosaicEventsSchedule<double> *,
                                  VirtualMosaicEvent *
                                );
        void
        AnalyzeAndPredictIntersection (
                                        MosaicSegment<double> *,
                                        MosaicSegment<double> *,
                                        MosaicEventsSchedule<double> *, double
                                    );
        void
        CheckIntersectionsAfterSwap (
                                        MosaicSweepLineStatus<double> *,
                                        MosaicSweepLineStatus<double>::Iterator &,
                                        MosaicSweepLineStatus<double>::Iterator &,
                                        MosaicSegment<double> *,
                                        MosaicEventsSchedule<double> *,
                                        double,
                                        int
                                    );

        double fieldWidth;
        double fieldHeight;
        unsigned int linesNumber;

};

#endif // MOSAICPMF_HPP
