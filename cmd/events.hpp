#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

#include "../cmd/segment.hpp"

namespace pmf
{
    long long   pmf_event_counter = 0;

    typedef enum {
                    NormalBirth = 0,
                    BorderBirth = 1,
                    PointUpdate = 2,
                    NormalDeath = 3,
                    BorderDeath = 4,
                    OldPoint    = 5
    }
    EventType;


    template <typename REAL> struct Segment;

    class Event
    {
        private:
            void OnInit() { ++pmf_event_counter; }

        protected:
            typedef Point<double> POINT;
            typedef Segment<double> SEGMENT;

            POINT * _pt;

        public:
            Event(POINT * pt) : _pt(pt) { OnInit(); }
            POINT * GetPoint() const { return _pt; }

            virtual EventType GetType() const = 0;
            virtual SEGMENT * GetSegment(bool first = true) const = 0;
            virtual ~Event() { --pmf_event_counter; }

            friend std::ostream& operator << (std::ostream & out, const Event & evt)
            {
                return out;
            }
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

            EventType GetType() const { return ((_s2 != NULL) ? NormalDeath : BorderDeath); }
            SEGMENT * GetSegment(bool first) const { return first ? _s1 : _s2; }

        private:
            SEGMENT * _s1;
            SEGMENT * _s2;
    };


    class OldEvent : public Event
    {
        public:
            OldEvent(POINT * pt, SEGMENT * s1) : Event(pt), _s1(s1), _s2(NULL) {}
            OldEvent(POINT * pt, SEGMENT * s1, SEGMENT * s2) : Event(pt), _s1(s1), _s2(s2) {}

            EventType GetType() const { return OldPoint; }
            SEGMENT * GetSegment(bool first) const { return first ? _s1 : _s2; }

        private:
            SEGMENT * _s1;
            SEGMENT * _s2;
    };

}

#endif // EVENTS_HPP_INCLUDED
