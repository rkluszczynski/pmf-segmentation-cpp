#ifndef MOSAICGRAPHNODE_HPP
#define MOSAICGRAPHNODE_HPP

#include <cmath>
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)

class MosaicGraphNode
{
    typedef  std::list<MosaicGraphNode *>::const_iterator Iterator;

    public :
        std::vector<std::pair<int, int> > n;

        MosaicGraphNode(double, double, int);
        MosaicGraphNode(const MosaicGraphNode & other);
        virtual ~MosaicGraphNode();

        inline Iterator begin() const { return nlist.begin(); }
        inline Iterator end()   const { return nlist.end(); }

        inline double x() { return _x; }
        inline double y() { return _y; }

        inline unsigned int GetId() { return id; }

        void SortNeighboursInCounterClockwiseOrder(std::vector<MosaicGraphNode *> &);

        friend std::ostream & operator << (std::ostream &, const MosaicGraphNode *);

    protected :

    private :
        double _x, _y;
        std::list<MosaicGraphNode *> nlist;
        unsigned int id;
};

#endif // MOSAICGRAPHNODE_HPP
