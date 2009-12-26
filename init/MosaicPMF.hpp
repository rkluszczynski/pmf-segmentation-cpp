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

        double fieldWidth;
        double fieldHeight;
        unsigned int linesNumber;

        set<pair<double, double> > linesParameters;

};

#endif // MOSAICPMF_HPP
