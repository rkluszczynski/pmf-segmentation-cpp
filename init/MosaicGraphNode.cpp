#include "MosaicGraphNode.hpp"


MosaicGraphNode::MosaicGraphNode (double xx, double yy, int iid) : x(xx), y(yy), id(iid)
{
    //ctor
}


MosaicGraphNode::~MosaicGraphNode ()
{
    //dtor
}



struct CounterClockwiseComparator
{
    public :
        CounterClockwiseComparator (std::vector<MosaicGraphNode *> & nnodes, int iid) : nodes(nnodes), id(iid) {}

        bool operator() (std::pair<int, int> pair_i, std::pair<int, int> pair_j)
        {
            double ix = nodes[pair_i.first]->x - nodes[id]->x;
            double iy = nodes[pair_i.first]->y - nodes[id]->y;
            double jx = nodes[pair_j.first]->x - nodes[id]->x;
            double jy = nodes[pair_j.first]->y - nodes[id]->y;

            double iAngle = atan2(iy, ix);
            double jAngle = atan2(jy, jx);

            return iAngle < jAngle; // (-PI,PI] increasing angle values
        }

    private :
        std::vector<MosaicGraphNode *> nodes;
        int id;
};


void
MosaicGraphNode::SortNeighboursInCounterClockwiseOrder (std::vector<MosaicGraphNode *> & nodes)
{
    struct CounterClockwiseComparator myComparator(nodes, id);
    std::sort(n.begin(), n.end(), myComparator);
}


std::ostream & operator << (std::ostream & out, const MosaicGraphNode * node)
{
    out.precision(3);
    out.setf ( std::ios::fixed, std::ios::floatfield );
    out << "[" << node->x << ";" << node->y << "] :";
    out.unsetf ( std::ios::floatfield  );
    FOREACH(it, node->n) out << " " << (*it).first << "{" << (*it).second << "}";
    out << std::endl;
    return out;
}


MosaicGraphNode::MosaicGraphNode(const MosaicGraphNode & other)
{
    //copy ctor
    x = other.x;
    y = other.y;
    id = other.id;
    n.assign(other.n.begin(), other.n.end());
}
