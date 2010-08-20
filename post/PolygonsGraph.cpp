#include "PolygonsGraph.h"

#include "configuration.hpp"
#include "pmf.hpp"
#include "segment.hpp"

#include "PolygonsSchedule.h"
#include "PolygonsSweepLine.h"
#include "FindUnionClass.h"


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

    double pixelWidth = pmf.GetWidth() / double(img.GetWidth());
    double pixelHeight = pmf.GetHeight() / double(img.GetHeight());

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
    double x_offset = .25 * pixelWidth;
    pmf::Point<double> * p1 = new pmf::Point<double>(-x_offset, pmf.GetHeight() + x_offset, NULL, NULL, 0., 0., long(n + 1), pmf::PT_TypesCount);
    pmf::Point<double> * p2 = new pmf::Point<double>(pmf.GetWidth() + x_offset, pmf.GetHeight() + x_offset, p1, NULL, 0., 0., long(n + 2), pmf::PT_TypesCount);
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
    FindUnionClass areasIds;
//**
    double column = .5 * pixelWidth;
    //long areaCount = 0l;
    long areaCount = areasIds.MakeNewSet();

    sweep.Insert(p1, s12, areaCount);
    std::map<long, std::set<long> > children;
    children[0] = std::set<long>();

    while (schedule.Size() > 0)
    {
        PolygonsSchedule::Event evt = schedule.SeeFirst();
        pmf::Point<double> * pt = evt->GetPoint();
        //cout << endl << endl << pt << endl;

        while (pt->x > column)
        {
            cout << endl;
            cout << "... scan at " << column << endl;

            double row = .5 * pixelHeight;
            PolygonsSweepLine::Iterator it2 = sweep.begin();
            cout << " ==> ";
            while (it2 != sweep.end())
            {
                cout << " " << (*it2)->GetUpperAreaNumber();
                cout << "(" << areasIds.Find( (*it2)->GetUpperAreaNumber() ) << ")";
                ++it2;
            }
            cout << endl;

            PolygonsSweepLine::Iterator it = sweep.begin();
            while (row < pmf.GetHeight() + pixelHeight)
            {
                while ( it != sweep.end()  and   (*it)->y0(column) < row )
                {
                    cout << "--- passed line : " << (*it)->GetSegment()->GetP()->id << " ~ " << (*it)->GetSegment()->GetQ()->id << " | " << (*it)->GetUpperAreaNumber() << " -> " << areasIds.Find( (*it)->GetUpperAreaNumber() ) << endl;
                    ++it;
                }

                row += pixelHeight;
            }
            column += pixelWidth;
        }

        PolygonsSweepLine::Iterator it, it1, it2;
        long areaId;
        switch (evt->GetType())
        {
            case PolygonsBeginSegment :
                                    if (Sn1[pt->id]->GetQ()->y > Sn2[pt->id]->GetQ()->y)
                                        std::swap( Sn1[pt->id], Sn2[pt->id] );
                                    assert(Sn1[pt->id]->GetP()->id == Sn2[pt->id]->GetP()->id);
                                    /// TODO : check it
                                    it1 = sweep.Insert(pt, Sn2[pt->id], areasIds.MakeNewSet() ).first;
                                    it2 = sweep.Insert(pt, Sn1[pt->id], areasIds.MakeNewSet() ).first;

                                    printf("  it2 = %li\n", (*it2)->GetUpperAreaNumber() );
                                    printf("  it1 = %li\n", (*it1)->GetUpperAreaNumber() );
                                    ++it1;
                                    printf("++it1 = %li\n", (*it1)->GetUpperAreaNumber() );

                                    areasIds.Union((*it2)->GetUpperAreaNumber(), (*it1)->GetUpperAreaNumber());
                                    --it1;
                                    children[ areasIds.Find((*it1)->GetUpperAreaNumber()) ] = std::set<long>();
                                    break;;
            case PolygonsBorderBegin :
                                    it1 = it2 = sweep.Insert(pt, Sn1[pt->id], areasIds.MakeNewSet() ).first;
                                    --it2;
                                    //*
                                    if (not sweep.IsNull(it2))
                                    {
                                        areaId = (*it2)->GetUpperAreaNumber();
                                        (*it1)->SetUpperAreaNumber(areaId);
                                        //--areaCount;
                                    }
                                    // */
                                    break;;
            case PolygonsEndOfSegment :
                                    if (Sn1[pt->id]->GetP()->y < Sn2[pt->id]->GetP()->y)
                                        std::swap( Sn1[pt->id], Sn2[pt->id] );
                                    assert(Sn1[pt->id]->GetQ()->id == Sn2[pt->id]->GetQ()->id);
                                    /// FIX IT
                                    it1 = sweep.Find(Sn1[pt->id]);
                                    it2 = sweep.Find(Sn2[pt->id]);

                                    printf("  it2 = %li\n", (*it2)->GetUpperAreaNumber() );
                                    printf("  it1 = %li\n", (*it1)->GetUpperAreaNumber() );
                                    ++it1;
                                    printf("++it1 = %li\n", (*it1)->GetUpperAreaNumber() );

                                    areasIds.Union((*it2)->GetUpperAreaNumber(), (*it1)->GetUpperAreaNumber());
                                    --it1;
                                    children[ areasIds.Find((*it2)->GetUpperAreaNumber()) ].insert( areasIds.Find((*it1)->GetUpperAreaNumber()) );
                                    sweep.Erase(it2);
                                    sweep.Erase(it1);
                                    break;;
            case PolygonsBorderEnd :
                                    /// FIX IT
                                    sweep.Erase(Sn1[pt->id]);
                                    break;;
            case PolygonsUpdateSegment :
                                    it = sweep.Find(Sn1[pt->id]);
                                    areaId = (*it)->GetUpperAreaNumber();
                                    sweep.Erase(it);
                                    //sweep.Erase(Sn1[pt->id]);
                                    sweep.Insert(pt, Sn2[pt->id], areaId);
                                    break;;
            case PolygonsMarker :
                                    sweep.Erase(evt->GetSegment(false));
                                    break;;
            default :
                    assert("STH is WRONG" and false);
        }
        schedule.Erase( evt );

        //FOREACH(sle, sweep)  cout << " __>  [" << (*sle)->GetUpperAreaNumber() << "]{" << areasIds.Find((*sle)->GetUpperAreaNumber()) << "}  " << **sle << endl;
        //scanf("%*c");
        cout << '*';
        //cout << pt << endl;
    }
    cout << endl;
    cout << " Made areas " << areasIds.size() << endl;
    cout << "\\__> ";
    for(unsigned i = 0; i < areasIds.size(); ++i)
        cout << " " << i << "[" << areasIds.Find(i) << "]";
    cout << endl;
    ///cout << schedule << endl;

    FOREACH(iiit, children)
    {
        cout << "[ " << iiit->first << " ]  : ";
        FOREACH(viiit, iiit->second)  cout << " " << *viiit;
        cout << endl;
    }

// */
}


PolygonsGraph::~PolygonsGraph()
{
    //dtor
    cout << "[ PG ] : deleted" << endl;
}

