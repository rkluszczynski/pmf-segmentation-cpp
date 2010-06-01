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
    public:
        typedef pmf::Point<double> POINT;
        typedef pmf::Segment<double> SEGMENT;

        VirtualPolygonsEvent(POINT * pt, SEGMENT * seg1, SEGMENT * seg2) : _pt(pt), _s1(seg1), _s2(seg2) { OnInit(); }
        POINT * GetPoint() const { return _pt; }

        virtual PolygonsEventType GetType() const = 0;
        ///virtual SEGMENT * GetSegment(bool next = false) const = 0;
        virtual SEGMENT * GetSegment(bool next) const { return next ? _s2 : _s1; }
        virtual ~VirtualPolygonsEvent() { }

    protected:
        POINT * _pt;
        SEGMENT * _s1, * _s2;

    private:
        inline void OnInit() {}
};


class PolygonsBeginEvent : public VirtualPolygonsEvent
{
    public:
        PolygonsBeginEvent(POINT * pt, SEGMENT * s1, SEGMENT * s2) : VirtualPolygonsEvent(pt, s1, s2) {}

        PolygonsEventType GetType() const { return PolygonsBeginSegment; };
};


class PolygonsEndEvent : public VirtualPolygonsEvent
{
    public:
        PolygonsEndEvent(POINT * pt, SEGMENT * s1, SEGMENT * s2) : VirtualPolygonsEvent(pt, s1, s2) {}

        PolygonsEventType GetType() const { return PolygonsEndOfSegment; };
};


class PolygonsUpdateEvent : public VirtualPolygonsEvent
{
    public:
        PolygonsUpdateEvent(POINT * pt, SEGMENT * s1, SEGMENT * s2) : VirtualPolygonsEvent(pt, s1, s2) {}

        PolygonsEventType GetType() const { return PolygonsUpdateSegment; }
};


#endif // POLYGONSEVENTS_HPP_INCLUDED