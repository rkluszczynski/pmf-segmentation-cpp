#ifndef MOSAICGRAPHNODE_HPP
#define MOSAICGRAPHNODE_HPP

#include <cmath>
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>

#include "MosaicGraphEdge.hpp"

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)


class MosaicGraphNode
{
    public :
        typedef  std::list<MosaicGraphEdge *>::iterator Iterator;
        typedef  std::list<MosaicGraphEdge *>::const_iterator ConstIterator;


        MosaicGraphNode(double, double, unsigned int);
        MosaicGraphNode(const MosaicGraphNode & other);
        virtual ~MosaicGraphNode();

        inline ConstIterator begin() const { return nlist.begin(); }
        inline ConstIterator end()   const { return nlist.end(); }

        inline Iterator Begin() { return nlist.begin(); }
        inline Iterator End()   { return nlist.end(); }

        inline MosaicGraphEdge * Front() { return nlist.front(); }
        inline MosaicGraphEdge * Back()  { return nlist.back(); }

        inline double x() { return _x; }
        inline double y() { return _y; }

        inline unsigned int GetId() { return id; }
        void SetId(unsigned int val) { id = val; }

        inline unsigned int Size() { return nlist.size(); }

        Iterator AddNeighbor(MosaicGraphEdge *);
        void EraseNeighbor(Iterator &);

        void SortNodesNeighborsInCounterClockwiseOrder(std::vector<MosaicGraphNode *> &);

        friend std::ostream & operator << (std::ostream &, const MosaicGraphNode *);

    protected :

    private :
        double _x, _y;
        std::list<MosaicGraphEdge *> nlist;
        unsigned int id;
};

#endif // MOSAICGRAPHNODE_HPP
