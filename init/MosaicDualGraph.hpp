#ifndef MOSAICDUALGRAPH_HPP
#define MOSAICDUALGRAPH_HPP

#include <iostream>
#include <stack>
#include <set>
#include <map>
#include <utility>

#include "MosaicGraph.hpp"
#include "../cmd/grayimage.h"


class MosaicDualGraph
{
    public:
        MosaicDualGraph(MosaicGraph *);
        virtual ~MosaicDualGraph();

        void DetermineAreasColors(pmf::GrayscaleImage &);
        void CalculateComponents();

        friend std::ostream & operator << (std::ostream &, const MosaicDualGraph &);

    protected:
        int DetermineAreaColor(std::vector<int> &, pmf::GrayscaleImage &);
        void CountBlackAndWhitePixels(
                                            double ux, double uy, double unx, double uny,
                                            double dx, double dy, double dnx, double dny,
                                            pmf::GrayscaleImage & gimg,
                                            double xb, double xe
                                        );

    private:
        MosaicGraph * graph;

        std::vector<std::vector<int> > areas;
        std::vector<std::vector<int> > areaGraph;

        std::vector<int> areasColors;
};

#endif // MOSAICDUALGRAPH_HPP
