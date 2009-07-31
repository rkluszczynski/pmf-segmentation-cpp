#ifndef PMF_HPP
#define PMF_HPP

#include "../cmd/configuration.hpp"
#include "../cmd/schedule.hpp"
#include "../cmd/status.hpp"
#include <wx/string.h>
#include "grayimage.h"

namespace pmf
{
    typedef enum {
        TextFile,
        SVGFile,
        GeoGebraFile,
        GeoGebraCmds
    }
    PMFFileType;


    typedef enum {
        BirthEnd,
        DeathEnd
    }
    EndType;

    template <class REAL> class PointComparator
    {
        public:
            bool operator() (Point<REAL> * const & p1, Point<REAL> * const & p2) const
            {
                //*
                if (Geometry::IsZero(p1->x - p2->x))
                {
                    if (p1->type == PT_BirthOnBorder  &&  p2->type != PT_BirthOnBorder) return false;
                    if (p2->type == PT_BirthOnBorder  &&  p1->type != PT_BirthOnBorder) return true;
                    if (p1->type == PT_BirthInField   &&  p2->type != PT_BirthInField) return false;
                    if (p2->type == PT_BirthInField   &&  p1->type != PT_BirthInField) return true;
                    if (p1->type == PT_Update         &&  p2->type != PT_Update) return false;
                    if (p2->type == PT_Update         &&  p1->type != PT_Update) return true;
                    if (p1->type == PT_Collision      &&  p2->type != PT_Collision) return false;
                    if (p2->type == PT_Collision      &&  p1->type != PT_Collision) return true;
                    if (p1->type == PT_DeathOnBorder  &&  p2->type != PT_DeathOnBorder) return false;
                    if (p2->type == PT_DeathOnBorder  &&  p1->type != PT_DeathOnBorder) return true;
                }
                //*/
                return p1->x > p2->x;
            }
    };

    template <class REAL> class PMF
    {
        public:
            PMF(REAL, REAL);
            virtual ~PMF();

            void GenerateField();
            void RotatePoints (REAL, REAL);
            void RotatePoints2 (REAL, REAL);

            void AddBirthPoint (REAL, REAL, REAL);
            void UpdatePointVelocity (long, REAL);
            void RemoveBirthPoint (long, REAL);
            void AddBirthSegment (REAL, REAL, REAL);

            void SetSeed(time_t);
            bool LoadPMF(const char *);
            bool SavePMF(const char *, PMFFileType);
            //bool SaveConfiguration(const char *);
            //bool SaveConfigurationAsSVG(const char *, double, double);
            void DrawPMF (wxMemoryDC& dc, int scale = 200) { cf->DrawConfiguration(dc, scale); }

            inline REAL GetWidth()  { return cf->GetFieldWidth();  }
            inline REAL GetHeight() { return cf->GetFieldHeight(); }
            inline long GetCount()  { return cf->GetPointsCount(); }

            REAL CalculateEnergy(GrayscaleImage * img);

        protected:
            time_t seed;
            Configuration<REAL> * cf;


        private:
            typedef priority_queue<Point<REAL> *, std::vector<Point<REAL> *>, PointComparator<REAL> >  PointPriorityQueue;
            //typedef typename SweepLineStatus<REAL>::Iterator SweepIterator;

            inline
            long GenerateInitialBirths (EventsSchedule<REAL> *);
            inline
            void CheckNewBirthSite (Event *, EventsSchedule<REAL> *, long &);
            inline
            bool IsPointInsideTheField (REAL, REAL);
            inline
            void InsertNewSegmentIntoSweep (Point<REAL> *, Segment<REAL> *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            bool ArrangeNewEvent (Point<REAL> *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL, bool);
            inline
            void ProcessBirthEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            void ProcessUpdateEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            void CorrectCollisionStartPoints (Point<REAL> *, long, long);
            inline
            void CheckIntersectionAfterDeath (Segment<REAL> *, Segment<REAL> *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            void ProcessDeathEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            Point<REAL> * DetectPossibleCollision (Segment<REAL> *, Segment<REAL> *, long &, REAL, REAL);
            inline
            bool IsTheEventInvalid (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *);

            inline
            EndType DetermineN (Point<REAL> *, int, std::vector<bool> &);
            inline
            void PrepareTheEvolution (REAL, REAL, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, PointPriorityQueue &, REAL);
            inline
            void ProcessOldEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            bool IsTheEventInvalid (REAL, REAL, Event * &, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            inline
            void ForgetOldCollisionPoint (REAL, REAL, Point<REAL> *, Segment<REAL> *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            inline
            void EvolveTheRestOfField (REAL, REAL, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long);

    };


    std::ostream out(std::cout.rdbuf());


    #include "pmf.cpp"


    typedef PMF<double> DoublePMF;
}


#endif // PMF_HPP
