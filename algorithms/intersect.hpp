#ifndef INTERSECT_HPP_INCLUDED
#define INTERSECT_HPP_INCLUDED

#include <vector>

#include "schedule.hpp"
#include "sweepline.hpp"

class SegmentsIntersections
{
    typedef Types::Segment SEGMENT;
    typedef Types::Point   POINT;
    typedef Types::NT      NT;
    typedef Types::FT      FT;
    typedef MutableSegmentSet LSTATUS;

    public :
        SegmentsIntersections(int n = 0) : _segments(n) { }
        ~SegmentsIntersections() { }

        SEGMENT & operator[] (const int index) { return _segments[index]; }

        void PushBack(NT x1, NT y1, NT x2, NT y2)
        {
            assert(x1 != x2  ||  y1 != y2);
            if ((x1 > x2) || (x1 == x2 && y1 > y2)) { swap(x1,x2); swap(y1,y2); }
            POINT p1(x1, y1), p2(x2, y2);
            SEGMENT seg(p1, p2);
            _segments.push_back(seg);
        }

        void Execute()
        {
            EventPointSchedule events;
            LSTATUS line_status;

            REP(i, SIZE(_segments))
            {
                events.insert( new SourceEvent( (_segments[i].GetPointerP()), &_segments[i] ) );
                events.insert( new TargetEvent( (_segments[i].GetPointerQ()), &_segments[i] ) );
            }

            while (! events.empty())
            {
                cout << events << endl;

                Event * evt = events.top();
                PrintEventInfo(evt);
                switch (evt->type())
                {
                    case SOURCE :
                        ProcessSourceEvent(evt, events, line_status); break;;
                    case TARGET :
                        ProcessTargetEvent(evt, events, line_status); break;;
                    case INTERSECTION :
                        ProcessIntersectionEvent(evt, events, line_status); break;;
                    default :  assert(false);
                }
                events.remove(evt);
            }
        }

        void WriteResult()
        {
            cout << "[ DONE ]" << endl;
        }


    private :
        vector<SEGMENT> _segments;


        void ProcessSourceEvent(Event * evt, EventPointSchedule & events, LSTATUS & line_status)
        {
            cout << "... starting processing source vertex" << endl;

            POINT * pt = evt->point();
            SEGMENT * seg = evt->segment();
            LSTATUS::Iterator s = line_status.Insert(*evt->point(), evt->segment()).ST;
            LSTATUS::Iterator s1 = line_status.Above(s);
            LSTATUS::Iterator s2 = line_status.Below(s);

            if(! line_status.IsNull(s1))
            {
                if(Types::isOnSegment(*pt, *seg->GetPointerP(), *seg->GetPointerQ()) == 1 &&  Types::AreCollinear(*seg, *(s1->segment())))
                {
                    ProcessSourceCollinearityCase(evt, events, line_status);
                }
                else {
                    POINT * cpt;
                    if((cpt = Types::AnalizeSegments(*(s->segment()), *(s1->segment()))) != NULL) events.insert( new IntersectionEvent( cpt, s->segment(), s1->segment() ) );
                }

            }
            if(! line_status.IsNull(s2))
            {
                POINT * cpt;
                if((cpt = Types::AnalizeSegments(*(s->segment()), *(s2->segment()))) != NULL) events.insert( new IntersectionEvent( cpt, s->segment(), s2->segment() ) );
            }

            cout << "... done source vertex" << endl;
        }


        void ProcessSourceCollinearityCase(Event * evt, EventPointSchedule & events, LSTATUS & line_status)
        {
            cout << "... starting source collinearity case" << endl;

            cout << "... done collinearity case" << endl;
        }


        void ProcessTargetEvent(Event * evt, EventPointSchedule & events, LSTATUS & line_status)
        {
            cout << "... starting processing target vertex" << endl;

            POINT * pt = evt->point();
            SEGMENT * seg = evt->segment();
            LSTATUS::Iterator s = line_status.Find(seg);
            line_status.Remove(s);

            LSTATUS::Iterator s1 = line_status.Above(s);
            LSTATUS::Iterator s2 = line_status.Below(s);
            if((! line_status.IsNull(s1)) && (! line_status.IsNull(s2)))
            {
                if((! Types::AreCollinear(*seg, *(s1->segment()))) && (! Types::AreCollinear(*seg, *(s2->segment()))))
                {
                    POINT * cpt;
                    if((cpt = Types::AnalizeSegments(*(s1->segment()), *(s2->segment()))) != NULL)
                        if(cpt->x() > pt->x()) events.insert( new IntersectionEvent( cpt, s1->segment(), s2->segment() ) );
                }
                else {
                    /// TODO :  for all segments s' collinear with s
                    ///             report their common segment with s
                }
            }

            cout << "... done target vertex" << endl;
        }


        void ProcessIntersectionEvent(Event * evt, EventPointSchedule & events, LSTATUS & line_status)
        {
            cout << "... starting processing intersection vertex" << endl;

            cout << "... done intersection vertex" << endl;
        }


        void PrintEventInfo(Event * evt)
        {
            cout << *evt->point() << "  [";
            switch (evt->type())
            {
                case SOURCE : cout << "SOURCE"; break;
                case TARGET : cout << "TARGET"; break;
                case INTERSECTION : cout << "INTERSECTION"; break;
                default : assert(false);
            }
            cout << "]";

            cout << endl;
        }
};

#endif // INTERSECT_HPP_INCLUDED
