#ifndef SCHEDULEEVENT_HPP
#define SCHEDULEEVENT_HPP

#include <set>
#include "DetectorSegment.hpp"

typedef enum
{
    BeginSegment = 0,
    EndOfSegment = 1
}
DetectorEventType;


struct segments_order : std::binary_function<DetectorSegment<double> *, DetectorSegment<double> *, bool>
{
    bool operator () ( DetectorSegment<double> * const & e1, DetectorSegment<double> * const & e2)
    {
        if (e1->GetBeginPoint()->x() < e2->GetBeginPoint()->x()) return true;
        if (e1->GetBeginPoint()->x() > e2->GetBeginPoint()->x()) return false;

        if (e1->GetBeginPoint()->y() < e2->GetBeginPoint()->y()) return true;
        if (e1->GetBeginPoint()->y() > e2->GetBeginPoint()->y()) return false;

        if (e1->GetEndPoint()->x() < e2->GetEndPoint()->x()) return true;
        if (e1->GetEndPoint()->x() > e2->GetEndPoint()->x()) return false;

        if (e1->GetEndPoint()->y() < e2->GetEndPoint()->y()) return true;
        if (e1->GetEndPoint()->y() > e2->GetEndPoint()->y()) return false;

        return false;
    }
};


class ScheduleEvent
{
    public:
        typedef DetectorPoint<double> POINT;
        typedef DetectorSegment<double> SEGMENT;
        typedef std::set<SEGMENT *, segments_order> SegmentsSet;
        typedef SegmentsSet::iterator Iterator;

        ScheduleEvent(POINT *, SEGMENT *, DetectorEventType);
        virtual ~ScheduleEvent();

        inline Iterator begin() { return _segs.begin(); }
        inline Iterator   end() { return _segs.end(); }

        inline void AddSegment(SEGMENT * seg) { _segs.insert(seg); }
        inline void EraseSegment(SEGMENT * seg) { _segs.erase(seg); }

        inline POINT * GetPoint() const { return _pt; }
        inline DetectorEventType GetType() { return _type; }
        inline SEGMENT * GetSegment(bool last = false) { return last ? *(--_segs.end()) : *_segs.begin(); }

        inline unsigned GetSegmentsSize() { return _segs.size(); }

        friend std::ostream & operator << (std::ostream &, const ScheduleEvent *);

    protected:
        POINT * _pt;
        SegmentsSet _segs;
        DetectorEventType _type;

    private:
        void OnInit();
};


#endif // SCHEDULEEVENT_HPP
