#include "MosaicDualGraph.hpp"

#ifdef PMF_CWD
    #define OUTPUT_DIR "./"
    #define  TIMESTAMP MosaicConstants::GetTimeStamp()
#else
    #define OUTPUT_DIR "output/"
    #define  TIMESTAMP ""
#endif

#define DEBUG_SWITCH 0
#define DEBUG_COLORS 0
#define DEBUG_COMPONENTS 0
#define DEBUG_SMALL 0

MosaicDualGraph::MosaicDualGraph(MosaicGraph * graph) : graph(graph)
{
    //ctor
    MosaicGraph * other = graph;
    other = new MosaicGraph(*graph);
    //std::cout << *other << std::endl;

    bool generateDualGraph = true;
    std::map<std::pair<int, int>, int> memory;
    /// TODO (Rafal#5#): to get rid of memory logn complexity (it's rather possible)

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
#if (DEBUG_SWITCH)
            std::cout << std::endl << "[[ GOING FROM _" << i << "_ ]]" << std::endl;
#endif
            area.push_back(i);
            do
            {
                a = b;
                MosaicGraphNode::Iterator it = nit;
                b = (*it)->GetId();
#if (DEBUG_SWITCH)
                std::cout << "[ WAY ] :  " << a << " -- " << b << std::endl;
#endif
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
#if (DEBUG_SWITCH)
                            std::cout << "[ LINK ] :   area " << anum << "  ~~  area " << areas.size() << std::endl;
#endif
                        }
                    }
                    other->RemoveEdge(a, it, b);
                }
            }
            while (b != i);

            areas.push_back(area);
#if (DEBUG_SWITCH)
            //std::cout << *other << std::endl;
#endif
        }
    }

#if (DEBUG_SWITCH)
    std::cout << *other << std::endl;
#endif
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
#if (DEBUG_COMPONENTS)
    int i = 0;
    FOREACH(it, components)
    {
        std::cout << "[ " << i++ << " ] :";
        FOREACH(jt, *it) std::cout << " " << *jt;
        std::cout << std::endl;
    }
    i = 0;
#endif
    MosaicGraph * pmfgraph = new MosaicGraph(*graph);
    std::set<std::pair<int, int> > edges2remove;

    FOREACH(it, components)
    {
        std::set<std::pair<int, int> > edges;
#if (DEBUG_COMPONENTS)
        std::cout << "[[ COMPONENT " << i++ << " ]] :" << std::endl;
#endif
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
#if (DEBUG_COMPONENTS)
                        std::cout << " ->  INSERT  (" << a << "," << b << ")" << std::endl;
#endif
                        edges.insert(std::make_pair(a, b));
                    }
                    else {
#if (DEBUG_COMPONENTS)
                        std::cout << " ->  REMOVE  (" << a << "," << b << ")" << std::endl;
#endif
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
#if (DEBUG_COMPONENTS)
        std::cout << "  (" << it->first << "," << it->second << ")";
#endif
        toremove[it->first].push_back(it->second);
    }

    for (unsigned int i = 0; i < pmfgraph->Size(); ++i)
        pmfgraph->RemoveNeighborsOf(i, toremove[i]);

#if (DEBUG_COMPONENTS)
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "[[ GOING TO REMOVE ALL UNNECESSARY NODES ]]" << std::endl;
    std::cout << *pmfgraph << std::endl;
#endif
    pmfgraph->RemoveUnnecessaryCollinearNodes(DEBUG_COMPONENTS);

#if (DEBUG_COMPONENTS)
    std::cout << "[[ REMOVED ALL UNNECESSARY NODES ]]" << std::endl;
    std::cout << *pmfgraph << std::endl;
#endif
    std::string filepath;

    filepath = std::string(OUTPUT_DIR) + std::string(TIMESTAMP) + std::string("_2_merged-areas.ggb");
    pmfgraph->SaveAsGeoGebraFile(filepath.c_str());

    filepath = std::string(OUTPUT_DIR) + std::string(TIMESTAMP) + std::string("_3_disordered-pmf.ggb");
    pmfgraph->SaveAsGeoGebraFile(filepath.c_str());

    pmfgraph->MutateIntersectionElements();
    pmfgraph->RemoveUnnecessaryCollinearNodes();

    filepath = std::string(OUTPUT_DIR) + std::string(TIMESTAMP) + std::string("_4_ready-pmf.ggb");
    pmfgraph->SaveAsGeoGebraFile(filepath.c_str());

    RemoveToSmallAreas(pmfgraph, atof(MosaicConstants::GetValueOfKey("area-cutoff")));
    pmfgraph->RemoveUnnecessaryCollinearNodes();
