#ifndef MOSAICGRAPH_HPP
#define MOSAICGRAPH_HPP

#include <cassert>

#include "../init/MosaicGraphNode.hpp"

class MosaicGraph
{
    public:
        MosaicGraph();
        virtual ~MosaicGraph();

        unsigned int CreateNewNode(double, double);
        void AddEdge(unsigned int, unsigned int);

        friend std::ostream & operator << (std::ostream &, const MosaicGraph &);

    protected:
    private:
        std::vector<MosaicGraphNode *> nodes;
};

#endif // MOSAICGRAPH_HPP
