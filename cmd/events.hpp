#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

namespace pmf
{
    typedef Point<double> POINT;
    typedef enum {
                    NormalBirth = 0,
                    BorderBirth = 1,
                    PointUpdate = 2,
                    DeathSite  = 3
    }
    EventType;


    class Event
    {
        public:
            Event(POINT * pt) : _pt(pt) {}
            POINT * GetPoint() const { return _pt; }

            virtual EventType GetType() const = 0;
            //virtual SEGMENT * GetSegment(bool first = true) const = 0;
            virtual ~Event() {}

        protected:
            POINT * _pt;
    };


    class BirthEvent : public Event
    {
        public:
            //BirthEvent(POINT * pt, SEGMENT * s) : Event(pt), _s(s) {}
            BirthEvent(POINT * pt) : Event(pt), _type(NormalBirth) {}
            BirthEvent(POINT * pt, EventType type) : Event(pt), _type(type) {}
            EventType GetType() const { return _type; };
            //SEGMENT * segment(bool first) const { return _s; }
        private:
            //SEGMENT * _s;
            EventType _type;
    };


    class UpdateEvent : public Event
    {
        public:
            //TargetEvent(POINT * pt, SEGMENT * s) : Event(pt), _s(s) {}
            UpdateEvent(POINT * pt) : Event(pt) {}
            EventType GetType() const { return PointUpdate; }
            //SEGMENT * segment(bool first) const { return _s; }
        private:
            //SEGMENT * _s;
    };


    class DeathEvent : public Event
    {
        public:
            //DeathEvent(POINT * pt, SEGMENT * s1, SEGMENT * s2) : Event(pt), _s1(s1), _s2(s2) {}
            DeathEvent(POINT * pt) : Event(pt) {}
            EventType GetType() const { return DeathSite; }
            //SEGMENT * segment(bool first) const { return first ? _s1 : _s2; }
        private:
            //SEGMENT * _s1;
            //SEGMENT * _s2;
    };

}

#endif // EVENTS_HPP_INCLUDED
