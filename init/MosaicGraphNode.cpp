#include "MosaicGraphNode.hpp"

MosaicGraphNode::MosaicGraphNode(double xx, double yy) : x(xx), y(yy)
{
    //ctor
}

MosaicGraphNode::~MosaicGraphNode()
{
    //dtor
}


std::ostream & operator << (std::ostream & out, const MosaicGraphNode * node)
{
    out.precision(2);
    out <<  "[" << node->x << ";" << node->y << "] :";
    FOREACH(it, node->n) out << " " << *it;
    out << std::endl;
    return out;
}

