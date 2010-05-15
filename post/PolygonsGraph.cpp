#include "PolygonsGraph.h"

#include "configuration.hpp"


PolygonsGraph::PolygonsGraph(const char * filename)
{
    //ctor
    cf = new pmf::Configuration<double>(0., 0.);
    ifstream fin(filename);
    cf->LoadConfiguration(fin);
    fin.close();

    cf->ShowConfiguration(cout, 5);

    FOREACH(it, *cf)
    {
        pmf::Point<double> * pt = &(**it);
        cout << pt << endl;
    }
}


PolygonsGraph::~PolygonsGraph()
{
    //dtor
    cout << "[ PG ] : deleted" << endl;
}

