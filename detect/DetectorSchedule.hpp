#ifndef DETECTORSCHEDULE_HPP
#define DETECTORSCHEDULE_HPP

#include <set>

#include "DetectorEvent.hpp"


template <class EVENT>
struct detector_event_before : std::binary_function<EVENT, EVENT, bool>
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


class DetectorSchedule
{
    typedef detector_event_before<DetectorEvent *> EventComparator;
    typedef DetectorEvent::POINT POINT;
    typedef DetectorEvent * EventPoint;
    typedef std::set<EventPoint, EventComparator > EventList;

    public:
        DetectorSchedule();
        virtual ~DetectorSchedule();

    protected:

    private:

};

#endif // DETECTORSCHEDULE_HPP
