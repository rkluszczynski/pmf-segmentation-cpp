#ifndef MOSAICGRAPH_HPP
#define MOSAICGRAPH_HPP

#include <cassert>
#include <vector>

#include "../init/GaussianRandomClass.hpp"
#include "../init/MosaicConstants.hpp"
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
        void RemoveEdge(unsigned int, MosaicGraphNode::Iterator &, unsigned int);

        void RemoveNeighborsOf(unsigned int, std::vector<int> &);
        void RemoveUnnecessaryCollinearNodes(bool = false);
        void MutateIntersectionElements();
        void MakeGaussianShakeToDisorder(double);

        void SortNeighborsInCounterClockwiseOrder();
        void SaveAsGeoGebraFile(const char *);
        void SaveAsTextFile(const char *);

        friend std::ostream & operator << (std::ostream &, const MosaicGraph &);

    protected:

    private:
        std::vector<MosaicGraphNode *> nodes;
};

#endif // MOSAICGRAPH_HPP
