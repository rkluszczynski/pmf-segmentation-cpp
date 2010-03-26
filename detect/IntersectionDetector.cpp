#include "IntersectionDetector.hpp"

#include "DetectorSweepLine.hpp"

#include <cassert>

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)


IntersectionDetector::IntersectionDetector(double eps) : epsilon(eps)
{
    //ctor
}


IntersectionDetector::~IntersectionDetector()
{
    //dtor
    ClearSegments();
}


#define WRITE_SWITCH 0
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
        continue;
        //cout << &dsl << endl;

        switch (evt->GetType())
        {
            case BeginSegment :
                {
                    std::pair<DetectorSweepLine::Iterator, bool> res = dsl.Insert(evt->GetPoint()->x() + epsilon, evt->GetSegment());
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
    assert(seg1->GetBeginPoint()->x() <= seg1->GetEndPoint()->x());
    assert(seg2->GetBeginPoint()->x() <= seg2->GetEndPoint()->x());

    /// Case when two segments starts from the same point (no intersections)
    if (seg1->GetBeginPoint()->x() == seg2->GetBeginPoint()->x()  and  seg1->GetBeginPoint()->y() == seg2->GetBeginPoint()->y())
        return false;

    /// Case when two segments ends at the same point (no intersections)
    if (seg1->GetEndPoint()->x() == seg2->GetEndPoint()->x()  and  seg1->GetEndPoint()->y() == seg2->GetEndPoint()->y())
        return false;

    if (
            seg1->GetBeginPoint()->x() > seg2->GetBeginPoint()->x()
        or
            (seg1->GetBeginPoint()->y() == seg2->GetBeginPoint()->y()  and  seg1->GetBeginPoint()->y() > seg2->GetBeginPoint()->y())
    )
        std::swap(seg1, seg2);

    /// begin of seg1 is less or equal than begin of seg2
    std::pair<REAL, REAL> ipt = CalculateIntersection(
                                    seg1->GetBeginPoint()->x(), seg1->GetBeginPoint()->y(), seg1->GetEndPoint()->x(), seg1->GetEndPoint()->y(),
                                    seg2->GetBeginPoint()->x(), seg2->GetBeginPoint()->y(), seg2->GetEndPoint()->x(), seg2->GetEndPoint()->y()
                                 );
    REAL x = ipt.first;
    REAL y = ipt.second;

    REAL xmin = std::max( std::min(seg1->GetBeginPoint()->x(), seg1->GetEndPoint()->x()), std::min(seg2->GetBeginPoint()->x(), seg2->GetEndPoint()->x()) );
    REAL xmax = std::min( std::max(seg1->GetBeginPoint()->x(), seg1->GetEndPoint()->x()), std::max(seg2->GetBeginPoint()->x(), seg2->GetEndPoint()->x()) );
    REAL ymin = std::max( std::min(seg1->GetBeginPoint()->y(), seg1->GetEndPoint()->y()), std::min(seg2->GetBeginPoint()->y(), seg2->GetEndPoint()->y()) );
    REAL ymax = std::min( std::max(seg1->GetBeginPoint()->y(), seg1->GetEndPoint()->y()), std::max(seg2->GetBeginPoint()->y(), seg2->GetEndPoint()->y()) );

    if (xmin < x  and  x < xmax  and  ymin < y  and  y < ymax)
        return true;

    return false;
}


IntersectionDetector::Point
IntersectionDetector::CalculateIntersection(REAL xp, REAL yp, REAL xq, REAL yq, REAL xr, REAL yr, REAL xs, REAL ys)
{
    if (xr != xs)
    {
        REAL a = (yr - ys) / (xr - xs);
        REAL b = yr - a * xr;
        if (xp != xq)
        {
            REAL c = (yp - yq) / (xp - xq);
            REAL d = yp - c * xp;
            REAL x = (d-b)/(a-c);
            return std::make_pair( x, a * x + b );
        }
        else
        {
            REAL x = .5 * (xp + xq);
            return std::make_pair( x, a * x + b );
        }
    }
    else
    {
        REAL c = (yp - yq) / (xp - xq);
        REAL d = yp - c * xp;
        REAL x = .5 * (xr + xs);
        return std::make_pair(x, c * x + d);
    }
}


void
IntersectionDetector::AddSegment(REAL p_x, REAL p_y, REAL q_x, REAL q_y)
{
    if (p_x > q_x  or  (p_x == q_x  and  p_y > q_y))
    {
        std::swap(p_x, q_x);
        std::swap(p_y, q_y);
    }
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

