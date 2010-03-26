#ifndef SCHEDULEEVENT_HPP
#define SCHEDULEEVENT_HPP

#include <vector>
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

        inline void AddSegment(SEGMENT * seg) { _segs.push_back(seg); }

        inline POINT * GetPoint() const { return _pt; }
        inline DetectorEventType GetType() { return _type; }
        inline SEGMENT * GetSegment(int i = 0) { return _segs[i]; }

        inline unsigned GetSegmentsSize() { return _segs.size(); }

        friend std::ostream & operator << (std::ostream &, const ScheduleEvent *);

    protected:
        POINT * _pt;
        std::vector<SEGMENT *> _segs;
        DetectorEventType _type;

    private:
        void OnInit();
};


#endif // SCHEDULEEVENT_HPP
