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
void
MosaicGraph::SortNeighboursInCounterClockwiseOrder ()
{
    FOREACH(it, nodes)
        (*it)->SortNeighboursInCounterClockwiseOrder (nodes);
}


/** @brief (one liner)
  *
  * (documentation goes here)
  */
unsigned int
MosaicGraph::CreateNewNode (double x, double y)
{
    unsigned int id = nodes.size();
    MosaicGraphNode * node = new MosaicGraphNode(x, y, id);
    nodes.push_back(node);
    return id;
}


/** @brief (one liner)
  *
  * (documentation goes here)
  */
void
MosaicGraph::AddEdge (unsigned int n1, unsigned int n2, int deg)
{
    assert(0 <= n1  and  n1 < nodes.size());
    assert(0 <= n2  and  n2 < nodes.size());
    nodes[n1]->n.push_back(std::make_pair(n2, deg));
    if (n1 != n2)
        nodes[n2]->n.push_back(std::make_pair(n1, deg));
}


void
MosaicGraph::RemoveEdge (unsigned int n1, unsigned int n2)
{
    unsigned int j = 0;
    while (j < nodes[n1]->n.size()  and  nodes[n1]->n[j].first != n2) ++j;
    for (unsigned int i = j; i < nodes[n1]->n.size()-1; ++i) nodes[n1]->n[i] = nodes[n1]->n[i+1];
    nodes[n1]->n.pop_back();

    j = 0;
    while (j < nodes[n2]->n.size()  and  nodes[n2]->n[j].first != n1) ++j;
    for (unsigned int i = j; i < nodes[n2]->n.size()-1; ++i) nodes[n2]->n[i] = nodes[n2]->n[i+1];
    nodes[n2]->n.pop_back();
}


std::ostream & operator << (std::ostream & out, const MosaicGraph & graph)
{
    out << "[ MosaicGraph ]  :  nodes = " << graph.nodes.size() << std::endl;
    int i = 0;
    FOREACH(it, graph.nodes) out << i++ << " : " << *it;
    out << std::endl;
    return out;
}


MosaicGraph::MosaicGraph(const MosaicGraph & other)
{
    //copy ctor
    FOREACH(it, other.nodes)
        nodes.push_back (new MosaicGraphNode(*(*it)));
}
