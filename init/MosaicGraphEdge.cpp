#include "MosaicGraphEdge.hpp"


MosaicGraphEdge::MosaicGraphEdge(unsigned int _id, int deg) : id(_id), degree(deg)
{
    //ctor
}


MosaicGraphEdge::~MosaicGraphEdge()
{
    //dtor
}


MosaicGraphEdge::MosaicGraphEdge(const MosaicGraphEdge & other)
{
    //copy ctor
    id = other.id;
    degree = other.degree;
}


std::ostream & operator << (std::ostream & out, const MosaicGraphEdge * edge)
{
    out << edge->id << "{" << edge->degree << "}";
    out << "/" << (*edge->other)->GetId() << "/";
    assert( (*(*edge->other)->GetOther())->GetId() == edge->id );
    return out;
}
