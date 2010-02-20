#ifndef MOSAICGRAPHNODE_HPP
#define MOSAICGRAPHNODE_HPP

#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)

class MosaicGraphNode
{
    public:
        double x, y;
        std::vector<std::pair<int, int> > n;
        int id;

        MosaicGraphNode(double, double, int);
        virtual ~MosaicGraphNode();
        MosaicGraphNode(const MosaicGraphNode & other);

        void SortNeighboursInCounterClockwiseOrder(std::vector<MosaicGraphNode *> &);

        friend std::ostream & operator << (std::ostream &, const MosaicGraphNode *);

    protected:
    private:
};

#endif // MOSAICGRAPHNODE_HPP
