#include "MosaicPMF.hpp"

using namespace std;
#define EPSILON 1e-7
#include "../cmd/geometry.hpp"

#define ST first
#define ND second

#define REP(X, N)       for(int X = 0; X < (N); ++X)
#define FOR(X, B, E)    for(int X = B; X <= (E); ++X)
#define FORD(X, B, E)   for(int X = B; X >= (E); --X)

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)





MosaicPMF::MosaicPMF (
                        double w,
                        double h,
                        MosaicLinesDeterminer & mosaic,
                        pmf::GrayscaleImage & gimg
                    )
: fieldWidth(w), fieldHeight(h)
{
    //ctor
    /** Preparing events list **/
    MosaicPoint<double> * ptLU = new MosaicPoint<double>(0., fieldHeight);
    MosaicPoint<double> * ptLD = new MosaicPoint<double>(0., 0.);
    MosaicPoint<double> * ptRU = new MosaicPoint<double>(fieldWidth, fieldHeight);
    MosaicPoint<double> * ptRD = new MosaicPoint<double>(fieldWidth, 0.);
    AreaEvent * e0;

    MosaicEventsSchedule<double> * evts = new MosaicEventsSchedule<double>();
    e0 = new AreaEvent(ptLU);      evts->Insert(e0);
    e0 = new AreaEvent(ptLD);      evts->Insert(e0);
    e0 = new AreaEvent(ptRU);      evts->Insert(e0);
    e0 = new AreaEvent(ptRD);      evts->Insert(e0);
    FOREACH(it, mosaic)
    {
        MosaicSegment<double> * seg = * it;

        BeginEvent * e1 = new BeginEvent(seg->GetLeftPoint(), seg);
        EndEvent * e2 = new EndEvent(seg->GetRightPoint(), seg);

        evts->Insert(e1);
        evts->Insert(e2);
    }

    //freopen ("output/10-generation.txt", "w", stdout);

    /** Generating segments graph **/
    MosaicGraph * graph = GenerateSegmentsGraph (evts);
    graph->SaveAsGeoGebraFile("output/segraph.ggb");

    //freopen ("output/20-generated-graph.txt", "w", stdout);
    //cout << *graph << endl;

    //freopen ("output/30-sorted-neighbours.txt", "w", stdout);
    graph->SortNeighborsInCounterClockwiseOrder();
    //cout << *graph << endl;

    //freopen ("output/40-dual-graph.txt", "w", stdout);
    MosaicDualGraph dual(graph);

    //freopen ("output/50-dual-graph.txt", "w", stdout);
    //cout << dual << endl;

    //freopen ("output/60-colors-determining.txt", "w", stdout);
    dual.DetermineAreasColors(gimg);

    freopen ("output/70-components.txt", "w", stdout);
    dual.CalculateComponents();
}


#define WRITE_SWITCH    0
#define WRITE_STEP(X)   if (WRITE_SWITCH) \
                        { \
                            cout << "________________________________________" << endl; \
                            cout << "----------------------------------------" << endl; \
                            cout << "   [ STEP " << ++step << " ]" << endl; \
                            if (step > maxstep) { cout << "TOO MUCH !!!" << endl; exit(1); } \
                            cout << endl; \
                        }
#define WRITE_EVENTS(EVTS, EV)  if (WRITE_SWITCH) \
                                { \
                                    cout << evts << endl; \
                                    cout << "[ EVENT ] : " << endl << evt->GetPoint() << endl; \
                                    if (evt->GetSegment()) cout << evt->GetSegment() << endl; \
                                }
