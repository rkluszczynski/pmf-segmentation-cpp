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
    pmf.SavePMF("../output/__singiel_output-test-file.ggb", pmf::GeoGebraFile);
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
    pmf::Point<double> * p1 = new pmf::Point<double>(-0.1, pmf.GetHeight() + .1, NULL, NULL, 0., 0., long(n + 1), pmf::PT_TypesCount);
    pmf::Point<double> * p2 = new pmf::Point<double>(pmf.GetWidth() + .1, pmf.GetHeight() + .1, p1, NULL, 0., 0., long(n + 2), pmf::PT_TypesCount);
    p1->n1 = p2;
    pmf::Segment<double> * s12 = new pmf::Segment<double>(p1, p2);
    PolygonsMarkerEvent * pme = new PolygonsMarkerEvent(p2, s12);

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
    schedule.Insert(pme);


    PolygonsSweepLine sweep;
    sweep.Insert(p1, s12);
//**
    double pixelWidth = pmf.GetWidth() / double(img.GetWidth());
    double pixelHeight = pmf.GetHeight() / double(img.GetHeight());
    double column = .5 * pixelWidth;

    while (schedule.Size() > 0)
    {
        PolygonsSchedule::Event evt = schedule.SeeFirst();
        pmf::Point<double> * pt = evt->GetPoint();
        //cout << pt << endl;

        while (pt->x > column)
        {
            cout << endl;
            cout << "... scan at " << column << endl;

            double row = .5 * pixelHeight;
            PolygonsSweepLine::Iterator it = sweep.begin();
            while (row < pmf.GetHeight())
            {
                while ( it != sweep.end()  and   (*it)->y0(column) < row )
                {
                    cout << "--- passed line : " << (*it)->GetSegment()->GetP()->id << " ~ " << (*it)->GetSegment()->GetQ()->id << endl;
                    ++it;
                }

                row += pixelHeight;
            }
            column += pixelWidth;
        }

        switch (evt->GetType())
        {
            case PolygonsBeginSegment :
                                    sweep.Insert(pt, Sn2[pt->id]);
            case PolygonsBorderBegin :
                                    sweep.Insert(pt, Sn1[pt->id]);
                                    break;;
            case PolygonsEndOfSegment :
                                    sweep.Erase(Sn2[pt->id]);
            case PolygonsBorderEnd :
                                    sweep.Erase(Sn1[pt->id]);
                                    break;;
            case PolygonsUpdateSegment :
                                    sweep.Erase(Sn1[pt->id]);
                                    sweep.Insert(pt, Sn2[pt->id]);
                                    break;;
            case PolygonsMarker :
                                    sweep.Erase(evt->GetSegment(false));
                                    break;;
            default :
                    assert("STH is WRONG" and false);
        }
        schedule.Erase( evt );

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

