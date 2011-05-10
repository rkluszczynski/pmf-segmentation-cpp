#ifndef PMF_HPP
#define PMF_HPP

#include "../cmd/configuration.hpp"
#include "../cmd/schedule.hpp"
#include "../cmd/status.hpp"
#include <wx/string.h>
#include "../include/statistics.h"
#include "grayimage.h"
#include <list>

#include "DoublePRNG.h"


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
            PointComparator(REAL eps) : epsilon(eps) { }
            bool operator() (Point<REAL> * const & p1, Point<REAL> * const & p2) const
            {
                //*
                if (Geometry::IsZero(p1->x - p2->x, epsilon))
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

                    if (p1->type == PT_Update         &&  p2->type == PT_Update)
                    {
                        if (p1->n2->id == p2->id  &&  p2->n1->id == p1->id)  return false;
                        if (p2->n2->id == p1->id  &&  p1->n1->id == p2->id)  return true;
                    }

                    //if (SegmentationParameters::_trigger == 1)
                        if (p1->x != p2->x)  return p1->x > p2->x;

                    if (! Geometry::IsZero(p1->y - p2->y, epsilon))
                    {
                        if (p1->y < p2->y) return false;
                        if (p1->y > p2->y) return true;
                    }
                }
                //*/
                return p1->x > p2->x;
            }
        private:
            REAL epsilon;
    };

    class SegmentMapComparator
    {
        public:
            bool operator() (const pair<long,long> & e1, const pair<long,long> & e2) const
            {
                return e1.ST < e2.ST || (e1.ST == e2.ST  &&  e1.ND < e2.ND);
            }
    };


    template <class REAL> class PMF
    {
        public:
            //PMF(REAL, REAL);
            PMF(REAL, REAL, NumericalParameters);
            virtual ~PMF();

            PMF<REAL> * Clone();
            void GenerateField();
            void RotatePoints (REAL, REAL);
            void RotatePoints2 (REAL);
            void RotatePoints2 (REAL, REAL, bool = true);

            bool AddBirthPoint (REAL, REAL, REAL, REAL);
            void UpdatePointVelocity (long, REAL, REAL);
            void RemoveBirthPoint (long, REAL, REAL);
            void AddBirthSegment (REAL, REAL, REAL, REAL);

            void EraseSmallPolygons (REAL);

            inline void AddBirthPoint (REAL, REAL, REAL);
            inline void UpdatePointVelocity (long, REAL);
            inline void RemoveBirthPoint (long, REAL);
            inline void AddBirthSegment (REAL, REAL, REAL);

            bool TestPointsCoincidence ();
            bool IsFreeEpsilonCoincidence (REAL, REAL);
            bool IsOldBirthBeforeOldDeathInEpsilonDistance ();

            void SetSeed(time_t);
            void SetPRNG(DoublePRNG *);
            bool LoadPMF(const char *);
            bool SavePMF(const char *, PMFFileType);
            //bool SaveConfiguration(const char *);
            //bool SaveConfigurationAsSVG(const char *, double, double);
            void DrawPMF (wxMemoryDC & dc, int scale = 200) { cf->DrawConfiguration(dc, scale); }

            inline void PushBack (Point<REAL> * pt)  { cf->PushBack(pt); }
            Configuration<REAL> * GetCf ()  { return cf; }

            inline REAL GetWidth()  { return cf->GetFieldWidth();  }
            inline REAL GetHeight() { return cf->GetFieldHeight(); }
            inline long GetCount()  { return cf->GetPointsCount(); }
            inline pmf::Statistics GetStatistics() { return cf->GetStatistics(); }

            REAL CalculateGrayscaleImageEnergyTerm(GrayscaleImage * img);
            REAL CalculateLennardJonesEnergyTerm (REAL, REAL, REAL, REAL);
            pair<REAL, REAL> ScanVerticalLine(GrayscaleImage *, REAL, REAL);

            //inline
            std::ostream & GetOutStream() { return out; }
            //inline
            void SetOutStream(std::ostream & ostr) { out.rdbuf(ostr.rdbuf()); }

            inline
            pmf::DoublePRNG * GetPRNG()
            {
                //assert(prng != NULL);
                return prng;
            }


        protected:
            time_t seed;
            Configuration<REAL> * cf;

            DoublePRNG * prng;
            NumericalParameters nparams;

            std::ostream out;


        private:
            typedef priority_queue<Point<REAL> *, std::vector<Point<REAL> *>, PointComparator<REAL> >  PointPriorityQueue;
            typedef map<pair<long, long>, Segment<REAL> *, SegmentMapComparator>                              SegmentsMap;
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
            void PrepareTheEvolution (EventsSchedule<REAL> *, SweepLineStatus<REAL> *, PointPriorityQueue &, SegmentsMap &, REAL);
            inline
            void ProcessOldEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            bool IsTheEventInvalid (REAL, REAL, Event * &, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            inline
            bool CheckExistanceAfterForgeting (Point<REAL> *, REAL, REAL, Point<REAL> *, Point<REAL> *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            inline
            //void ForgetOldCollisionPoint (REAL, REAL, Point<REAL> *, Segment<REAL> *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            void ForgetOldCollisionPoint (REAL, REAL, Point<REAL> *, Point<REAL> *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            inline
            void EvolveTheRestOfField (REAL, REAL, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long);

    };


    //std::ostream out(std::cout.rdbuf());


    #include "pmf.cpp"


    typedef PMF<double> DoublePMF;
}


#endif // PMF_HPP
