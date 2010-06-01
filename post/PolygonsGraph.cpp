#include "PolygonsGraph.h"

#include "../post/PolygonsSchedule.h"
#include "configuration.hpp"
#include "pmf.hpp"
#include "segment.hpp"


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
    pmf::Segment<double> * n1[pmf.GetCf()->GetPointsCount()+1];
    pmf::Segment<double> * n2[pmf.GetCf()->GetPointsCount()+1];

    FOREACH(it, *pmf.GetCf())
    {
        pmf::Point<double> * pt = &(**it);
    }

    FOREACH(it, *pmf.GetCf())
    {
        pmf::Point<double> * pt = &(**it);
        PolygonsSchedule::SEGMENT s1 = NULL, s2 = NULL;

        switch (pt->type)
        {
            case pmf::PT_BirthInField:
            case pmf::PT_BirthOnBorder:
                                    break;;
            case pmf::PT_DeathOnBorder:
            case pmf::PT_Update:
                                    break;;
            case pmf::PT_Collision:
                                    break;;
            default :
                        assert("ZLE ZLE ZLE" and false);
        }
        //cout << pt << endl;
        schedule.Insert(pt, s1, s2);
    }

    FOREACH(it, *pmf.GetCf())
    {
        pmf::Point<double> * pt = &(**it);
        //cout << pt << endl;
        //schedule.Insert(pt);
    }
    //cout << schedule << endl;
/**
    while (schedule.Size() > 0)
    {
        PolygonsSchedule::Event evt = schedule.SeeFirst();
        pmf::Point<double> * pt = evt->GetPoint();
        schedule.Erase( evt );



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

*/
}


PolygonsGraph::~PolygonsGraph()
{
    //dtor
    cout << "[ PG ] : deleted" << endl;
}

