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
    vector<MosaicSegment<double> *> mosaic;

    GenerateRandomSegmentsByPolarParameters (n, mosaic);

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

    MosaicGraph * graph = GenerateSegmentsGraph (evts);
    /*
    cout << "________________________________________" << endl;
    cout << "########################################" << endl;
    cout << *graph << endl;
    // */
    graph->SortNeighboursInCounterClockwiseOrder();
    cout << *graph << endl;

    //*
    MosaicGraph * other = new MosaicGraph(*graph);
    cout << *other << endl;


    std::vector<std::vector<int> > areas;
    std::vector<std::vector<int> > areaGraph;

    std::map<std::pair<int, int>, int> memory;

    //int i = 0;
    FOR(i, 0, other->Size()-1)
    {
        MosaicGraphNode * node = other->Get(i);
        while (node->n.size() > 0)
        {
            std::vector<int> area;
            areaGraph.push_back(area);

            int a = -1;
            int b = i;
            int j = 1;
            area.push_back(i);
            do
            {
                a = b;
                j = (j == 0) ? other->Get(b)->n.size()-1 : j-1;
                b = other->Get(b)->n[ j ].first;

                j = 0;
                while (other->Get(b)->n[j].first != a) ++j;

                //cout << " WAY  : " << a << "  " << b << "   (j=" << j << ")" << endl;
                area.push_back(b);

                if (other->Get(b)->n[j].second > 1)
                {
                    --other->Get(b)->n[j].second;
                    int jj = 0;
                    while (other->Get(a)->n[jj].first != b)  ++jj;
                    --other->Get(a)->n[jj].second;

                    memory[make_pair(a, b)] = memory[make_pair(b, a)] = areaGraph.size()-1;
                }
                else {
                    //*
                    std::map<std::pair<int, int>, int>::iterator it = memory.find(make_pair(a, b));
                    if (it != memory.end())
                    {
                        int anum = it->second;
                        areaGraph[ anum ].push_back( areaGraph.size()-1 );
                        areaGraph[ areaGraph.size()-1 ].push_back( anum );
                    }
                    // */
                    other->RemoveEdge(a, b);
                }
                //cout << *other << endl;
            }
            while (b != i);

            areas.push_back(area);
        }
    }
    cout << *other << endl;

    int i = 0;
    FOREACH(it, areas)
    {
        cout << "[ AREA " << i++ << " ] :";
        FOREACH(iit, *it) cout << " " << *iit;
        cout << endl;
    }

    std::cout << " ++++++++++++++++++++++++++++++++ " << std::endl;

    i = 0;
    FOREACH(it, areaGraph)
    {
        cout << "[ " << i++ << " ] :";
        FOREACH(iit, *it) cout << " " << *iit;
        cout << endl;
    }
    // */


    std::vector<int> areasColors(areaGraph.size());
    FOREACH(it, areasColors)  *it = rand() % 2;

    cout << "[ COLORS ] :";
    FOR(j, 0, areasColors.size()-1) cout << " " << j << "{" << areasColors[j] << "}";
    cout << endl;

    std::vector<int> components(areaGraph.size());
    fill_n(components.begin(), areaGraph.size(), -1);
    int counter = 0;
    FOR(j, 0, areaGraph.size()-1)
        if (components[j] == -1)
        {
            std::stack<int> st;
            st.push(j);

            while (! st.empty())
            {
                int v = st.top();
                components[v] = counter;
                st.pop();

                FOREACH(it, areaGraph[v])
                {
                    if (components[*it] == -1  and  areasColors[*it] == areasColors[v])
                    {
                        st.push(*it);
                    }
                }
            }
            ++counter;
        }

    cout << "[ COMPONENTS ] :";
    FOR(j, 0, components.size()-1) cout << " " << j << "[" << components[j] << "]";
    cout << endl;


    MosaicGraph * final = new MosaicGraph(*graph);
    FOR(k, 0, counter-1)
    {
        std::set<std::pair<int, int> > edges;
        cout << "[ COMPONENT " << k << " ]" << endl;

        FOR(j, 0, components.size()-1)
        {
            if (components[j] == k)
            {
                int mem_b = -1;
                FOREACH(it, areas[j])
                {
                    int a = mem_b;
                    int b = *it;
                    mem_b = b;
                    if (a > b) std::swap(a,b);

                    if (a >= 0)
                    {
                        if (edges.find(make_pair(a,b)) == edges.end())
                        {
                            cout << " ->  INSERT  (" << a << "," << b << ")" << endl;
                            edges.insert(make_pair(a,b));
                        }
                        else {
                            cout << " ->  REMOVE  (" << a << "," << b << ")" << endl;
                            final->RemoveEdge(a, b);
                        }
                    }
                }
            }
        }
    }
    cout << *final << endl;
    // removing vertex degree 2 with collinear neigbours
    FOR(i, 0, final->Size()-1)
    {
        MosaicGraphNode * node = final->Get(i);
        if (node->n.size() == 2)
        {
            int n1 = node->n[0].first;
            int n2 = node->n[1].first;
            int d1 = node->n[0].second;
            int d2 = node->n[1].second;

            double x0 = node->x;
            double y0 = node->y;
            double x1 = final->Get( n1 )->x;
            double y1 = final->Get( n1 )->y;
            double x2 = final->Get( n2 )->x;
            double y2 = final->Get( n2 )->y;

            double dx01 = x1 - x0;
            double dy01 = y1 - y0;
            double dx02 = x2 - x0;
            double dy02 = y2 - y0;

            double val = dx01 * dy02 - dx02 * dy01;
            if (-1e-8 < val  and  val < 1e-8)
            {
                //cout << i << " " << n1 << " " << n2 << endl;
                assert(d1 == d2);
                final->RemoveEdge(i, n1);
                final->RemoveEdge(i, n2);
                final->AddEdge(n1, n2, d1);
            }
        }
    }
    cout << *final << endl;

}


