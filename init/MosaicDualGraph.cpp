#include "MosaicDualGraph.hpp"

MosaicDualGraph::MosaicDualGraph(MosaicGraph * graph) : graph(graph)
{
    //ctor
    MosaicGraph * other = graph;
    other = new MosaicGraph(*graph);
    std::cout << *other << std::endl;

    bool generateDualGraph = true;
    std::map<std::pair<int, int>, int> memory;
    /// TODO (Rafal#5#): to get rid of memory logn complexity

    for (unsigned int i = 0; i < other->Size(); ++i)
    {
        MosaicGraphNode * node = other->Get(i);

        while (node->Size() > 0)
        {
            std::vector<int> area;
            if (generateDualGraph) areaGraph.push_back(area);

            MosaicGraphNode::Iterator nit = node->Begin();
            unsigned int a = -1;
            unsigned int b = i;

            area.push_back(i);
            do
            {
                a = b;
                MosaicGraphNode::Iterator it = nit;
                b = (*it)->GetId();

                std::cout << "[ WAY ] :  " << a << " -- " << b << std::endl;
                area.push_back(b);

                MosaicGraphNode * nodeb = other->Get(b);
                nit = (*it)->GetOther();
                nit = (nit == nodeb->Begin()) ? (--nodeb->End()) : (--nit);

                if ( (*it)->GetDegree() > 1 )
                {
                    (*it)->SetDegree(1);
                    (* (*it)->GetOther() )->SetDegree(1);

                    if (generateDualGraph)
                        memory[std::make_pair(a,b)] = memory[std::make_pair(b,a)] = areas.size();
                }
                else
                {
                    if (generateDualGraph)
                    {
                        std::map<std::pair<int, int>, int>::iterator iit = memory.find(std::make_pair(a,b));
                        if (iit != memory.end())
                        {
                            int anum = iit->second;
                            areaGraph[anum].push_back( areas.size() );
                            areaGraph[ areas.size() ].push_back(anum);
                            std::cout << "[ LINK ] :   area " << anum << "  ~~  area " << areas.size() << std::endl;
                        }
                    }
                    other->RemoveEdge(a, it, b);
                }
                //break;
            }
            while (b != i);

            areas.push_back(area);
            //std::cout << *other << std::endl;
        }
    }


    std::cout << *other << std::endl;
}


MosaicDualGraph::~MosaicDualGraph()
{
    //dtor
}


void
MosaicDualGraph::CalculateComponents ()
{
    std::vector<std::vector<int> > components;

    std::vector<bool> visited(areaGraph.size());
    fill_n(visited.begin(), visited.size(), false);

    for (unsigned int i = 0; i < areaGraph.size(); ++i)
    {
        if (! visited[i])
        {
            components.push_back(std::vector<int>());

            std::stack<int> st;
            st.push(i);
            visited[i] = true;

            while (! st.empty())
            {
                int v = st.top();
                st.pop();
                components.back().push_back(v);

                FOREACH(it, areaGraph[v])
                    if (! visited[*it]  and  areasColors[*it] == areasColors[v])
                    {
                        st.push(*it);
                        visited[*it] = true;
                    }
            }
        }
    }

    int i = 0;
    FOREACH(it, components)
    {
        std::cout << "[ " << i++ << " ] :";
        FOREACH(jt, *it) std::cout << " " << *jt;
        std::cout << std::endl;
    }


    MosaicGraph * pmfgraph = new MosaicGraph(*graph);
    std::set<std::pair<int, int> > edges2remove;

    FOREACH(it, components)
    {
        std::set<std::pair<int, int> > edges;

        FOREACH(jt, *it)
        {
            int mem_b = -1;

            FOREACH(kt, areas[*jt])
            {
                int a = mem_b;
                int b = *kt;
                mem_b = b;
                if (a > b)  std::swap(a, b);

                if (a >= 0)
                {
                    if (edges.find(std::make_pair(a, b)) == edges.end())
                    {
                        std::cout << " ->  INSERT  (" << a << "," << b << ")" << std::endl;
                        edges.insert(std::make_pair(a, b));
                    }
                    else {
                        std::cout << " ->  REMOVE  (" << a << "," << b << ")" << std::endl;
                        edges2remove.insert(std::make_pair(a, b));
                    }
                }
            }
        }
        edges.clear();
    }


    std::vector<std::vector<int> > toremove(graph->Size(), std::vector<int>());
    FOREACH(it, edges2remove)
    {
        std::cout << "  (" << it->first << "," << it->second << ")";
        toremove[it->first].push_back(it->second);
    }

    for (unsigned int i = 0; i < pmfgraph->Size(); ++i)
        pmfgraph->RemoveNeighborsOf(i, toremove[i]);

    std::cout << std::endl;
    pmfgraph->RemoveUnnecessaryCollinearNodes();

    std::cout << *pmfgraph << std::endl;
    pmfgraph->SaveAsGeoGebraFile("output/pmfgraph.ggb");

    pmfgraph->MutateIntersectionElements();
    //pmfgraph->RemoveUnnecessaryCollinearNodes();
    pmfgraph->SaveAsGeoGebraFile("output/finalpmf.ggb");
}


int
MosaicDualGraph::DetermineAreaColor (std::vector<int> & area)
{
    int db = 0, de = 1;
    std::cout << " db = " << db << std::endl;
    while (graph->Get(area[de])->x() < graph->Get(area[de+1])->x()) ++de;
    int ub = de;
    std::cout << " de = " << de << std::endl;
    while (graph->Get(area[ub])->x() == graph->Get(area[ub+1])->x()) ++ub;
    std::cout << " ub = " << ub << std::endl;
    int ue = ub + 1;
    while (ue < area.size()-1  and  graph->Get(area[ue])->x() > graph->Get(area[ue+1])->x()) ++ue;
    std::cout << " ue = " << ue << std::endl;

    std::vector<int> down, up;
    int i = 0;
    while (i <= de) { down.push_back(area[i]); ++i; }
    i = ue;
    while (i >= ub) { up.push_back(area[i]); --i; }

    FOREACH(it, up) std::cout << " " << *it;  std::cout << std::endl;
    FOREACH(it, down) std::cout << " " << *it;  std::cout << std::endl;

}


void
MosaicDualGraph::DetermineAreasColors ()
{
    areasColors.resize(areas.size());
    int i = 0;
    FOREACH(it, areas)
    {
        std::cout << "[[ AREA " << i++ << " ]]" << std::endl;
        DetermineAreaColor(*it);
    }

    //srand(2);
    FOREACH(it, areasColors)  *it = rand() % 2;

    std::cout << "[ COLORS ] :";
    for (unsigned int j = 0; j < areasColors.size(); ++j) std::cout << " " << j << "{" << areasColors[j] << "}";
    std::cout << std::endl;
}


std::ostream & operator << (std::ostream & out, const MosaicDualGraph & dual)
{
    out << "[ MosaicDualGraph ]  :  areas = " << dual.areas.size() << std::endl;
    int i = 0;
    FOREACH(it, dual.areas)
    {
        out << "[ AREA " << i++ << " ] :";
        FOREACH(iit, *it) out << " " << *iit;
        out << std::endl;
    }
    out << " ______________________________________________ " << std::endl;
    i = 0;
    FOREACH(it, dual.areaGraph)
    {
        out << "[ " << i++ << " ] :";
        FOREACH(iit, *it) out << " " << *iit;
        out << std::endl;
    }
    out << " ++++++++++++++++++++++++++++++++++++++++++++++ " << std::endl;
    return out;
}

