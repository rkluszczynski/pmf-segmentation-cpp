#ifndef SCHEDULE_HPP_INCLUDED
#define SCHEDULE_HPP_INCLUDED

#include <set>
#include <queue>
#include "events.hpp"

namespace pmf
{
    template <class EVENT>
    struct event_before : binary_function<EVENT,EVENT,bool>
    {
        bool operator () (const EVENT & e1, const EVENT & e2) const
        {
            /*
            bool res = p1->GetPoint()->x < p2->GetPoint()->x;
            cout << "[]  " << p1->point()->x() << " < " << p2->point()->x() << "  " << (res ? "TRUE" : "FALSE") << endl;
            return res;
            //*/
            using Geometry::IsZero;

            if (! IsZero(e1->GetPoint()->x - e2->GetPoint()->x))
            {
                if (e1->GetPoint()->x < e2->GetPoint()->x) return true;
                if (e1->GetPoint()->x > e2->GetPoint()->x) return false;
            }
            if (e1->GetType() == DeathSite  &&  e2->GetType() != DeathSite) return true;
            if (e2->GetType() == DeathSite  &&  e1->GetType() != DeathSite) return false;
            if (e1->GetType() == PointUpdate  &&  e2->GetType() != PointUpdate) return true;
            if (e2->GetType() == PointUpdate  &&  e1->GetType() != PointUpdate) return false;

            if (! IsZero(e1->GetPoint()->y - e2->GetPoint()->y))
            {
                if (e1->GetPoint()->y < e2->GetPoint()->y) return true;
                if (e1->GetPoint()->y > e2->GetPoint()->y) return false;
            }
            /*
            if(p1->type()==SOURCE && p2->type()==SOURCE && p1->segment()->slope()>p2->segment()->slope()) return true;
            if(p1->type()==SOURCE && p2->type()==SOURCE && p1->segment()->slope()<p2->segment()->slope()) return false;
            if(p1->type()==TARGET && p2->type()==TARGET && p1->segment()->slope()>p2->segment()->slope()) return true;
            if(p1->type()==TARGET && p2->type()==TARGET && p1->segment()->slope()<p2->segment()->slope()) return false;
            //*/
            return false;
        }
    };


    template <class REAL = double>
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
                if (_events.find(e) != _events.end())
                {
                    assert(e->GetType() == DeathSite);

                    EventPoint tmp = (*_events.find(e));
                    _events.erase(_events.find(e));
                    //delete tmp->GetSegment();
                    delete tmp->GetPoint();
                    delete tmp;
                }
                bool res = _events.insert(e).ND;
                //cout << " RES = " << (res ? "TRUE" : "FALSE" ) << endl;
                assert(res == true);
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
                FOREACH(it, es->_events)  {
                    POINT * pt = (*it)->GetPoint();
                    out << " " << pt;
                }
                return out;
            }

        private:
            EventList _events;
            //std::set<EventPoint, CompareEventPointer> _event_list;
    };


    template <class REAL>
    bool
    EventsSchedule<REAL>::InsertBirthEvent (POINT * pt)
    {
        EventType type = (pt->type == PT_BirthOnBorder) ? BorderBirth : NormalBirth;
        BirthEvent * be = new BirthEvent(pt, type);
        return Insert(be);
    }


}

#endif // SCHEDULE_HPP_INCLUDED
