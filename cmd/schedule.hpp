#ifndef SCHEDULE_HPP_INCLUDED
#define SCHEDULE_HPP_INCLUDED

#include <set>
#include <queue>
#include "events.hpp"

namespace pmf
{
    template <class I>
    struct event_before : binary_function<I,I,bool>
    {
        bool operator () (const I& p1, const I& p2) const
        {
            /*
            bool res = p1->GetPoint()->x < p2->GetPoint()->x;
            cout << "[]  " << p1->point()->x() << " < " << p2->point()->x() << "  " << (res ? "TRUE" : "FALSE") << endl;
            return res;
            //*/
            if(p1->GetPoint()->x < p2->GetPoint()->x) return true;
            if(p1->GetPoint()->x > p2->GetPoint()->x) return false;
            if(p1->GetPoint()->y < p2->GetPoint()->y) return true;
            if(p1->GetPoint()->y > p2->GetPoint()->y) return false;
            /*
            if(p1->type()==SOURCE && p2->type()==TARGET) return true;
            if(p2->type()==SOURCE && p1->type()==TARGET) return false;
            if(p1->type()==SOURCE && p2->type()==INTERSECTION) return true;
            if(p2->type()==SOURCE && p1->type()==INTERSECTION) return false;
            if(p1->type()==INTERSECTION && p2->type()==TARGET) return true;
            if(p2->type()==INTERSECTION && p1->type()==TARGET) return false;

            if(p1->type()==SOURCE && p2->type()==SOURCE && p1->segment()->slope()>p2->segment()->slope()) return true;
            if(p1->type()==SOURCE && p2->type()==SOURCE && p1->segment()->slope()<p2->segment()->slope()) return false;
            if(p1->type()==TARGET && p2->type()==TARGET && p1->segment()->slope()>p2->segment()->slope()) return true;
            if(p1->type()==TARGET && p2->type()==TARGET && p1->segment()->slope()<p2->segment()->slope()) return false;
            //*/
            return false;
        }
    };


    class EventsSchedule
    {
        typedef Point<double> POINT;
        typedef Event * EventPoint;
        typedef std::set<EventPoint, event_before<EventPoint> > EventList;

        public:
            EventsSchedule() {};
            ~EventsSchedule() {};

            bool InsertBirthEvent(POINT * pt);


            bool Insert(EventPoint e)
            {
                bool res = _events.insert(e).ND;
                //cout << " RES = " << (res ? "TRUE" : "FALSE" ) << endl;
                assert(res);
                return res;
            }

            void Erase(EventPoint e)
            {
                _events.erase(e);
                delete e;
            }
            bool IsEmpty() { return _events.empty(); }
            EventPoint SeeFirst() { return *_events.begin(); }

            //EventList * GetEvents() { return & _event_list; }

            friend ostream & operator << (ostream & out, const EventsSchedule * es)
            {
                out << "[ EVENTS ] :>";
                FOREACH(it, es->_events)  out << " " << ((POINT *)((*it)->GetPoint()) );
                return out;
            }

        private:
            EventList _events;
            //std::set<EventPoint, CompareEventPointer> _event_list;
    };


    bool
    EventsSchedule::InsertBirthEvent (POINT * pt)
    {
        BirthEvent * be = new BirthEvent(pt);
        return Insert(be);
    }


}

#endif // SCHEDULE_HPP_INCLUDED
