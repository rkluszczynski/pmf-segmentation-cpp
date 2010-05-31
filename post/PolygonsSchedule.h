#ifndef POLYGONSSCHEDULE_H
#define POLYGONSSCHEDULE_H

#include <set>
#include "../cmd/point.hpp"
#include "../cmd/geometry.hpp"

#include "PolygonsEvents.hpp"


template <class EVENT>
struct polygons_event_before : binary_function<EVENT,EVENT,bool>
{
    bool operator () (const EVENT & e1, const EVENT & e2) const
    {
        using pmf::Geometry::IsZero;
        pmf::Point<double>* pt1 = e1->GetPoint();
        pmf::Point<double>* pt2 = e2->GetPoint();

        if (! IsZero(pt1->x - pt2->x))
        {
            if (pt1->x < pt2->x) return true;
            if (pt1->x > pt2->x) return false;
        }

        if (pt1->type == pmf::PT_BirthOnBorder  &&  pt2->type != pmf::PT_BirthOnBorder) return true;
        if (pt2->type == pmf::PT_BirthOnBorder  &&  pt1->type != pmf::PT_BirthOnBorder) return false;
        if (pt1->type == pmf::PT_BirthInField   &&  pt2->type != pmf::PT_BirthInField) return true;
        if (pt2->type == pmf::PT_BirthInField   &&  pt1->type != pmf::PT_BirthInField) return false;
        if (pt1->type == pmf::PT_Update         &&  pt2->type != pmf::PT_Update) return true;
        if (pt2->type == pmf::PT_Update         &&  pt1->type != pmf::PT_Update) return false;
        if (pt1->type == pmf::PT_Collision      &&  pt2->type != pmf::PT_Collision) return true;
        if (pt2->type == pmf::PT_Collision      &&  pt1->type != pmf::PT_Collision) return false;
        if (pt1->type == pmf::PT_DeathOnBorder  &&  pt2->type != pmf::PT_DeathOnBorder) return true;
        if (pt2->type == pmf::PT_DeathOnBorder  &&  pt1->type != pmf::PT_DeathOnBorder) return false;

        if (pt1->type == pmf::PT_Update         &&  pt2->type == pmf::PT_Update) return pt1->id < pt2->id;

        if (! IsZero(pt1->y - pt2->y))
        {
            if (pt1->y < pt2->y) return true;
            if (pt1->y > pt2->y) return false;
        }
        return false;
    }
};


class PolygonsSchedule
{

    public:
        typedef VirtualPolygonsEvent::POINT* POINT;
        typedef VirtualPolygonsEvent::SEGMENT* SEGMENT;

        typedef VirtualPolygonsEvent* Event;
        typedef std::set<Event, polygons_event_before<Event> > EventList;
        typedef EventList::iterator Iterator;

        PolygonsSchedule();
        virtual ~PolygonsSchedule();

        inline
        unsigned Size() { return _events.size(); }
        inline
        bool IsEmpty() { return _events.empty(); }

        Iterator Begin () { return _events.begin(); }
        Iterator End () { return _events.end(); }

        Event SeeFirst() { return *_events.begin(); }

        bool Insert(POINT, SEGMENT, SEGMENT);
        void Erase(Event);

        friend std::ostream & operator << (std::ostream &, const PolygonsSchedule &);

    protected:

    private:
        EventList _events;
};

#endif // POLYGONSSCHEDULE_H
