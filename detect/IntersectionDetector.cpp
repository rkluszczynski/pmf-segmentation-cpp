#include "IntersectionDetector.hpp"

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
    }

    cout << "[ _END_ ]  IntersectionDetector::CheckIntersectionExistance()" << endl;
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

