#ifndef MOSAICGRAPHNODE_HPP
#define MOSAICGRAPHNODE_HPP

#include <vector>
#include <iostream>

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)

class MosaicGraphNode
{
    public:
        double x, y;
        std::vector<int> n;

        MosaicGraphNode(double, double);
        virtual ~MosaicGraphNode();

        friend std::ostream & operator << (std::ostream &, const MosaicGraphNode *);

    protected:
    private:
};

#endif // MOSAICGRAPHNODE_HPP
