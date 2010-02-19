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

        bool operator() (int i, int j)
        {
            double ix = nodes[i]->x - nodes[id]->x;
            double iy = nodes[i]->y - nodes[id]->y;
            double jx = nodes[j]->x - nodes[id]->x;
            double jy = nodes[j]->y - nodes[id]->y;

            double iAngle = atan2(iy, ix);
            double jAngle = atan2(jy, jx);

            return iAngle < jAngle; // (-PI,PI] increasing angle values
            /*
            double areaTrapezoid = ix * jy - iy * jx;
            //areaTriangle = areaTrapezoid / 2.0
            //LeftTurn = areaTrapezoid > 0.0
            //CompareAngle =
            if (areaTrapezoid > 0.) return 1;
            else if (areaTrapezoid == 0.) return 0;
            else return -1;
            // */
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
    out.setf( std::ios::fixed, std::ios::floatfield );
    out << "[" << node->x << ";" << node->y << "] :";
    out.unsetf ( std::ios::floatfield  );
    FOREACH(it, node->n) out << " " << *it;
    out << std::endl;
    return out;
}