#define WRITE_STEP(X)   { \
                            cout << "________________________________________" << endl; \
                            cout << "----------------------------------------" << endl; \
                            cout << "   [ STEP " << ++step << " ]" << endl; \
                            cout << endl; \
                        }
#define WRITE_EVENTS(EVTS, EV)  { \
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
    int step = 0;
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

        cout << endl;
        if (evt->GetType() == BeginSegment)
        {
            cout << "-{" << step << "}-> BEGIN EVENT" << endl;
            ProcessBeginSegmentEvent(msls, evts, evt);
            evt->GetSegment()->SetLastGraphNodeId( id );
        }
        else if (evt->GetType() == AreaMarkings)
        {
            cout << "-{" << step << "}->  AREA EVENT" << endl;
            cout << evt->GetPoint() << endl;
        }
        else
            assert("WRONG LEFT EVENT TYPE DURING EVOLUTION" && false);

        evts->Erase(evt);
    }
    lowerLastId = lastLeftId;

    cout << "########################################" << endl;
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
        cout << endl;
        switch (evt->GetType())
        {
            case BeginSegment :
                {
                    cout << "-{" << step << "}-> BEGIN EVENT" << endl;
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
                    cout << "-{" << step << "}->  END  EVENT" << endl;

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
                    graph->AddEdge( s1->GetLastGraphNodeId(), id, 2 );
                    s1->SetLastGraphNodeId( id );
                    cout << s1 << endl;
                    CheckIntersectionsAfterSwap(msls, res1.ST, res2.ST, s1, evts, 1);

                    assert(res2.ND);
                    graph->AddEdge( s2->GetLastGraphNodeId(), id, 2 );
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
        cout << endl;
        switch (evt->GetType())
        {
            case EndOfSegment :
                {
                    cout << "-{" << step << "}->  END  EVENT" << endl;

                    MosaicSweepLineStatus<double>::Iterator it, ita, itb;
                    itb = ita = it = msls->Find(evt->GetSegment());
                    --ita;
                    ++itb;

                    msls->Erase(it);

                    graph->AddEdge( evt->GetSegment()->GetLastGraphNodeId(), id, 2 );
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
        graph->AddEdge(lastLeftId, id, 0);
        lastLeftId = id;

        cout << endl << msls << endl;
        cout << "----------------------------------------" << endl;
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


void
MosaicPMF::GenerateRandomSegmentsByPolarParameters (unsigned int amount, vector<MosaicSegment<double> *> & mosaic)
{
    set<pair<double, double> > linesParameters;
    //*
    while (linesParameters.size() < amount)
    {
        double alpha = UNIFORM(-M_PI_2, M_PI);
        if (fabs(alpha - M_PI_2) < 1e-7) continue;

        double maxD = 0.;
        double a = tan(alpha);
        if (alpha <= 0.)
            maxD = fabs(fieldWidth) / sqrt(a*a + 1);  // IV quarter (w * cosL)
        else if (alpha <= M_PI_2)
            maxD = fabs(fieldHeight * a + fieldWidth) / sqrt(a*a + 1);  // I quarter
        else if (alpha < M_PI)
            maxD = fabs(a * fieldHeight) / sqrt(a*a + 1);  // II quarter
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

    mosaic.clear();
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
        if (0. < b  and  b < fieldHeight)
        {
            ///cout << " ( " << 0. << " , " << b << " )" << endl;
            seg.push_back(make_pair(0., b));
        }
        // up
        tmpx = (fieldHeight - b) / a;
        if (0. < tmpx  and  tmpx < fieldWidth)
        {
            ///cout << " ( " << tmpx << " , " << h << " )" << endl;
            seg.push_back(make_pair(tmpx, fieldHeight));
        }
        // right
        tmpy = a * fieldWidth + b;
        if (0. < tmpy  and  tmpy < fieldHeight)
        {
            ///cout << " ( " << w << " , " << tmpy << " )" << endl;
            seg.push_back(make_pair(fieldWidth, tmpy));
        }
        // down
        tmpx = - b / a;
        if (0. < tmpx  and  tmpx < fieldWidth)
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
    return;
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