MosaicGraph *
MosaicPMF::GenerateSegmentsGraph (
                                  MosaicEventsSchedule<double> * evts
                                )
{
    MosaicGraph * graph = new MosaicGraph();

    MosaicSweepLineStatus<double> * msls = new MosaicSweepLineStatus<double>();
    int step = 0, maxstep = evts->Size() * evts->Size();
    unsigned int lastLeftId, upperLastId, lowerLastId;

    WRITE_STEP(step);
    assert(evts->SeeFirst()->GetType() == AreaMarkings);
    upperLastId = lastLeftId = graph->CreateNewNode(evts->SeeFirst()->GetPoint()->x, evts->SeeFirst()->GetPoint()->y);
    evts->Erase(evts->SeeFirst());

    // process left border
    while (! evts->IsEmpty()  and  evts->SeeFirst()->GetPoint()->x == 0.0)
    {
        VirtualMosaicEvent * evt = evts->SeeFirst();

        WRITE_STEP(step);
        WRITE_EVENTS(evts, evt);

        int id = graph->CreateNewNode(evt->GetPoint()->x, evt->GetPoint()->y);
        graph->AddEdge(lastLeftId, id, 0);
        lastLeftId = id;
#if (WRITE_SWITCH)
        cout << endl;
#endif
        if (evt->GetType() == BeginSegment)
        {
#if (WRITE_SWITCH)
            cout << "-{" << step << "}-> BEGIN EVENT" << endl;
#endif
            ProcessBeginSegmentEvent(msls, evts, evt);
            evt->GetSegment()->SetLastGraphNodeId( id );
        }
        else if (evt->GetType() == AreaMarkings)
        {
#if (WRITE_SWITCH)
            cout << "-{" << step << "}->  AREA EVENT" << endl;
            cout << evt->GetPoint() << endl;
#endif
        }
        else
            assert("WRONG LEFT EVENT TYPE DURING EVOLUTION" && false);

        evts->Erase(evt);
    }
    lowerLastId = lastLeftId;

#if (WRITE_SWITCH)
    cout << "########################################" << endl;
#endif
    // process between left and right border
    while (! evts->IsEmpty()  and  evts->SeeFirst()->GetPoint()->x < fieldWidth)
    {
        VirtualMosaicEvent * evt = evts->SeeFirst();

        WRITE_STEP(step);
        WRITE_EVENTS(evts, evt);

        std::pair < MosaicSweepLineStatus<double>::Iterator ,  bool  > res;
        MosaicSweepLineStatus<double>::Iterator it, ita, itb;
        MosaicSegment<double> * s1, * s2;

        int id = graph->CreateNewNode(evt->GetPoint()->x, evt->GetPoint()->y);
#if (WRITE_SWITCH)
        cout << endl;
#endif
        switch (evt->GetType())
        {
            case BeginSegment :
                {
#if (WRITE_SWITCH)
                    cout << "-{" << step << "}-> BEGIN EVENT" << endl;
#endif
                    ProcessBeginSegmentEvent(msls, evts, evt);

                    evt->GetSegment()->SetLastGraphNodeId( id );
                    if (evt->GetPoint()->y == fieldHeight)
                    {
                        graph->AddEdge(lowerLastId, id, 0);
                        lowerLastId = id;
                    }
                    else if (evt->GetPoint()->y == 0.)
                    {
                        graph->AddEdge(upperLastId, id, 0);
                        upperLastId = id;
                    }
                    else  assert(" BEGIN EVENT INSIDE THE FIELD " and false);

                    break;;
                }
            case EndOfSegment :
                {
#if (WRITE_SWITCH)
                    cout << "-{" << step << "}->  END  EVENT" << endl;
#endif
                    itb = ita = it = msls->Find(evt->GetSegment());
                    --ita;
                    ++itb;

                    msls->Erase(it);

                    graph->AddEdge( evt->GetSegment()->GetLastGraphNodeId(), id, 2 );
                    evt->GetSegment()->SetLastGraphNodeId( id );
                    if (evt->GetPoint()->y == fieldHeight)
                    {
                        graph->AddEdge(lowerLastId, id, 0);
                        lowerLastId = id;
                    }
                    else if (evt->GetPoint()->y == 0.)
                    {
                        graph->AddEdge(upperLastId, id, 0);
                        upperLastId = id;
                    }
                    else
                        assert(" BEGIN EVENT INSIDE THE FIELD " and false);

                    if (!msls->IsNull(ita)  and  !msls->IsNull(itb))
                    {
#if (WRITE_SWITCH)
                        cout << "END-&-CROSS" << endl;
#endif
                        MosaicSegment<double> * seg1 = (*ita)->GetSegment();
                        MosaicSegment<double> * seg2 = (*itb)->GetSegment();

                        AnalyzeAndPredictIntersection(seg1, seg2, evts, evt->GetPoint()->x);
                    }
                    break;;
                }
            case Intersection :
                {
#if (WRITE_SWITCH)
                    cout << "-{" << step << "}-> CROSS EVENT" << endl;
#endif
                    s1 = evt->GetSegment();
                    s2 = evt->GetSegment(false);

                    msls->Erase(s1);
                    msls->Erase(s2);

                    std::pair < MosaicSweepLineStatus<double>::Iterator ,  bool  > res1 = msls->Insert(evt->GetPoint(), s1);
                    std::pair < MosaicSweepLineStatus<double>::Iterator ,  bool  > res2 = msls->Insert(evt->GetPoint(), s2);

                    assert(res1.ND);
                    graph->AddEdge( s1->GetLastGraphNodeId(), id, 2 );
                    s1->SetLastGraphNodeId( id );
#if (WRITE_SWITCH)
                    cout << s1 << endl;
#endif
                    CheckIntersectionsAfterSwap(msls, res1.ST, res2.ST, s1, evts, evt->GetPoint()->x, 1);

                    assert(res2.ND);
                    graph->AddEdge( s2->GetLastGraphNodeId(), id, 2 );
                    s2->SetLastGraphNodeId( id );
#if (WRITE_SWITCH)
                    cout << s2 << endl;
#endif
                    CheckIntersectionsAfterSwap(msls, res2.ST, res1.ST, s2, evts, evt->GetPoint()->x, 2);

                    break;;
                }
            case AreaMarkings :
                {
#if (WRITE_SWITCH)
                    cout << "-{" << step << "}->  AREA EVENT" << endl;
                    cout << evt->GetPoint() << endl;
#endif
                    break;;
                }
            default :
                    assert("WRONG __EVENT__ TYPE DURING EVOLUTION" && false);
        }
#if (WRITE_SWITCH)
        //cout << endl << msls << endl;
        //cout << *graph << endl;
        cout << "----------------------------------------" << endl;
#endif
        evts->Erase(evt);
    }

#if (WRITE_SWITCH)
    cout << "########################################" << endl;
#endif
    WRITE_STEP(step);
    assert(evts->SeeFirst()->GetType() == AreaMarkings);
    lastLeftId = graph->CreateNewNode(evts->SeeFirst()->GetPoint()->x, evts->SeeFirst()->GetPoint()->y);
    graph->AddEdge(upperLastId, lastLeftId, 0);
    evts->Erase(evts->SeeFirst());

    while (! evts->IsEmpty())
    {
        VirtualMosaicEvent * evt = evts->SeeFirst();

        WRITE_STEP(step);
        WRITE_EVENTS(evts, evt);

        int id = graph->CreateNewNode(evt->GetPoint()->x, evt->GetPoint()->y);
#if (WRITE_SWITCH)
        cout << endl;
#endif
        switch (evt->GetType())
        {
            case EndOfSegment :
                {
#if (WRITE_SWITCH)
                    cout << "-{" << step << "}->  END  EVENT" << endl;
#endif
                    MosaicSweepLineStatus<double>::Iterator it, ita, itb;
                    itb = ita = it = msls->Find(evt->GetSegment());
                    --ita;
                    ++itb;

                    msls->Erase(it);

                    graph->AddEdge( evt->GetSegment()->GetLastGraphNodeId(), id, 2 );
                    evt->GetSegment()->SetLastGraphNodeId( id );

                    if (!msls->IsNull(ita)  and  !msls->IsNull(itb))
                    {
#if (WRITE_SWITCH)
                        cout << "END-&-CROSS" << endl;
#endif
                        MosaicSegment<double> * seg1 = (*ita)->GetSegment();
                        MosaicSegment<double> * seg2 = (*itb)->GetSegment();

                        AnalyzeAndPredictIntersection(seg1, seg2, evts, evt->GetPoint()->x);
                    }
                    break;;
                }
            case AreaMarkings :
                {
#if (WRITE_SWITCH)
                    cout << "-{" << step << "}->  AREA EVENT" << endl;
                    cout << evt->GetPoint() << endl;
#endif
                    break;;
                }
            default :
                    assert("WRONG __EVENT__ TYPE DURING EVOLUTION" && false);
        }
        graph->AddEdge(lastLeftId, id, 0);
        lastLeftId = id;

#if (WRITE_SWITCH)
        cout << endl << msls << endl;
        cout << "----------------------------------------" << endl;
#endif
        evts->Erase(evt);
    }

    graph->AddEdge(lowerLastId, lastLeftId, 0);

    return graph;
}
#undef WRITE_EVENTS
#undef WRITE_STEP



