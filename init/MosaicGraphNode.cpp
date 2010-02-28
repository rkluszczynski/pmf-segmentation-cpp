#include "MosaicGraphNode.hpp"


MosaicGraphNode::MosaicGraphNode (double xx, double yy, unsigned int iid) : _x(xx), _y(yy), id(iid)
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
            double ix = nodes[pair_i.first]->x() - nodes[id]->x();
            double iy = nodes[pair_i.first]->y() - nodes[id]->y();
            double jx = nodes[pair_j.first]->x() - nodes[id]->x();
            double jy = nodes[pair_j.first]->y() - nodes[id]->y();

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


struct CounterClockwiseListComparator
{
    public :
        CounterClockwiseListComparator (std::vector<MosaicGraphNode *> & nnodes, int iid) : nodes(nnodes), id(iid) {}

        bool operator() (MosaicGraphEdge * edge_i, MosaicGraphEdge * edge_j)
        {
            double ix = nodes[edge_i->GetId()]->x() - nodes[id]->x();
            double iy = nodes[edge_i->GetId()]->y() - nodes[id]->y();
            double jx = nodes[edge_j->GetId()]->x() - nodes[id]->x();
            double jy = nodes[edge_j->GetId()]->y() - nodes[id]->y();

            double iAngle = atan2(iy, ix);
            double jAngle = atan2(jy, jx);

            return iAngle < jAngle; // (-PI,PI] increasing angle values
        }

    private :
        std::vector<MosaicGraphNode *> & nodes;
        int id;
};

void
MosaicGraphNode::SortListNeighboursInCounterClockwiseOrder (std::vector<MosaicGraphNode *> & nodes)
{
    const unsigned int size = nlist.size();
    MosaicGraphEdge * neighbours[size];
    int amount = size;
    while (amount--)
    {
        neighbours[amount] = nlist.front();
        nlist.pop_front();
    }

    struct CounterClockwiseListComparator myListComparator(nodes, id);
    //std::sort(n.begin(), n.end(), myComparator);
    std::sort(neighbours, neighbours + size, myListComparator);

    for (unsigned int i = 0; i < size; ++i)
    {
        nlist.push_back(neighbours[i]);
        MosaicGraphNode::Iterator it = nlist.end();
        --it;
        (* neighbours[i]->GetOther())->SetOther(it);
    }
}


MosaicGraphNode::Iterator
MosaicGraphNode::AddNeighbour (MosaicGraphEdge * edge)
{
    nlist.push_back(edge);
    MosaicGraphNode::Iterator it = nlist.end();
    return (--it);
}


void
MosaicGraphNode::EraseNeighbour (Iterator & it)
{
    nlist.erase(it);
}


std::ostream & operator << (std::ostream & out, const MosaicGraphNode * node)
{
    out.precision(3);
    out.setf ( std::ios::fixed, std::ios::floatfield );
    out << "[" << node->_x << ";" << node->_y << "] :";
    out.unsetf ( std::ios::floatfield  );
    //FOREACH(it, node->n) out << " " << (*it).first << "{" << (*it).second << "}";
    FOREACH(it, node->nlist)  out << " " << (*it);
    out << std::endl;
    return out;
}


MosaicGraphNode::MosaicGraphNode(const MosaicGraphNode & other)
{
    //copy ctor
    _x = other._x;
    _y = other._y;
    id = other.id;
/*
    //MosaicGraphNode::Iterator
    FOREACH(it, other.nlist)
    {
        MosaicGraphEdge * edge = new MosaicGraphEdge(**it);

    }
    nlist.assign(other.nlist.begin(), other.nlist.end());
 */
    n.assign(other.n.begin(), other.n.end());
}
