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

            void Generate();

            void SetSeed(time_t _seed) { seed = _seed; }
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

    };


#include "pmf.cpp"

}


#endif // PMF_HPP