void MosaicPMF::ProcessBeginSegmentEvent (
                                          MosaicSweepLineStatus<double> * msls,
                                          MosaicEventsSchedule<double> * evts,
                                          VirtualMosaicEvent * evt
                                         )
{
    std::pair<MosaicSweepLineStatus<double>::Iterator, bool> res = msls->Insert(evt->GetPoint(), evt->GetSegment());
    assert(res.ND);

    MosaicSweepLineStatus<double>::Iterator ita = msls->Above(res.ST);
    MosaicSweepLineStatus<double>::Iterator itb = msls->Below(res.ST);

    if (! msls->IsNull(ita))
    {
#if (WRITE_SWITCH)
        cout << "ABOVE" << endl << **ita << endl;
#endif
        MosaicSegment<double> * seg1 = (*ita)->GetSegment();
        MosaicSegment<double> * seg2 = evt->GetSegment();

        AnalyzeAndPredictIntersection(seg1, seg2, evts, evt->GetPoint()->x);
    }
    if (! msls->IsNull(itb))
    {
#if (WRITE_SWITCH)
        cout << "BELOW" << endl;
#endif
        MosaicSegment<double> * seg1 = evt->GetSegment();
        MosaicSegment<double> * seg2 = (*itb)->GetSegment();

        AnalyzeAndPredictIntersection(seg1, seg2, evts, evt->GetPoint()->x);
    }
}



