#ifndef MOSAICSCHEDULE_HPP_INCLUDED
#define MOSAICSCHEDULE_HPP_INCLUDED

#define VAR(V, N)       __typeof(N) V = (N)
#define FOREACH(I, C)   for(VAR(I, (C).begin()); I != (C).end(); ++I)

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

        inline
        int Size() { return _events.size(); }

        Iterator End () { return _events.end(); }

        bool Insert(EventPoint e)
        {
            Iterator it = _events.find(e);
            bool res;
            if (it == End())
            {
                res = _events.insert(e).second;
            }
            else {
                res = false;
                if (
                        ( (*it)->GetSegment() == e->GetSegment()  and  (*it)->GetSegment(false) == e->GetSegment(false) )
                    or
                        ( (*it)->GetSegment() == e->GetSegment(false)  and  (*it)->GetSegment(false) == e->GetSegment() )
                )
                    res = true;
            }

            if (! res)
            {
                cout << " RES = " << (res ? "TRUE" : "FALSE" ) << endl;
                cout << this << endl;
                cout << e->GetPoint() << endl;
            }
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

        friend ostream & operator << (ostream & out, const MosaicEventsSchedule * mes)
        {
            out << "[ EVENTS ] :>" << endl;
            //*
            FOREACH(it, mes->_events)  {
                POINT * pt = (*it)->GetPoint();
                out << " -[" << (*it)->GetType() << "]->  " << pt << endl;
            }
            // */
            return out;
        }

    private:
        EventList _events;
};


#endif // MOSAICSCHEDULE_HPP_INCLUDED
