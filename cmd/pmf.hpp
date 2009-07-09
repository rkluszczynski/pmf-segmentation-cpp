#ifndef PMF_HPP
#define PMF_HPP

#include "../cmd/configuration.hpp"
#include "../cmd/schedule.hpp"
#include "../cmd/status.hpp"
#include <wx/string.h>


namespace pmf
{

    template <class REAL> class PMF
    {
        public:
            PMF(REAL, REAL);
            virtual ~PMF();

            void GenerateField();
            void RotatePoints (REAL, REAL);

            void SetSeed(time_t);
            bool LoadPMF(const char *);
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
            bool ArrangeNewEvent (Point<REAL> *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            inline
            void ProcessBirthEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            inline
            void ProcessUpdateEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            inline
            void CorrectCollisionStartPoints (Point<REAL> *, long, long);
            inline
            void ProcessDeathEvent (Event *, EventsSchedule<REAL> *, SweepLineStatus<REAL> *, long &);
            inline
            Point<REAL> * DetectPossibleCollision (Segment<REAL> *, Segment<REAL> *, long &);

    };


#include "pmf.cpp"

}


#endif // PMF_HPP
