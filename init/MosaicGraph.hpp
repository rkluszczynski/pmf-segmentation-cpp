#ifndef MOSAICGRAPH_HPP
#define MOSAICGRAPH_HPP

#include <cassert>
#include <vector>

#include "../init/MosaicGraphNode.hpp"

class MosaicGraph
{
    public:
        MosaicGraph();
        virtual ~MosaicGraph();
        MosaicGraph(const MosaicGraph & other);

        inline unsigned int Size() { return nodes.size(); }
        inline MosaicGraphNode * Get(unsigned int i) { return nodes[i]; }

        //void RemoveEdge(unsigned int, unsigned int);

        unsigned int CreateNewNode(double, double);

        void AddEdge(unsigned int, unsigned int, int);
        void AddListEdge(unsigned int, unsigned int, int);

        void RemoveEdge(unsigned int, unsigned int);
        void RemoveListEdge(unsigned int, MosaicGraphNode::Iterator &, unsigned int);

        void RemoveNeighboursOf(unsigned int, std::vector<int> &);
        void RemoveUnnecessaryCollinearNodes();

        void SortNeighboursInCounterClockwiseOrder();
        void SaveAsGeoGebraFile(char *);

        friend std::ostream & operator << (std::ostream &, const MosaicGraph &);

    protected:
    private:
        std::vector<MosaicGraphNode *> nodes;

};

#endif // MOSAICGRAPH_HPP
