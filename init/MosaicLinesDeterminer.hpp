#ifndef MOSAICLINESDETERMINER_HPP
#define MOSAICLINESDETERMINER_HPP

#include <cmath>

#include <set>
#include <vector>
#include <utility>

#include "../init/MosaicConstants.hpp"
#include "../init/MosaicSegment.hpp"


class MosaicLinesDeterminer
{
    public:
        typedef std::vector<MosaicSegment<double> *>::const_iterator ConstIterator;

        MosaicLinesDeterminer(bool = false);
        virtual ~MosaicLinesDeterminer();

        void GenerateRandomSegmentsByPolarParameters (unsigned int, double, double);

        inline ConstIterator begin() const { return mosaic.begin(); }
        inline ConstIterator end()   const { return mosaic.end(); }

        inline size_t size()   const { return mosaic.size(); }

    protected:

    private:
        double Epsilon;
        std::vector<MosaicSegment<double> *> mosaic;

        bool debug;
};

#endif // MOSAICLINESDETERMINER_HPP
