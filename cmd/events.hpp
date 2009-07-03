#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

#include "../cmd/segment.hpp"

namespace pmf
{
    typedef enum {
                    NormalBirth = 0,
                    BorderBirth = 1,
                    PointUpdate = 2,
                    DeathSite  = 3
    }
    EventType;


    template <typename REAL> struct Segment;

    class Event
    {
        protected:
            typedef Point<double> POINT;
            typedef Segment<double> SEGMENT;

            POINT * _pt;

        public:
            Event(POINT * pt) : _pt(pt) {}
            POINT * GetPoint() const { return _pt; }

            virtual EventType GetType() const = 0;
            virtual SEGMENT * GetSegment(bool first = true) const = 0;
            virtual ~Event() {}
    };


    class BirthEvent : public Event
    {
        public:
            BirthEvent(POINT * pt) : Event(pt), _type(NormalBirth) {}
            BirthEvent(POINT * pt, EventType type) : Event(pt), _type(type) {}

            EventType GetType() const { return _type; };
            SEGMENT * GetSegment(bool first) const { return NULL; }

        private:
            EventType _type;
    };


    class UpdateEvent : public Event
    {
        public:
            UpdateEvent(POINT * pt, SEGMENT * s) : Event(pt), _s(s) {}

            EventType GetType() const { return PointUpdate; }
            SEGMENT * GetSegment(bool first) const { return _s; }

        private:
            SEGMENT * _s;
    };


    class DeathEvent : public Event
    {
        public:
            DeathEvent(POINT * pt, SEGMENT * s1) : Event(pt), _s1(s1), _s2(NULL) {}
            DeathEvent(POINT * pt, SEGMENT * s1, SEGMENT * s2) : Event(pt), _s1(s1), _s2(s2) {}

            EventType GetType() const { return DeathSite; }
            SEGMENT * GetSegment(bool first) const { return first ? _s1 : _s2; }

        private:
            SEGMENT * _s1;
            SEGMENT * _s2;
    };

}

#endif // EVENTS_HPP_INCLUDED
