#ifndef MOSAICGRAPH_HPP
#define MOSAICGRAPH_HPP

#include <cassert>

#include "../init/MosaicGraphNode.hpp"

class MosaicGraph
{
    public:
        MosaicGraph();
        virtual ~MosaicGraph();
        MosaicGraph(const MosaicGraph & other);

        inline unsigned int Size() { return nodes.size(); }
        inline MosaicGraphNode * Get(unsigned int i) { return nodes[i]; }

        unsigned int CreateNewNode(double, double);
        void AddEdge(unsigned int, unsigned int, int);

        void RemoveEdge(unsigned int, unsigned int);

        void SortNeighboursInCounterClockwiseOrder();

        friend std::ostream & operator << (std::ostream &, const MosaicGraph &);

    protected:
    private:
        std::vector<MosaicGraphNode *> nodes;
};

#endif // MOSAICGRAPH_HPP
