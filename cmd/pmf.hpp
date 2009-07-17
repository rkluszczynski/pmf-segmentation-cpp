#ifndef PMF_HPP
#define PMF_HPP

#include "../cmd/configuration.hpp"
#include "../cmd/schedule.hpp"
#include "../cmd/status.hpp"
#include <wx/string.h>


namespace pmf
{
    typedef enum {
        TextFile,
        SVGFile,
        GeoGebraFile,
        GeoGebraCmds
    }
    PMFFileType;


    template <class REAL> class PMF
    {
        public:
            PMF(REAL, REAL);
            virtual ~PMF();

            void GenerateField();
            void RotatePoints (REAL, REAL);

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


        protected:
            time_t seed;
            Configuration<REAL> * cf;


        private:
            inline
            long GenerateInitialBirths (EventsSchedule<REAL> *);
            inline
            void CheckNewBirthSite (Event *, EventsSchedule<REAL> *, long &);
            inline
            bool IsPointInsideTheField (REAL, REAL);
            inline
            bool ArrangeNewEvent (Point<REAL> *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            void ProcessBirthEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            void ProcessUpdateEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            void CorrectCollisionStartPoints (Point<REAL> *, long, long);
            inline
            void ProcessDeathEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &, REAL, REAL);
            inline
            Point<REAL> * DetectPossibleCollision (Segment<REAL> *, Segment<REAL> *, long &, REAL, REAL);
            inline
            bool IsTheEventInvalid (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *);

            inline
            void PrepareTheEvolution (REAL, REAL, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, REAL);
            inline
            void ProcessOldEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            inline
            bool IsTheEventInvalid (REAL, REAL, Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, set<Point<REAL> *> &, long &);
            inline
            void ForgetOldCollisionPoint (REAL, REAL, Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, set<Point<REAL> *> &, long &);
            inline
            void EvolveTheRestOfField (REAL, REAL, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long);

    };


    std::ostream out(std::cout.rdbuf());


    #include "pmf.cpp"


    typedef PMF<double> DoublePMF;
}


#endif // PMF_HPP
