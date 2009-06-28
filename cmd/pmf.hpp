#ifndef PMF_HPP
#define PMF_HPP

#include "..\cmd\configuration.hpp"

namespace pmf
{

    template <class REAL> class PMF
    {
        public:
            PMF(REAL, REAL);
            virtual ~PMF();

            void GenerateField();

            void SetSeed(time_t _seed);
            //bool LoadConfiguration(const char *);
            //bool SaveConfiguration(const char *);
            //bool SaveConfigurationAsSVG(const char *, double, double);

            inline REAL GetWidth()  { return cf->GetFieldWidth();  }
            inline REAL GetHeight() { return cf->GetFieldHeight(); }
            //inline long GetCount()  { return cf->; }


        protected:
            time_t seed;
            Configuration<REAL> * cf;


        private:
            inline
            long GenerateInitialBirths (EventsSchedule *);
            inline
            void CheckNewBirthSite (Event *, EventsSchedule *, long &);
            inline
            void ArrangeNewEvent (Point<REAL> *, EventsSchedule *, long &);
            inline
            void ProcessBirthEvent (Event *, EventsSchedule *, long &);
            inline
            void ProcessUpdateEvent (Event *, EventsSchedule *, long &);
            inline
            void ProcessDeathEvent (Event *, EventsSchedule *, long &);

    };


#include "pmf.cpp"

}


#endif // PMF_HPP