void MosaicPMF::CheckIntersectionsAfterSwap (
                                               MosaicSweepLineStatus<double> * msls,
                                               MosaicSweepLineStatus<double>::Iterator & check,
                                               MosaicSweepLineStatus<double>::Iterator & ignore,
                                               MosaicSegment<double> * seg,
                                               MosaicEventsSchedule<double> * evts,
                                               double presentEventCoordinateX,
                                               int test_num
                                            )
{
    MosaicSweepLineStatus<double>::Iterator ita = msls->Above(check);
    MosaicSweepLineStatus<double>::Iterator itb = msls->Below(check);

    if (! msls->IsNull(ita))
    {
        if ( (*ita)->GetSegment() == (*ignore)->GetSegment() )
        {
#if (WRITE_SWITCH)
            cout << "Above" << test_num << " : THE SAME" << endl;
#endif
        }
        else {
#if (WRITE_SWITCH)
            cout << "Above" << test_num << " : DIFFERENT" << endl;
#endif
            MosaicSegment<double> * _seg = (*ita)->GetSegment();
#if (WRITE_SWITCH)
            cout << "Above" << test_num << " : " << _seg << endl;
#endif
            AnalyzeAndPredictIntersection(seg, _seg, evts, presentEventCoordinateX);
        }
    }
    if (! msls->IsNull(itb))
    {
        if ( (*itb)->GetSegment() == (*ignore)->GetSegment() )
        {
#if (WRITE_SWITCH)
            cout << "Below" << test_num << " : THE SAME" << endl;
#endif
        }
        else {
#if (WRITE_SWITCH)
            cout << "Below" << test_num << " : DIFFERENT" << endl;
#endif
            MosaicSegment<double> * _seg = (*itb)->GetSegment();
#if (WRITE_SWITCH)
            cout << "Below" << test_num << " : " << _seg << endl;
#endif
            AnalyzeAndPredictIntersection(seg, _seg, evts, presentEventCoordinateX);
        }
    }
}



void MosaicPMF::AnalyzeAndPredictIntersection (
                                               MosaicSegment<double> * seg1,
                                               MosaicSegment<double> * seg2,
                                               MosaicEventsSchedule<double> * evts,
                                               double presentEventCoordinateX
                                            )
{
    if (
            seg1->GetLeftPoint()->x > seg2->GetLeftPoint()->x
        or
            (seg1->GetLeftPoint()->x == seg2->GetLeftPoint()->x  and   seg1->GetLeftPoint()->y > seg2->GetLeftPoint()->y)
    )
        std::swap(seg1, seg2);

    int check = pmf::Geometry::CheckIntersection2(
                                    seg1->GetLeftPoint()->x, seg1->GetLeftPoint()->y,
                                   seg1->GetRightPoint()->x, seg1->GetRightPoint()->y,
                                    seg2->GetLeftPoint()->x, seg2->GetLeftPoint()->y,
                                   seg2->GetRightPoint()->x, seg2->GetRightPoint()->y );
#if (WRITE_SWITCH)
    cout << " check = " << check << endl;
#endif
    if (check == 1)
    {
        pair<double, double> pt =
            pmf::Geometry::CalculateIntersection(
                                    seg1->GetLeftPoint()->x, seg1->GetLeftPoint()->y,
                                   seg1->GetRightPoint()->x, seg1->GetRightPoint()->y,
                                    seg2->GetLeftPoint()->x, seg2->GetLeftPoint()->y,
                                   seg2->GetRightPoint()->x, seg2->GetRightPoint()->y );
#if (WRITE_SWITCH)
        cout << " -> ( " << pt.ST << " , " << pt.ND << " )" << endl;
#endif
        if (pt.ST > presentEventCoordinateX)
        {
            MosaicPoint<double> * ipt = new MosaicPoint<double>(pt.ST, pt.ND);
            IntersectionEvent * e = new IntersectionEvent(ipt, seg1, seg2);
            evts->Insert(e);
#if (WRITE_SWITCH)
            cout << " -> ADDED" << endl;
#endif
        }
#if (WRITE_SWITCH)
        else
            cout << " -> IGNORED" << endl;
#endif
    }
}
#undef WRITE_SWITCH



MosaicPMF::~MosaicPMF()
{
    //dtor
}


MosaicPMF::MosaicPMF(const MosaicPMF& other)
{
    //copy ctor
}


MosaicPMF& MosaicPMF::operator=(const MosaicPMF& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