#if (DEBUG_SMALL)
    std::cout << *pmfgraph << std::endl;
#endif
    filepath = std::string(OUTPUT_DIR) + std::string(TIMESTAMP) + std::string("_result-pmf.ggb");
    pmfgraph->SaveAsGeoGebraFile(filepath.c_str());
    filepath = std::string(OUTPUT_DIR) + std::string(TIMESTAMP) + std::string("_result-pmf.txt");
    pmfgraph->SaveAsTextFile(filepath.c_str());

    //*
    pmfgraph->MakeGaussianShakeToDisorder( atof(MosaicConstants::GetValueOfKey("gauss-variance")) );

    filepath = std::string(OUTPUT_DIR) + std::string(TIMESTAMP) + std::string("_shaked-pmf.ggb");
    pmfgraph->SaveAsGeoGebraFile(filepath.c_str());
    filepath = std::string(OUTPUT_DIR) + std::string(TIMESTAMP) + std::string("_shaked-pmf.txt");
    pmfgraph->SaveAsTextFile(filepath.c_str());
    // */
}



void
MosaicDualGraph::RemoveToSmallAreas(MosaicGraph * graph, double cutoff)
{
    MosaicGraph * copygraph = new MosaicGraph(*graph);
    copygraph->SortNeighborsInCounterClockwiseOrder();

    std::vector<std::vector<int> > nareas;
    for (unsigned int i = 0; i < copygraph->Size(); ++i)
    {
        MosaicGraphNode * node = copygraph->Get(i);

        while (node->Size() > 0)
        {
            std::vector<int> area;

            MosaicGraphNode::Iterator nit = node->Begin();
            unsigned int a = -1;
            unsigned int b = i;

            area.push_back(i);
            do
            {
                a = b;
                MosaicGraphNode::Iterator it = nit;
                b = (*it)->GetId();

                area.push_back(b);

                MosaicGraphNode * nodeb = copygraph->Get(b);
                nit = (*it)->GetOther();
                nit = (nit == nodeb->Begin()) ? (--nodeb->End()) : (--nit);
                //*
                if ( (*it)->GetDegree() > 1 )
                {
                    (*it)->SetDegree(1);
                    (* (*it)->GetOther() )->SetDegree(1);
                }
                else
                // */
                    copygraph->RemoveEdge(a, it, b);
            }
            while (b != i);

            nareas.push_back(area);
            //std::cout << *copygraph << std::endl;
        }
    }

    std::set<std::pair<int, int> > edgestoremove;
    FOREACH(it, nareas)
    {
#if (DEBUG_SMALL)
        std::cout << std::endl;
        std::cout << "=>";
        FOREACH(iit, *it)  std::cout << " " << *iit;
        std::cout << std::endl;
#endif
        std::vector<int>::iterator iit = it->begin();
        double x0 = copygraph->Get(*iit)->x();
        double y0 = copygraph->Get(*iit)->y();

        std::vector<int>::iterator it1 = (++iit);
        std::vector<int>::iterator it2 = (++iit);
        double value = 0.;
        while (*it->begin() != *it2)
        {
            double x1 = copygraph->Get(*it1)->x();
            double y1 = copygraph->Get(*it1)->y();
            double x2 = copygraph->Get(*it2)->x();
            double y2 = copygraph->Get(*it2)->y();

            double det = x0*y1 + x1*y2 + x2*y0 - y0*x1 - y1*x2 - y2*x0;
            //std::cout << " ... " << det << std::endl;
            value += det;

            it1 = it2;
            it2 = (++iit);
        }
#if (DEBUG_SMALL)
        std::cout << "\\___ = " << .5 * value << std::endl;
#endif
        if (.5 * value < cutoff)
        {
#if (DEBUG_SMALL)
            std::cout << " TO REMOVE " << std::endl;
#endif
            FOREACH(iit, *it)
            {
                std::vector<int>::iterator niit = iit + 1;
                if (niit != it->end())
                {
                    unsigned int a = *iit;
                    unsigned int b = *niit;
                    //if (a > b)  std::swap(a,b);
                    edgestoremove.insert(std::make_pair(a, b));
                    edgestoremove.insert(std::make_pair(b, a));
                }
            }
        }
    }
#if (DEBUG_SMALL)
    std::cout << "[ SMALL AREAS EDGES ] :";
    FOREACH(it, edgestoremove) if (it->first < it->second) std::cout << " (" << it->first << "," << it->second << ")";
    std::cout << std::endl;
#endif
    for (unsigned int i = 0; i < graph->Size(); ++i)
    {
        MosaicGraphNode * node = graph->Get(i);
        //FOREACH(nit, *node)
        //for (MosaicGraphNode::Iterator nodeit = node->Begin(); nodeit != node->End(); ++nodeit)
        MosaicGraphNode::Iterator nodeit = node->Begin();
        while (nodeit != node->End())
        {
            unsigned int a = i;
            unsigned int b = (*nodeit)->GetId();
            MosaicGraphNode::Iterator tmp = nodeit;
            ++nodeit;
            //if (a > b)  std::swap(a,b);
            if (edgestoremove.find(std::make_pair(a,b)) != edgestoremove.end()
                and
                (*tmp)->GetDegree() > 0
                and
                a < b
                )
            {
#if (DEBUG_SMALL)
                std::cout << " removing " << a << " - " << b << std::endl;
#endif
                graph->RemoveEdge(a, tmp, b);
            }
        }
    }
}


