#include "PolygonsGraph.h"

#include "../post/PolygonsSchedule.h"
#include "configuration.hpp"
#include "pmf.hpp"


PolygonsGraph::PolygonsGraph(const char * filename)
{
    //ctor
    pmf::PMF<double> pmf(0., 0.);
    pmf.LoadPMF(filename);
    pmf.RotatePoints2(0., 1.);

    pmf::GrayscaleImage img("../input/30x30.png");
    double pixelWidth = pmf.GetWidth() / double(img.GetWidth());
    double pixelHeight = pmf.GetHeight() / double(img.GetHeight());
    double column = .5 * pixelWidth;


    pmf.GetCf()->ShowConfiguration(cout, 5);

    PolygonsSchedule schedule;
    FOREACH(it, *pmf.GetCf())
    {
        pmf::Point<double> * pt = &(**it);
        //cout << pt << endl;
        schedule.Insert(pt);
    }
    //cout << schedule << endl;

    while (schedule.Size() > 0)
    {
        pmf::Point<double> * pt = schedule.SeeFirst();
        schedule.Erase( pt );



        if (pt->x > column)
        {
            cout << "... scan at " << column << endl;

            double row = .5 * pixelWidth;
            while (row < pmf.GetHeight())
            {



                row += pixelHeight;
            }
            column += pixelWidth;
        }
        cout << pt << endl;
    }
    cout << schedule << endl;
}


PolygonsGraph::~PolygonsGraph()
{
    //dtor
    cout << "[ PG ] : deleted" << endl;
}

