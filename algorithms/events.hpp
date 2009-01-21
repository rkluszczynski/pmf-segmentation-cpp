#ifndef EVENTS_HPP_INCLUDED
#define EVENTS_HPP_INCLUDED

class Event
{
    public:

        Event(POINT * pt) : _pt(pt) {}
        POINT * point() const { return _pt; }

        virtual EventType type() const = 0;
        virtual SEGMENT * segment(bool first = true) const = 0;
        virtual ~Event() {}

    protected:
        POINT * _pt;
};


class SourceEvent : public Event
{
    public:
        SourceEvent(POINT * pt, SEGMENT * s) : Event(pt), _s(s) {}
        EventType type() const { return SOURCE; };
        SEGMENT * segment(bool first) const { return _s; }
    private:
        SEGMENT * _s;
};


class TargetEvent : public Event
{
    public:
        TargetEvent(POINT * pt, SEGMENT * s) : Event(pt), _s(s) {}
        EventType type() const { return TARGET; }
        SEGMENT * segment(bool first) const { return _s; }
    private:
        SEGMENT * _s;
};


class IntersectionEvent : public Event
{
    public:
        IntersectionEvent(POINT * pt, SEGMENT * s1, SEGMENT * s2) : Event(pt), _s1(s1), _s2(s2) {}
        EventType type() const { return INTERSECTION; }
        SEGMENT * segment(bool first) const { return first ? _s1 : _s2; }
    private:
        SEGMENT * _s1;
        SEGMENT * _s2;
};

#endif // EVENTS_HPP_INCLUDED
