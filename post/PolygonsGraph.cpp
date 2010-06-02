#include "PolygonsGraph.h"

#include "configuration.hpp"
#include "pmf.hpp"
#include "segment.hpp"

#include "PolygonsSchedule.h"
#include "PolygonsSweepLine.h"


PolygonsGraph::PolygonsGraph(const char * filename)
{
    //ctor
    pmf::PMF<double> pmf(0., 0.);
    pmf.LoadPMF(filename);
    pmf.RotatePoints2(0., 1., false);

    pmf::GrayscaleImage img("../input/30x30.png");

    pmf.GetCf()->ShowConfiguration(cout, 5);
    const unsigned n = pmf.GetCount() + 1u;

    PolygonsSchedule schedule;
    pmf::Point<double> * Points[n];
    pmf::Segment<double> * Sn1[n];
    pmf::Segment<double> * Sn2[n];
    for (unsigned i = 1u; i < n; ++i) Points[i] = NULL;
    for (unsigned i = 1u; i < n; ++i) Sn1[i] = Sn2[i] = NULL;

    FOREACH(it, *pmf.GetCf())
    {
        pmf::Point<double> * pt = &(**it);
        pmf::Point<double> * n1 = pt->n1;
        pmf::Point<double> * n2 = pt->n2;

        Points[pt->id] = pt;
        if (n1  and  pt->x < n1->x)
        {
            pmf::Segment<double> * s = new pmf::Segment<double>(pt, n1);
            Sn1[pt->id] = s;
            int id = n1->WhichNeighbourHasID(pt->id);
            assert(id > 0);
            if (id == 1) Sn1[n1->id] = s; else Sn2[n1->id] = s;
        }
        if (n2  and  pt->x < n2->x)
        {
            pmf::Segment<double> * s = new pmf::Segment<double>(pt, n2);
            Sn2[pt->id] = s;
            int id = n2->WhichNeighbourHasID(pt->id);
            assert(id > 0);
            if (id == 1) Sn1[n2->id] = s; else Sn2[n2->id] = s;
        }
    }
    /*
    for (unsigned i = 1u; i < n; ++i)
    {
        printf("%3li :  (%p)  x  (%p)\n", Points[i]->id, Sn1[i], Sn2[i]);
        //cout << Points[i]->id << " :   " << Sn1[i] << "  x  " << Sn2[i] << endl;
    }
    //*/
    FOREACH(it, *pmf.GetCf())
    {
        pmf::Point<double> * pt = &(**it);
        schedule.Insert(pt, Sn1[pt->id], Sn2[pt->id]);
    }


    PolygonsSweepLine sweep;

//**
    double pixelWidth = pmf.GetWidth() / double(img.GetWidth());
    double pixelHeight = pmf.GetHeight() / double(img.GetHeight());
    double column = .5 * pixelWidth;

    while (schedule.Size() > 0)
    {
        PolygonsSchedule::Event evt = schedule.SeeFirst();
        pmf::Point<double> * pt = evt->GetPoint();
        schedule.Erase( evt );


        while (pt->x > column)
        {
            cout << endl;
            cout << "... scan at " << column << endl;

            double row = .5 * pixelWidth;
            while (row < pmf.GetHeight())
            {



                row += pixelHeight;
            }
            column += pixelWidth;
        }
        cout << '*';
        //cout << pt << endl;
    }
    cout << endl;
    ///cout << schedule << endl;
// */
}


PolygonsGraph::~PolygonsGraph()
{
    //dtor
    cout << "[ PG ] : deleted" << endl;
}