std::pair<unsigned int, unsigned int>
MosaicDualGraph::CountBlackAndWhitePixels(
                                            double ux, double uy, double unx, double uny,
                                            double dx, double dy, double dnx, double dny,
                                            pmf::GrayscaleImage & gimg,
                                            double xb, double xe
                                        )
{
    double  width = MosaicConstants::GetPmfWidth();
    double height = MosaicConstants::GetPmfHeight();
#if (DEBUG_COLORS)
    std::cout << "[ SEGMENT  UP  ]  :  (" << ux << ";" << uy << ") - (" << unx << ";" << uny << ")" << std::endl;
    std::cout << "[ SEGMENT DOWN ]  :  (" << dx << ";" << dy << ") - (" << dnx << ";" << dny << ")" << std::endl;
#endif
    unsigned int black = 0;
    unsigned int white = 0;

    double pixelx =  width / double(gimg.GetWidth());
    double pixely = height / double(gimg.GetHeight());

    int picix = int(xb / pixelx);
#if (DEBUG_COLORS)
    std::cout << " start at pixel " << picix << std::endl;
#endif
    bool printDebug = true and DEBUG_COLORS;

    double osx = 0.5 * pixelx + picix * pixelx;
    while (osx <= xe)
    {
        double au, bu;
        if (ux != unx)
        {
            au = (uny - uy) / (unx - ux);
            bu = uy - au * ux;
        }
        else
        {
            au = 0.;
            bu = 0.5 * (uy + uny);
            bu = std::max(bu, 0.);
            bu = std::min(bu, height);
        }

        double ad, bd;
        if (dx != dnx)
        {
            ad = (dny - dy) / (dnx - dx);
            bd = dy - ad * dx;
        }
        else
        {
            ad = 0.;
            bd = 0.5 * (dy + dny);
            bd = std::max(bd, 0.);
            bd = std::min(bd, height);
        }

        double fromy = ad * osx + bd;
        double gotoy = au * osx + bu;

        if (printDebug)
            std::cout << " " << osx << "=(" << fromy << "::" << gotoy << ")";

        int picx = int(std::max(osx, 0.) / pixelx);
        int picyfrom = gimg.GetHeight() - 1 - int(std::max(gotoy, 0.) / pixely);
        int picygoto = gimg.GetHeight() - 1 - int(std::max(fromy, 0.) / pixely);

        for (int i = picyfrom; i <= picygoto; ++i)
        {
            int value = gimg[i][picx][0];

            if (value < 128)
            {
                black++;
            }
            else {
                white++;
            }
            if (false and printDebug)  printf("%c", (value < 128) ? 'X' : '.');
        }
        if (printDebug)
            std::cout << "#" << picx << "=" << picyfrom << ":" << picygoto;

        osx += pixelx;
    }
    if (printDebug)  std::cout << std::endl;
#if (DEBUG_COLORS)
    std::cout << "( B=" << black << " , W=" << white << " )  of  " << (white+black) << std::endl;
#endif
    return std::make_pair(black, white);
}


