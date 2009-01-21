#ifndef SCHEDULE_HPP_INCLUDED
#define SCHEDULE_HPP_INCLUDED

#include <set>
#include <queue>
#include "events.hpp"

template <class I>
struct point_before: binary_function<I,I,bool>
{
    bool operator () (const I& p1, const I& p2) const
    {
        /*
        bool res = p1->point()->x() < p2->point()->x();
        cout << "[]  " << p1->point()->x() << " < " << p2->point()->x() << "  " << (res ? "TRUE" : "FALSE") << endl;
        return res;
        //*/
        if(p1->point()->x() < p2->point()->x()) return true;
        if(p1->point()->x() > p2->point()->x()) return false;
        if(p1->point()->y() < p2->point()->y()) return true;
        if(p1->point()->y() > p2->point()->y()) return false;

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

        return false;
    }
};

class EventPointSchedule
{
    public:
        typedef Event* EventPoint;
        typedef std::set<EventPoint, point_before<EventPoint> > EventList;

        bool insert(EventPoint e)
        {
            //return _event_list.insert(e).ND;
            EventList::iterator it = _event_list.find(e);
            if (it == _event_list.end()) return _event_list.insert(e).ND;

            Event * ie = *it;
            /*
            cout << "QQQ  " << *e->point() << "  " << *e->segment(true) << "  " << *e->segment(false) << endl;
            cout << "     " << e->segment(true)->slope() << endl;
            cout << "     " << e->segment(false)->slope() << endl;
            cout << "QQQ  " << *ie->point() << "  " << *ie->segment(true) << "  " << *ie->segment(false) << endl;
            cout << "     " << ie->segment(true)->slope() << endl;
            cout << "     " << ie->segment(false)->slope() << endl;
            //*/
            Types::Segment * s1, * s2;
            s1 = (ie->segment(true)->slope() < e->segment(true)->slope()) ? ie->segment(true) : e->segment(true);
            s2 = (ie->segment(false)->slope() > e->segment(false)->slope()) ? ie->segment(false) : e->segment(false);
            //cout << " s1  : " << *s1 << endl;
            //cout << " s2  : " << *s2 << endl;
            IntersectionEvent * evt;
            evt = new IntersectionEvent(e->point(), s1, s2);
            _event_list.erase(it);
            bool res = _event_list.insert(evt).ND;
            //cout << " RES = " << (res ? "TRUE" : "FALSE" ) << endl;
            return res;
        }
        void remove(EventPoint e)
        {
            _event_list.erase(e);
            delete e;
        }
        bool empty() { return _event_list.begin() == _event_list.end(); }
        EventPoint top() { return *_event_list.begin(); }

        //EventList * GetEvents() { return & _event_list; }

        friend ostream & operator << (ostream & out, const EventPointSchedule & eps)
        {
            cout << "[ EVENTS ] :";
            FOREACH(it, eps._event_list)  out << " " << *(*it)->point();
            return out;
        }

    //private:
        EventList _event_list;
        //std::set<EventPoint, CompareEventPointer> _event_list;
};

#endif // SCHEDULE_HPP_INCLUDED
