#ifndef SCHEDULEEVENT_HPP
#define SCHEDULEEVENT_HPP

#include "DetectorSegment.hpp"

typedef enum
{
    BeginSegment = 0,
    EndOfSegment = 1
}
DetectorEventType;


class ScheduleEvent
{
    public:
        typedef DetectorPoint<double> POINT;
        typedef DetectorSegment<double> SEGMENT;

        ScheduleEvent(POINT *, SEGMENT *, DetectorEventType);
        virtual ~ScheduleEvent();

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


#endif // SCHEDULEEVENT_HPP
