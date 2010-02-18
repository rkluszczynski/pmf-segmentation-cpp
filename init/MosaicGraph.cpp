#include "MosaicGraph.hpp"

MosaicGraph::MosaicGraph()
{
    //ctor
}

MosaicGraph::~MosaicGraph()
{
    //dtor
}


/** @brief (one liner)
  *
  * (documentation goes here)
  */
unsigned int
MosaicGraph::CreateNewNode(double x, double y)
{
    unsigned int id = nodes.size();
    MosaicGraphNode * node = new MosaicGraphNode(x, y);
    nodes.push_back(node);
    return id;
}


/** @brief (one liner)
  *
  * (documentation goes here)
  */
void MosaicGraph::AddEdge(unsigned int n1, unsigned int n2)
{
    assert(0 <= n1  and  n1 < nodes.size());
    assert(0 <= n2  and  n2 < nodes.size());
    nodes[n1]->n.push_back(n2);
    if (n1 != n2)
        nodes[n2]->n.push_back(n1);
}


std::ostream & operator << (std::ostream & out, const MosaicGraph & graph)
{
    out << "[ MosaicGraph ]  :  nodes = " << graph.nodes.size() << std::endl;
    int i = 0;
    FOREACH(it, graph.nodes) out << i++ << " : " << *it;
    out << std::endl;
    return out;
}
