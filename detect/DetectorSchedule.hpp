#ifndef DETECTORSCHEDULE_HPP
#define DETECTORSCHEDULE_HPP

#include <set>

#include "ScheduleEvent.hpp"


template <class EVENT>
struct detector_event_before : std::binary_function<EVENT, EVENT, bool>
{
    bool operator () (const EVENT & e1, const EVENT & e2) const
    {
        if (e1->GetPoint()->x() < e2->GetPoint()->x()) return true;
        if (e1->GetPoint()->x() > e2->GetPoint()->x()) return false;

        if (e1->GetPoint()->y() < e2->GetPoint()->y()) return true;
        if (e1->GetPoint()->y() > e2->GetPoint()->y()) return false;

        return false;
    }
};


class DetectorSchedule
{
    typedef ScheduleEvent * EventPoint;
    typedef detector_event_before<EventPoint> EventComparator;
    typedef std::set<EventPoint, EventComparator > EventList;

    public:
        typedef ScheduleEvent::POINT   POINT;
        typedef EventList::iterator Iterator;

        DetectorSchedule();
        virtual ~DetectorSchedule();

        inline int       Size() { return _events.size(); }
        inline Iterator Begin() { return _events.end(); }
        inline Iterator   End() { return _events.end(); }
        inline bool   IsEmpty() { return _events.empty(); }

        bool Insert(EventPoint);
        void Erase(EventPoint);

        friend std::ostream & operator << (std::ostream &, const DetectorSchedule *);

    protected:

    private:
        EventList _events;
};

#endif // DETECTORSCHEDULE_HPP
