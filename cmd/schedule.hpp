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
            using Geometry::IsZero;
            /*
            bool res = (e1->GetPoint()->x < e2->GetPoint()->x);
            cout << "[]  __________" << endl;
            cout << "[]  " << e1->GetPoint() << "   <   " << e2->GetPoint() << endl;
            cout << "[]   IsZero = " << (IsZero(e1->GetPoint()->x - e2->GetPoint()->x) ? "YES" : "NO") << endl;
            cout.precision(12);
            cout << "[]  " << e1->GetPoint()->x << " < " << e2->GetPoint()->x << "  " << (res ? "TRUE" : "FALSE") << endl;
            // */
            if (! IsZero(e1->GetPoint()->x - e2->GetPoint()->x))
            {
                if (e1->GetPoint()->x < e2->GetPoint()->x) return true;
                if (e1->GetPoint()->x > e2->GetPoint()->x) return false;
            }
            //*
            if (e1->GetType() == OldPoint  &&  e2->GetType() != OldPoint) return true;
            if (e2->GetType() == OldPoint  &&  e1->GetType() != OldPoint) return false;
            // */
            //*
            if (e1->GetType() == OldPoint  &&  e2->GetType() == OldPoint)
            {
                Point<double> * pt1 = e1->GetPoint();
                Point<double> * pt2 = e2->GetPoint();
                if (pt1->type == PT_BirthOnBorder  &&  pt2->type != PT_BirthOnBorder) return true;
                if (pt2->type == PT_BirthOnBorder  &&  pt1->type != PT_BirthOnBorder) return false;
                if (pt1->type == PT_BirthInField   &&  pt2->type != PT_BirthInField) return true;
                if (pt2->type == PT_BirthInField   &&  pt1->type != PT_BirthInField) return false;
                if (pt1->type == PT_Update         &&  pt2->type != PT_Update) return true;
                if (pt2->type == PT_Update         &&  pt1->type != PT_Update) return false;
                if (pt1->type == PT_Collision      &&  pt2->type != PT_Collision) return true;
                if (pt2->type == PT_Collision      &&  pt1->type != PT_Collision) return false;
                if (pt1->type == PT_DeathOnBorder  &&  pt2->type != PT_DeathOnBorder) return true;
                if (pt2->type == PT_DeathOnBorder  &&  pt1->type != PT_DeathOnBorder) return false;

                if (pt1->type == PT_Update         &&  pt2->type == PT_Update) return pt1->id < pt2->id;
                cout << "QQQQQQQQQQQQQQQQQQQQQQQ" << endl;
            }
            // */
            if (e1->GetType() == NormalDeath  &&  e2->GetType() != NormalDeath) return true;
            if (e2->GetType() == NormalDeath  &&  e1->GetType() != NormalDeath) return false;
            if (e1->GetType() == BorderDeath  &&  e2->GetType() != BorderDeath) return true;
            if (e2->GetType() == BorderDeath  &&  e1->GetType() != BorderDeath) return false;
            if (e1->GetType() == NormalBirth  &&  e2->GetType() != NormalBirth) return true;
            if (e2->GetType() == NormalBirth  &&  e1->GetType() != NormalBirth) return false;
            if (e1->GetType() == BorderBirth  &&  e2->GetType() != BorderBirth) return true;
            if (e2->GetType() == BorderBirth  &&  e1->GetType() != BorderBirth) return false;
            if (e1->GetType() == PointUpdate  &&  e2->GetType() != PointUpdate) return true;
            if (e2->GetType() == PointUpdate  &&  e1->GetType() != PointUpdate) return false;

            if (! IsZero(e1->GetPoint()->y - e2->GetPoint()->y))
            {
                if (e1->GetPoint()->y < e2->GetPoint()->y) return true;
                if (e1->GetPoint()->y > e2->GetPoint()->y) return false;
            }


            /**
            static double eps2 = EPSILON * EPSILON;
            double d = (e1->GetPoint()->y - e2->GetPoint()->y)*(e1->GetPoint()->y - e2->GetPoint()->y)
                    + (e1->GetPoint()->x - e2->GetPoint()->x)*(e1->GetPoint()->x - e2->GetPoint()->x);
            assert(eps2 < d);

            if (e1->GetPoint()->x < e2->GetPoint()->x) return true;
            if (e1->GetPoint()->x > e2->GetPoint()->x) return false;

            if (e1->GetPoint()->y < e2->GetPoint()->y) return true;
            if (e1->GetPoint()->y > e2->GetPoint()->y) return false;
            /// */


            /*
                // does not work on seed 3774659 and size 1
            if (e1->GetPoint()->x < e2->GetPoint()->x) return true;
            if (e1->GetPoint()->x > e2->GetPoint()->x) return false;
            // */
            /*
            if(p1->type()==SOURCE && p2->type()==SOURCE && p1->segment()->slope()>p2->segment()->slope()) return true;
            if(p1->type()==SOURCE && p2->type()==SOURCE && p1->segment()->slope()<p2->segment()->slope()) return false;
            if(p1->type()==TARGET && p2->type()==TARGET && p1->segment()->slope()>p2->segment()->slope()) return true;
            if(p1->type()==TARGET && p2->type()==TARGET && p1->segment()->slope()<p2->segment()->slope()) return false;
            // */
            return false;
        }
    };


    template <class EVENT>
    struct event_before_2 : binary_function<EVENT,EVENT,bool>
    {
        bool operator () (const EVENT & e1, const EVENT & e2) const
        {
            using Geometry::IsZero;
            /*
            bool res = (e1->GetPoint()->x < e2->GetPoint()->x);
            cout << "[]  __________" << endl;
            cout << "[]  " << e1->GetPoint() << "   <   " << e2->GetPoint() << endl;
            cout << "[]   IsZero = " << (IsZero(e1->GetPoint()->x - e2->GetPoint()->x) ? "YES" : "NO") << endl;
            cout.precision(12);
            cout << "[]  " << e1->GetPoint()->x << " < " << e2->GetPoint()->x << "  " << (res ? "TRUE" : "FALSE") << endl;
            // */
            if (! IsZero(e1->GetPoint()->x - e2->GetPoint()->x))
            {
                if (e1->GetPoint()->x < e2->GetPoint()->x) return true;
                if (e1->GetPoint()->x > e2->GetPoint()->x) return false;
            }
            if (e1->GetType() == NormalDeath  &&  e2->GetType() != NormalDeath) return true;
            if (e2->GetType() == NormalDeath  &&  e1->GetType() != NormalDeath) return false;
            //*
            if (e1->GetType() == OldPoint  &&  e2->GetType() != OldPoint) return true;
            if (e2->GetType() == OldPoint  &&  e1->GetType() != OldPoint) return false;
            // */
            //*
            if (e1->GetType() == OldPoint  &&  e2->GetType() == OldPoint)
            {
                Point<double> * pt1 = e1->GetPoint();
                Point<double> * pt2 = e2->GetPoint();
                if (pt1->type == PT_BirthOnBorder  &&  pt2->type != PT_BirthOnBorder) return true;
                if (pt2->type == PT_BirthOnBorder  &&  pt1->type != PT_BirthOnBorder) return false;
                if (pt1->type == PT_BirthInField   &&  pt2->type != PT_BirthInField) return true;
                if (pt2->type == PT_BirthInField   &&  pt1->type != PT_BirthInField) return false;
                if (pt1->type == PT_Update         &&  pt2->type != PT_Update) return true;
                if (pt2->type == PT_Update         &&  pt1->type != PT_Update) return false;
                if (pt1->type == PT_Collision      &&  pt2->type != PT_Collision) return true;
                if (pt2->type == PT_Collision      &&  pt1->type != PT_Collision) return false;
                if (pt1->type == PT_DeathOnBorder  &&  pt2->type != PT_DeathOnBorder) return true;
                if (pt2->type == PT_DeathOnBorder  &&  pt1->type != PT_DeathOnBorder) return false;

                if (pt1->type == PT_Update         &&  pt2->type == PT_Update) return pt1->id < pt2->id;
            }
            // */
            if (e1->GetType() == BorderDeath  &&  e2->GetType() != BorderDeath) return true;
            if (e2->GetType() == BorderDeath  &&  e1->GetType() != BorderDeath) return false;
            if (e1->GetType() == NormalBirth  &&  e2->GetType() != NormalBirth) return true;
            if (e2->GetType() == NormalBirth  &&  e1->GetType() != NormalBirth) return false;
            if (e1->GetType() == BorderBirth  &&  e2->GetType() != BorderBirth) return true;
            if (e2->GetType() == BorderBirth  &&  e1->GetType() != BorderBirth) return false;
            if (e1->GetType() == PointUpdate  &&  e2->GetType() != PointUpdate) return true;
            if (e2->GetType() == PointUpdate  &&  e1->GetType() != PointUpdate) return false;

            if (! IsZero(e1->GetPoint()->y - e2->GetPoint()->y))
            {
                if (e1->GetPoint()->y < e2->GetPoint()->y) return true;
                if (e1->GetPoint()->y > e2->GetPoint()->y) return false;
            }


            /**
            if (e1->GetType() == OldPoint  &&  e2->GetType() == OldPoint)
            {
                static double eps2 = EPSILON * EPSILON;
                double d = (e1->GetPoint()->y - e2->GetPoint()->y)*(e1->GetPoint()->y - e2->GetPoint()->y)
                        + (e1->GetPoint()->x - e2->GetPoint()->x)*(e1->GetPoint()->x - e2->GetPoint()->x);
                assert(0. < eps2);
                assert(eps2 < d);

                if (e1->GetPoint()->x < e2->GetPoint()->x) return true;
                if (e1->GetPoint()->x > e2->GetPoint()->x) return false;

                if (e1->GetPoint()->y < e2->GetPoint()->y) return true;
                if (e1->GetPoint()->y > e2->GetPoint()->y) return false;
            }
            /// */


            /*
                // does not work on seed 3774659 and size 1
            if (e1->GetPoint()->x < e2->GetPoint()->x) return true;
            if (e1->GetPoint()->x > e2->GetPoint()->x) return false;
            // */
            return false;
        }
    };


    template <class REAL = double, class COMP = event_before_2<Event *> >
    class EventsSchedule
    {
        typedef Point<double> POINT;
        typedef Event * EventPoint;
        typedef std::set<EventPoint, COMP > EventList;

        public:
            typedef typename EventList::iterator Iterator;

            EventsSchedule() {};
            ~EventsSchedule() {};

            inline size_t size() { return _events.size(); }

            bool InsertBirthEvent(POINT * pt);

            Iterator LowerBound (EventPoint e)
            {
                //if (_events.lower_bound(e) == _events.end()) return NULL;
                return _events.lower_bound(e);
            }
            Iterator End () { return _events.end(); }

            bool Insert(EventPoint e)
            {
                if (_events.find(e) != _events.end())
                {
                    //assert(e->GetType() == NormalDeath);
                    /// NOTE (klusi#3#): when two border points in the same place from different births point is generated again //
                    if (e->GetType() != NormalDeath) return false;
                    assert(e->GetType() == NormalDeath);

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
                long _erased = _events.erase(e);
                assert(_erased > 0);
                delete e;
            }
            bool IsEmpty() { return _events.empty(); }
            EventPoint SeeFirst() { return *_events.begin(); }
            /*
            Point<REAL> * Find(EventPoint e)
            {
                cout << (*_events.find(e)) << endl;
                if (_events.find(e) == _events.end()) return NULL;
                return (*_events.find(e))->GetPoint();
            }
            // */
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


    template <class REAL, class COMP>
    bool
    EventsSchedule<REAL, COMP>::InsertBirthEvent (POINT * pt)
    {
        EventType type = (pt->type == PT_BirthOnBorder) ? BorderBirth : NormalBirth;
        BirthEvent * be = new BirthEvent(pt, type);
        return Insert(be);
    }


}

#endif // SCHEDULE_HPP_INCLUDED
