#include "IntersectionDetector.hpp"

#include <cassert>

#include "DetectorSweepLine.hpp"

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)


IntersectionDetector::IntersectionDetector()
{
    //ctor
}


IntersectionDetector::~IntersectionDetector()
{
    //dtor
}

#define WRITE_SWITCH 1
bool
IntersectionDetector::CheckIntersectionExistance()
{
    using std::cout;
    using std::endl;

    cout << "[ BEGIN ]  IntersectionDetector::CheckIntersectionExistance()" << endl;

    DetectorSweepLine dsl;
    FOREACH(eit, schedule)
    {
        ScheduleEvent * evt = *eit;

        cout << evt << endl;
        cout << &dsl << endl;

        switch (evt->GetType())
        {
            case BeginSegment :
                {
                    std::pair<DetectorSweepLine::Iterator, bool> res = dsl.Insert(evt->GetPoint()->x(), evt->GetSegment());
                    assert(res.second);
                    DetectorSweepLine::Iterator it = res.first;

                    DetectorSweepLine::Iterator ita = dsl.Above(it);
                    DetectorSweepLine::Iterator itb = dsl.Below(it);

                    if (! dsl.IsNull(ita))
                    {
#if (WRITE_SWITCH)
                        cout << "ABOVE  :=  " << **ita << endl;
#endif
                        DetectorSegment<REAL> * seg1 = (*ita)->GetSegment();
                        DetectorSegment<REAL> * seg2 = evt->GetSegment();

                        if (DoSegmentsIntersect(seg1, seg2))  return true;
                    }
                    if (! dsl.IsNull(itb))
                    {
#if (WRITE_SWITCH)
                        cout << "BELOW  :=  " << **itb << endl;
#endif
                        DetectorSegment<REAL> * seg1 = (*itb)->GetSegment();
                        DetectorSegment<REAL> * seg2 = evt->GetSegment();

                        if (DoSegmentsIntersect(seg1, seg2))  return true;
                    }
                    break;;
                }
            case EndOfSegment :
                {
                    DetectorSweepLine::Iterator it = dsl.Find(evt->GetSegment());

                    DetectorSweepLine::Iterator ita = dsl.Above(it);
                    DetectorSweepLine::Iterator itb = dsl.Below(it);

                    dsl.Erase(it);

                    if (!dsl.IsNull(ita)  and  !dsl.IsNull(itb))
                    {
#if (WRITE_SWITCH)
                        cout << "END-&-CROSS" << endl;
#endif
                        DetectorSegment<REAL> * seg1 = (*ita)->GetSegment();
                        DetectorSegment<REAL> * seg2 = (*itb)->GetSegment();

                        if (DoSegmentsIntersect(seg1, seg2))  return true;
                    }
                    break;;
                }
            default :
                assert(false and "WRONG DetectTypeEvent !!!");
        }
    }

    cout << "[ _END_ ]  IntersectionDetector::CheckIntersectionExistance()" << endl;
    return false;
}
#undef WRITE_SWITCH

bool
IntersectionDetector::DoSegmentsIntersect(DetectorSegment<REAL> * seg1, DetectorSegment<REAL> * seg2)
{

    return false;
}


void
IntersectionDetector::AddSegment(REAL p_x, REAL p_y, REAL q_x, REAL q_y)
{
    segments.push_back( std::make_pair( std::make_pair(p_x, p_y), std::make_pair(q_x, q_y) ) );

    DetectorPoint<REAL>   * ptP = new DetectorPoint<REAL>(p_x, p_y);
    DetectorPoint<REAL>   * ptQ = new DetectorPoint<REAL>(q_x, q_y);
    DetectorSegment<REAL> * seg = new DetectorSegment<REAL>(ptP, ptQ);

    ScheduleEvent * evtp = new ScheduleEvent(ptP, seg, BeginSegment);
    ScheduleEvent * evtq = new ScheduleEvent(ptQ, seg, EndOfSegment);

    schedule.Insert(evtp);
    schedule.Insert(evtq);
}


void
IntersectionDetector::ClearSegments()
{
    segments.clear();
    FOREACH(eit, schedule)
    {
        delete (*eit)->GetPoint();
        if ((*eit)->GetType() == EndOfSegment)
            delete (*eit)->GetSegment();
    }
    schedule.Clear();
}