int
MosaicDualGraph::DetermineAreaColor (std::vector<int> & area, pmf::GrayscaleImage & gimg)
{
    unsigned int db = 0, de = 1;
    //std::cout << " db = " << db << std::endl;
    while (graph->Get(area[de])->x() < graph->Get(area[de+1])->x()) ++de;
    unsigned int ub = de;
    //std::cout << " de = " << de << std::endl;
    while (graph->Get(area[ub])->x() == graph->Get(area[ub+1])->x()) ++ub;
    //std::cout << " ub = " << ub << std::endl;
    unsigned int ue = ub + 1;
    while (ue < area.size()-1  and  graph->Get(area[ue])->x() > graph->Get(area[ue+1])->x()) ++ue;
    //std::cout << " ue = " << ue << std::endl;
#if (DEBUG_COLORS)
    std::cout << "   (db, de) = (" << db << "," << de << ")";
    std::cout << "   (ub, ue) = (" << ub << "," << ue << ")";
    std::cout << std::endl;
#endif
    std::vector<unsigned int> down, up;
    unsigned int i = db;
    while (i <= de) { down.push_back(area[i]); ++i; }
    i = ue;
    while (i >= ub) { up.push_back(area[i]); --i; }

#if (DEBUG_COLORS)
    FOREACH(it, up) std::cout << " " << *it;  std::cout << std::endl;
    FOREACH(it, down) std::cout << " " << *it;  std::cout << std::endl;
#endif
    assert(graph->Get(up.front())->x() == graph->Get(down.front())->x());
    assert(graph->Get(up.back())->x() == graph->Get(down.back())->x());

    unsigned int ui = 0, di = 0;
    double ux = graph->Get(up[ui])->x();
    double uy = graph->Get(up[ui])->y();
    double dx = graph->Get(down[di])->x();
    double dy = graph->Get(down[di])->y();
    ++ui;  ++di;

    std::pair<unsigned int, unsigned int> pixels(0, 0);
    while (ui < up.size()-1  or  di < down.size()-1)
    {
        double unx = graph->Get(up[ui])->x();
        double uny = graph->Get(up[ui])->y();
        double dnx = graph->Get(down[di])->x();
        double dny = graph->Get(down[di])->y();

        double xb = std::max(ux, dx);
        double xe = std::min(unx, dnx);
#if (DEBUG_COLORS)
        std::cout << xb << "  " << xe << "      =>  " << ui << "/" << up.size() << " " << di << "/" << down.size() << std::endl;
#endif
        if (dnx < unx)
        {
            ++di;
            std::pair<unsigned int, unsigned int> val = CountBlackAndWhitePixels( ux, uy, unx, uny,  dx, dy, dnx, dny,  gimg, xb, xe );
            pixels.first  += val.first;
            pixels.second += val.second;
            dx = dnx;
            dy = dny;
        }
        else if (dnx > unx)
        {
            ++ui;
            std::pair<unsigned int, unsigned int> val = CountBlackAndWhitePixels( ux, uy, unx, uny,  dx, dy, dnx, dny,  gimg, xb, xe );
            pixels.first  += val.first;
            pixels.second += val.second;
            ux = unx;
            uy = uny;
        }
        else if (dnx == unx)
        {
            if (di == down.size()-1)
            {
                ++ui;
                ux = unx;
                uy = uny;
            }
            else if (ui == up.size()-1)
            {
                ++di;
                dx = dnx;
                dy = dny;
            }
            else
                assert("HAVE TO ANALIZE IT A BIT" && false);
        }
    }
    double unx = graph->Get(up[ui])->x();
    double uny = graph->Get(up[ui])->y();
    double dnx = graph->Get(down[di])->x();
    double dny = graph->Get(down[di])->y();

    double xb = std::max(ux, dx);
    double xe = std::min(unx, dnx);
#if (DEBUG_COLORS)
    std::cout << xb << "  " << xe << "      =>  " << ui << "/" << up.size() << " " << di << "/" << down.size() << std::endl;
#endif
    std::pair<unsigned int, unsigned int> val = CountBlackAndWhitePixels( ux, uy, unx, uny,  dx, dy, dnx, dny,  gimg, xb, xe );
    pixels.first  += val.first;
    pixels.second += val.second;

    int result = 0;
    if (pixels.first < pixels.second) result = 1;
    else if (pixels.first == pixels.second) result = rand() % 2;

#if (DEBUG_COLORS)
    std::cout << "[[ COLOR _" << result << "_ ]]  :  BLACK = " << pixels.first << " ,   WHITE = " << pixels.second << " ,   of  " << (pixels.first + pixels.second) << std::endl;
#endif
    return result;
}


void
MosaicDualGraph::DetermineAreasColors (pmf::GrayscaleImage & gimg)
{
    areasColors.resize(areas.size());

    //srand(2);
    FOREACH(it, areasColors)  *it = rand() % 2;

    int i = 0;
    FOREACH(it, areas)
    {
#if (DEBUG_COLORS)
        std::cout << std::endl << "[[ AREA " << i << " ]]  :  ";
        FOREACH(iit, *it)  std::cout << " " << *iit;
        std::cout << std::endl;
#endif
        areasColors[i] = DetermineAreaColor(*it, gimg);
        ++i;
    }
#if (DEBUG_COLORS)
    std::cout << "[ COLORS ] :";
    for (unsigned int j = 0; j < areasColors.size(); ++j) std::cout << " " << j << "{" << areasColors[j] << "}";
    std::cout << std::endl;
#endif
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

