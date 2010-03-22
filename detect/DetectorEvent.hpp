#ifndef DETECTOREVENT_HPP
#define DETECTOREVENT_HPP

#include "DetectorSegment.hpp"

typedef enum
{
    BeginSegment = 0,
    EndOfSegment = 1
}
DetectorEventType;


class DetectorEvent
{
    public:
        typedef DetectorPoint<double> POINT;
        typedef DetectorSegment<double> SEGMENT;

        DetectorEvent(POINT *, SEGMENT *, DetectorEventType);
        virtual ~DetectorEvent();

        inline POINT * GetPoint() const { return _pt; }
        inline DetectorEventType GetType() { return _type; }
        inline SEGMENT * GetSegment() { return _sg; }

    protected:
        POINT * _pt;
        SEGMENT * _sg;
        DetectorEventType _type;

    private:
        void OnInit();
};

#endif // DETECTOREVENT_HPP
