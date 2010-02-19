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

#define UNIFORM(A, B)   (((double(rand()) + 1.) / (double(RAND_MAX) + 2.)) * ((B) - (A)) + (A))



MosaicPMF::MosaicPMF(double w, double h, unsigned int n) : fieldWidth(w), fieldHeight(h), linesNumber(n)
{
    //ctor
    //*
    while (linesParameters.size() < n)
    {
        double alpha = UNIFORM(-M_PI_2, M_PI);
        if (fabs(alpha - M_PI_2) < 1e-7) continue;

        double maxD = 0.;
        double a = tan(alpha);
        if (alpha <= 0.)
            maxD = fabs(w) / sqrt(a*a + 1);  // IV quarter (w * cosL)
        else if (alpha <= M_PI_2)
            maxD = fabs(h * a + w) / sqrt(a*a + 1);  // I quarter
        else if (alpha < M_PI)
            maxD = fabs(a * h) / sqrt(a*a + 1);  // II quarter
        else
            assert("WRONG alpha !" && false);

        double d = UNIFORM(0., maxD);
        linesParameters.insert( make_pair(alpha, d) );

        ///printf("... added :   L = %6.3lf   D = %6.3lf   d = %6.3lf \n", alpha, maxD, d);
    }
    // */
    /*
    linesParameters.insert(make_pair(0.75 * M_PI, 1));
    linesParameters.insert(make_pair(0.25 * M_PI, 1));
    linesParameters.insert(make_pair(-0.25 * M_PI, 1));
    // */

    vector<MosaicSegment<double> *> mosaic;

    FOREACH(it, linesParameters)
    {
        ///cout << endl << " ->   L = " << it->ST << "   d = " << it->ND << endl;

        double a = - 1. / tan(it->ST);
        double b = it->ND / cos(M_PI_2 - it->ST);
        /*
        if (it->ST < 0.)
            b = it->ND / cos(M_PI_2 - it->ST);
        else if (it->ST < M_PI_2)
            b = it->ND / cos(M_PI_2 - it->ST);
        else if (it->ST < M_PI)
            b = it->ND / cos(it->ST - M_PI_2);
        else
            assert(false);
        // */
        ///printf("  >   y  =  %.2lf  *  x  +  %.2lf\n", a, b);

        double tmpx, tmpy;
        vector<pair<double, double> > seg;
        // left
        if (0. < b  and  b < h)
        {
            ///cout << " ( " << 0. << " , " << b << " )" << endl;
            seg.push_back(make_pair(0., b));
        }
        // up
        tmpx = (h - b) / a;
        if (0. < tmpx  and  tmpx < w)
        {
            ///cout << " ( " << tmpx << " , " << h << " )" << endl;
            seg.push_back(make_pair(tmpx, h));
        }
        // right
        tmpy = a * w + b;
        if (0. < tmpy  and  tmpy < h)
        {
            ///cout << " ( " << w << " , " << tmpy << " )" << endl;
            seg.push_back(make_pair(w, tmpy));
        }
        // down
        tmpx = - b / a;
        if (0. < tmpx  and  tmpx < w)
        {
            ///cout << " ( " << tmpx << " , " << 0. << " )" << endl;
            seg.push_back(make_pair(tmpx, 0.));
        }
        assert(seg.size() == 2);

        MosaicPoint<double> * p1 = new MosaicPoint<double>(seg[0].ST, seg[0].ND);
        MosaicPoint<double> * p2 = new MosaicPoint<double>(seg[1].ST, seg[1].ND);
        if (p1->x > p2->x) swap(p1, p2);

        MosaicSegment<double> * s = new MosaicSegment<double>(p1, p2);
        mosaic.push_back(s);

        ///cout << " added segment : " << s << endl;
    }


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

    MosaicSweepLineStatus<double> * msls = new MosaicSweepLineStatus<double>();
    MosaicGraph * graph = new MosaicGraph();
    int step = 0;
    unsigned int lastLeftId, upperLastId, lowerLastId;

    cout << "----------------------------------------" << endl;
    cout << "   [ STEP " << ++step << " ]" << endl;
    cout << endl;

    assert(evts->SeeFirst()->GetType() == AreaMarkings);
    upperLastId = lastLeftId = graph->CreateNewNode(evts->SeeFirst()->GetPoint()->x, evts->SeeFirst()->GetPoint()->y);
    evts->Erase(evts->SeeFirst());

    while (! evts->IsEmpty()  and  evts->SeeFirst()->GetPoint()->x == 0.0)
    {
        cout << "----------------------------------------" << endl;
        cout << "   [ STEP " << ++step << " ]" << endl;
        cout << endl;

        cout << evts << endl;
        VirtualMosaicEvent * evt = evts->SeeFirst();
        cout << "[ EVENT (L) ] : " << endl << evt->GetPoint() << endl;
        if (evt->GetSegment()) cout << evt->GetSegment() << endl;

        int id = graph->CreateNewNode(evt->GetPoint()->x, evt->GetPoint()->y);
        graph->AddEdge(lastLeftId, id);
        lastLeftId = id;

        cout << endl;
        if (evt->GetType() == BeginSegment)
        {
            cout << "-{" << step << "}-> BEGIN EVENT" << endl;
            std::pair < MosaicSweepLineStatus<double>::Iterator ,  bool  > res
                = msls->Insert(evt->GetPoint(), evt->GetSegment());
            assert(res.ND);
            evt->GetSegment()->SetLastGraphNodeId( id );

            MosaicSweepLineStatus<double>::Iterator ita = msls->Above(res.ST);
            MosaicSweepLineStatus<double>::Iterator itb = msls->Below(res.ST);

            if (! msls->IsNull(ita))
            {
                cout << "ABOVE" << endl;
                MosaicSegment<double> * seg1 = (*ita)->GetSegment();
                MosaicSegment<double> * seg2 = evt->GetSegment();

                AnalyzeAndPredictIntersection(seg1, seg2, evts);
            }
            if (! msls->IsNull(itb))
            {
                cout << "BELOW" << endl;
                MosaicSegment<double> * seg1 = evt->GetSegment();
                MosaicSegment<double> * seg2 = (*itb)->GetSegment();

                AnalyzeAndPredictIntersection(seg1, seg2, evts);
            }
        }
        else if (evt->GetType() == AreaMarkings)
        {
            cout << "-{" << step << "}->  AREA EVENT" << endl;
            cout << evt->GetPoint() << endl;
        }
        else
            assert("WRONG LEFT EVENT TYPE DURING EVOLUTION" && false);

        cout << "----------------------------------------" << endl;
        evts->Erase(evt);
    }

    lowerLastId = lastLeftId;
    cout << "########################################" << endl;

    while (! evts->IsEmpty()  and  evts->SeeFirst()->GetPoint()->x < fieldWidth)
    {
        cout << "----------------------------------------" << endl;
        cout << "   [ STEP " << ++step << " ]" << endl;
        cout << endl;

        cout << evts << endl;
        VirtualMosaicEvent * evt = evts->SeeFirst();
        cout << "[ __EVENT__ ] : " << endl << evt->GetPoint() << endl;
        if (evt->GetSegment()) cout << evt->GetSegment() << endl;

        std::pair < MosaicSweepLineStatus<double>::Iterator ,  bool  > res;
        MosaicSweepLineStatus<double>::Iterator it, ita, itb;
        MosaicSegment<double> * s1, * s2;

        int id = graph->CreateNewNode(evt->GetPoint()->x, evt->GetPoint()->y);
        cout << endl;
        switch (evt->GetType())
        {
            case BeginSegment :
                {
                    cout << "-{" << step << "}-> BEGIN EVENT" << endl;
                    res =
                        msls->Insert(evt->GetPoint(), evt->GetSegment());
                    assert(res.ND);

                    evt->GetSegment()->SetLastGraphNodeId( id );
                    if (evt->GetPoint()->y == fieldHeight)
                    {
                        graph->AddEdge(lowerLastId, id);
                        lowerLastId = id;
                    }
                    else if (evt->GetPoint()->y == 0.)
                    {
                        graph->AddEdge(upperLastId, id);
                        upperLastId = id;
                    }
                    else
                        assert(" BEGIN EVENT INSIDE THE FIELD " and false);

                    ita = msls->Above(res.ST);
                    itb = msls->Below(res.ST);

                    if (! msls->IsNull(ita))
                    {
                        cout << "ABOVE" << endl;
                        MosaicSegment<double> * seg1 = (*ita)->GetSegment();
                        MosaicSegment<double> * seg2 = evt->GetSegment();

                        AnalyzeAndPredictIntersection(seg1, seg2, evts);
                    }
                    if (! msls->IsNull(itb))
                    {
                        cout << "BELOW" << endl;
                        MosaicSegment<double> * seg1 = evt->GetSegment();
                        MosaicSegment<double> * seg2 = (*itb)->GetSegment();

                        AnalyzeAndPredictIntersection(seg1, seg2, evts);
                    }
                    break;;
                }
            case EndOfSegment :
                {
                    cout << "-{" << step << "}->  END  EVENT" << endl;

                    itb = ita = it = msls->Find(evt->GetSegment());
                    --ita;
                    ++itb;

                    msls->Erase(it);

                    evt->GetSegment()->SetLastGraphNodeId( id );
                    if (evt->GetPoint()->y == fieldHeight)
                    {
                        graph->AddEdge(lowerLastId, id);
                        lowerLastId = id;
                    }
                    else if (evt->GetPoint()->y == 0.)
                    {
                        graph->AddEdge(upperLastId, id);
                        upperLastId = id;
                    }
                    else
                        assert(" BEGIN EVENT INSIDE THE FIELD " and false);

                    if (!msls->IsNull(ita)  and  !msls->IsNull(itb))
                    {
                        cout << "END-&-CROSS" << endl;

                        MosaicSegment<double> * seg1 = (*ita)->GetSegment();
                        MosaicSegment<double> * seg2 = (*itb)->GetSegment();

                        AnalyzeAndPredictIntersection(seg1, seg2, evts);
                    }
                    break;;
                }
            case Intersection :
                {
                    cout << "-{" << step << "}-> CROSS EVENT" << endl;

                    s1 = evt->GetSegment();
                    s2 = evt->GetSegment(false);

                    msls->Erase(s1);
                    msls->Erase(s2);

                    std::pair < MosaicSweepLineStatus<double>::Iterator ,  bool  > res1 = msls->Insert(evt->GetPoint(), s1);
                    std::pair < MosaicSweepLineStatus<double>::Iterator ,  bool  > res2 = msls->Insert(evt->GetPoint(), s2);

                    assert(res1.ND);
                    graph->AddEdge( s1->GetLastGraphNodeId(), id );
                    s1->SetLastGraphNodeId( id );
                    cout << s1 << endl;
                    CheckIntersectionsAfterSwap(msls, res1.ST, res2.ST, s1, evts, 1);

                    assert(res2.ND);
                    graph->AddEdge( s2->GetLastGraphNodeId(), id );
                    s2->SetLastGraphNodeId( id );
                    cout << s2 << endl;
                    CheckIntersectionsAfterSwap(msls, res2.ST, res1.ST, s2, evts, 2);

                    break;;
                }
            case AreaMarkings :
                {
                    cout << "-{" << step << "}->  AREA EVENT" << endl;
                    cout << evt->GetPoint() << endl;
                    break;;
                }
            default :
                    assert("WRONG __EVENT__ TYPE DURING EVOLUTION" && false);
        }

        cout << endl << msls << endl;
        //cout << *graph << endl;
        cout << "----------------------------------------" << endl;
        evts->Erase(evt);
    }

    cout << "########################################" << endl;

    cout << "----------------------------------------" << endl;
    cout << "   [ STEP " << ++step << " ]" << endl;
    cout << endl;

    assert(evts->SeeFirst()->GetType() == AreaMarkings);
    lastLeftId = graph->CreateNewNode(evts->SeeFirst()->GetPoint()->x, evts->SeeFirst()->GetPoint()->y);
    graph->AddEdge(upperLastId, lastLeftId);
    evts->Erase(evts->SeeFirst());

    while (! evts->IsEmpty())
    {
        cout << "----------------------------------------" << endl;
        cout << "   [ STEP " << ++step << " ]" << endl;
        cout << endl;

        cout << evts << endl;
        VirtualMosaicEvent * evt = evts->SeeFirst();
        cout << "[ EVENT (R) ] : " << endl << evt->GetPoint() << endl;
        if (evt->GetSegment()) cout << evt->GetSegment() << endl;

        std::pair < MosaicSweepLineStatus<double>::Iterator ,  bool  > res;
        MosaicSweepLineStatus<double>::Iterator it, ita, itb;
        MosaicSegment<double> * s1, * s2;

        int id = graph->CreateNewNode(evt->GetPoint()->x, evt->GetPoint()->y);
        cout << endl;
        switch (evt->GetType())
        {
            case EndOfSegment :
                {
                    cout << "-{" << step << "}->  END  EVENT" << endl;

                    itb = ita = it = msls->Find(evt->GetSegment());
                    --ita;
                    ++itb;

                    msls->Erase(it);

                    graph->AddEdge( evt->GetSegment()->GetLastGraphNodeId(), id );
                    evt->GetSegment()->SetLastGraphNodeId( id );

                    if (!msls->IsNull(ita)  and  !msls->IsNull(itb))
                    {
                        cout << "END-&-CROSS" << endl;

                        MosaicSegment<double> * seg1 = (*ita)->GetSegment();
                        MosaicSegment<double> * seg2 = (*itb)->GetSegment();

                        AnalyzeAndPredictIntersection(seg1, seg2, evts);
                    }
                    break;;
                }
            case AreaMarkings :
                {
                    cout << "-{" << step << "}->  AREA EVENT" << endl;
                    cout << evt->GetPoint() << endl;
                    break;;
                }
            default :
                    assert("WRONG __EVENT__ TYPE DURING EVOLUTION" && false);
        }
        graph->AddEdge(lastLeftId, id);
        lastLeftId = id;

        cout << endl << msls << endl;
        cout << "----------------------------------------" << endl;
        evts->Erase(evt);
    }

    graph->AddEdge(lowerLastId, lastLeftId);

    cout << "________________________________________" << endl;
    cout << "########################################" << endl;
    cout << "########################################" << endl;

    cout << *graph << endl;
    graph->SortNeighboursInCounterClockwiseOrder();
    cout << *graph << endl;
}


