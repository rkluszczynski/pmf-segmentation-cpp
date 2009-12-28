#ifndef MOSAICEVENTS_HPP_INCLUDED
#define MOSAICEVENTS_HPP_INCLUDED

#include "MosaicPoint.hpp"
#include "MosaicSegment.hpp"


typedef enum
{
    BeginSegment = 0,
    EndOfSegment = 1,
    Intersection = 2,
    AreaMarkings = 3
}
MosaicEventType;


class VirtualMosaicEvent
{
    private:
        void OnInit() { } //++pmf_event_counter; }

    protected:
        typedef MosaicPoint<double> POINT;
        typedef MosaicSegment<double> SEGMENT;

        POINT * _pt;
        SEGMENT * _s1;

    public:
        VirtualMosaicEvent(POINT * pt, SEGMENT * seg) : _pt(pt), _s1(seg) { OnInit(); }
        POINT * GetPoint() const { return _pt; }

        virtual MosaicEventType GetType() const = 0;
        virtual SEGMENT * GetSegment(bool first = true) const = 0;
        virtual ~VirtualMosaicEvent() { } //--pmf_event_counter; }
};


class BeginEvent : public VirtualMosaicEvent
{
    public:
        BeginEvent(POINT * pt, SEGMENT * s) : VirtualMosaicEvent(pt, s) {}

        MosaicEventType GetType() const { return BeginSegment; };
        SEGMENT * GetSegment(bool first) const { return _s1; }
};


class EndEvent : public VirtualMosaicEvent
{
    public:
        EndEvent(POINT * pt, SEGMENT * s) : VirtualMosaicEvent(pt, s) {}

        MosaicEventType GetType() const { return EndOfSegment; };
        SEGMENT * GetSegment(bool first) const { return _s1; }
};


class IntersectionEvent : public VirtualMosaicEvent
{
    public:
        IntersectionEvent(POINT * pt, SEGMENT * s1, SEGMENT * s2) : VirtualMosaicEvent(pt, s1), _s2(s2) {}

        MosaicEventType GetType() const { return Intersection; }
        SEGMENT * GetSegment(bool first) const { return first ? _s1 : _s2; }

    private:
        SEGMENT * _s2;
};


class AreaEvent : public VirtualMosaicEvent
{
    public:
        AreaEvent(POINT * pt) : VirtualMosaicEvent(pt, NULL) {}

        MosaicEventType GetType() const { return AreaMarkings; };
        SEGMENT * GetSegment(bool first) const { return NULL; }
};


#endif // MOSAICEVENTS_HPP_INCLUDED
