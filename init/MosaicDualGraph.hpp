#ifndef MOSAICDUALGRAPH_HPP
#define MOSAICDUALGRAPH_HPP

#include <iostream>
#include <stack>
#include <set>
#include <map>
#include <utility>

#include "MosaicGraph.hpp"


class MosaicDualGraph
{
    public:
        MosaicDualGraph(MosaicGraph *);
        virtual ~MosaicDualGraph();

        void DetermineAreasColors();
        void CalculateComponents();

        friend std::ostream & operator << (std::ostream &, const MosaicDualGraph &);

    protected:
        int DetermineAreaColor(std::vector<int> &);

    private:
        MosaicGraph * graph;

        std::vector<std::vector<int> > areas;
        std::vector<std::vector<int> > areaGraph;

        std::vector<int> areasColors;
};

#endif // MOSAICDUALGRAPH_HPP