void MosaicPMF::CheckIntersectionsAfterSwap (
                                               MosaicSweepLineStatus<double> * msls,
                                               MosaicSweepLineStatus<double>::Iterator & check,
                                               MosaicSweepLineStatus<double>::Iterator & ignore,
                                               MosaicSegment<double> * seg,
                                               MosaicEventsSchedule<double> * evts,
                                               int test_num
                                            )
{
    MosaicSweepLineStatus<double>::Iterator ita = msls->Above(check);
    MosaicSweepLineStatus<double>::Iterator itb = msls->Below(check);

    if (! msls->IsNull(ita))
    {
        if ( (*ita)->GetSegment() == (*ignore)->GetSegment() )
        {
            cout << "Above" << test_num << " : THE SAME" << endl;
        }
        else {
            cout << "Above" << test_num << " : DIFFERENT" << endl;
            MosaicSegment<double> * _seg = (*ita)->GetSegment();
            cout << "Above" << test_num << " : " << _seg << endl;
            AnalyzeAndPredictIntersection(seg, _seg, evts);
        }
    }
    if (! msls->IsNull(itb))
    {
        if ( (*itb)->GetSegment() == (*ignore)->GetSegment() )
        {
            cout << "Below" << test_num << " : THE SAME" << endl;
        }
        else {
            cout << "Below" << test_num << " : DIFFERENT" << endl;
            MosaicSegment<double> * _seg = (*itb)->GetSegment();
            cout << "Below" << test_num << " : " << _seg << endl;
            AnalyzeAndPredictIntersection(seg, _seg, evts);
        }
    }
}


