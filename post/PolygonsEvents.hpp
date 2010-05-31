#ifndef POLYGONSEVENTS_HPP_INCLUDED
#define POLYGONSEVENTS_HPP_INCLUDED

#include "../cmd/point.hpp"
#include "../cmd/segment.hpp"

typedef enum
{
    PolygonsBeginSegment,
    PolygonsEndOfSegment,
    PolygonsUpdateSegment
}
PolygonsEventType;


class VirtualPolygonsEvent
{
    private:
        inline void OnInit() {}

    protected:
        typedef pmf::Point<double> POINT;
        typedef pmf::Segment<double> SEGMENT;

        POINT * _pt;
        SEGMENT * _s1;

    public:
        VirtualPolygonsEvent(POINT * pt, SEGMENT * seg) : _pt(pt), _s1(seg) { OnInit(); }
        POINT * GetPoint() const { return _pt; }

        virtual PolygonsEventType GetType() const = 0;
        virtual SEGMENT * GetSegment(bool next = false) const = 0;
        virtual ~VirtualPolygonsEvent() { }
};


class PolygonsBeginEvent : public VirtualPolygonsEvent
{
    public:
        PolygonsBeginEvent(POINT * pt, SEGMENT * s) : VirtualPolygonsEvent(pt, s) {}

        PolygonsEventType GetType() const { return PolygonsBeginSegment; };
        SEGMENT * GetSegment(bool next) const { return _s1; }
};


class PolygonsEndEvent : public VirtualPolygonsEvent
{
    public:
        PolygonsEndEvent(POINT * pt, SEGMENT * s) : VirtualPolygonsEvent(pt, s) {}

        PolygonsEventType GetType() const { return PolygonsEndOfSegment; };
        SEGMENT * GetSegment(bool next) const { return _s1; }
};


class PolygonsUpdateEvent : public VirtualPolygonsEvent
{
    public:
        PolygonsUpdateEvent(POINT * pt, SEGMENT * s1, SEGMENT * s2) : VirtualPolygonsEvent(pt, s1), _s2(s2) {}

        PolygonsEventType GetType() const { return PolygonsUpdateSegment; }
        SEGMENT * GetSegment(bool next) const { return next ? _s2 : _s1; }

    private:
        SEGMENT * _s2;
};


#endif // POLYGONSEVENTS_HPP_INCLUDED
