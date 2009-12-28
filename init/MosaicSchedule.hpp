#ifndef MOSAICSCHEDULE_HPP_INCLUDED
#define MOSAICSCHEDULE_HPP_INCLUDED

#include "../init/MosaicEvents.hpp"


template <class EVENT>
struct mosaic_event_before : binary_function<EVENT,EVENT,bool>
{
    bool operator () (const EVENT & e1, const EVENT & e2) const
    {

        if (e1->GetPoint()->x < e2->GetPoint()->x) return true;
        if (e1->GetPoint()->x > e2->GetPoint()->x) return false;

        if (e1->GetPoint()->y < e2->GetPoint()->y) return true;
        if (e1->GetPoint()->y > e2->GetPoint()->y) return false;

        return false;
    }
};


template <class REAL = double, class COMP = mosaic_event_before<VirtualMosaicEvent *> >
class MosaicEventsSchedule
{
    typedef MosaicPoint<double> POINT;
    typedef VirtualMosaicEvent * EventPoint;
    typedef std::set<EventPoint, COMP > EventList;

    public:
        typedef typename EventList::iterator Iterator;

        MosaicEventsSchedule() {};
        ~MosaicEventsSchedule() {};
    /*
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
    // */

        friend ostream & operator << (ostream & out, const MosaicEventsSchedule * mes)
        {
            out << "[ EVENTS ] :>";
            /*
            FOREACH(it, mes->_events)  {
                POINT * pt = (*it)->GetPoint();
                out << " " << pt;
            }
            // */
            return out;
        }

    private:
        EventList _events;
};


#endif // MOSAICSCHEDULE_HPP_INCLUDED