void MosaicPMF::AnalyzeAndPredictIntersection (
                                               MosaicSegment<double> * seg1,
                                               MosaicSegment<double> * seg2,
                                               MosaicEventsSchedule<double> * evts
                                            )
{
    int check = pmf::Geometry::CheckIntersection2(
                                    seg1->GetLeftPoint()->x, seg1->GetLeftPoint()->y,
                                   seg1->GetRightPoint()->x, seg1->GetRightPoint()->y,
                                    seg2->GetLeftPoint()->x, seg2->GetLeftPoint()->y,
                                   seg2->GetRightPoint()->x, seg2->GetRightPoint()->y );
    cout << " check = " << check << endl;
    if (check == 1)
    {
        pair<double, double> pt =
            pmf::Geometry::CalculateIntersection(
                                    seg1->GetLeftPoint()->x, seg1->GetLeftPoint()->y,
                                   seg1->GetRightPoint()->x, seg1->GetRightPoint()->y,
                                    seg2->GetLeftPoint()->x, seg2->GetLeftPoint()->y,
                                   seg2->GetRightPoint()->x, seg2->GetRightPoint()->y );

        cout << "  ( " << pt.ST << " , " << pt.ND << " )" << endl;

        MosaicPoint<double> * ipt = new MosaicPoint<double>(pt.ST, pt.ND);
        IntersectionEvent * e = new IntersectionEvent(ipt, seg1, seg2);
        evts->Insert(e);
    }
}


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
