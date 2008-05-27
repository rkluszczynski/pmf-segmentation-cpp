#ifndef PMF_HPP_INCLUDED
#define PMF_HPP_INCLUDED

#include <ctime>
#include <ostream>
#include <iostream>

#include "configurationlist.hpp"
#include "birthsheap.hpp"
#include "intersectionsheap.hpp"


#if pmf_LOG_ADD
std::ostream out(std::cout.rdbuf());
#endif

template <class T_REAL>
class PMF
{
    private :
        inline
        void EvolveRestOfField(BirthsHeap<T_REAL> *, IntersectionsHeap<T_REAL> *, T_REAL, T_REAL, long, long &);

    protected :
        T_REAL fieldWidth, fieldHeight;
        time_t seed;
        ConfigurationList<T_REAL> * pmfConf;

    public :
        PMF(T_REAL, T_REAL);
		virtual ~PMF();

		void Generate(T_REAL);
		void RotatePointTypes(T_REAL, T_REAL);
		void AddBirthPoint(T_REAL, T_REAL, T_REAL);
		void DetermineTypesFromLeftToRight();
		//void RedirectUpdatePoint(long, T_REAL);
		void ChangePointVelocity(long, T_REAL);
		void RemoveBirthPoint(long, T_REAL);

		void SetSeed(time_t);
		pmf_point<T_REAL> * FindClosestTo(T_REAL, T_REAL);

		bool LoadConfiguration(const char *);
		bool SaveConfiguration(const char *);
		bool SaveConfigurationAsSVG(const char *, double, double);

		inline T_REAL GetPMFWidth()  { return fieldWidth; }
		inline T_REAL GetPMFHeight() { return fieldHeight; }

        inline Element<pmf_point<T_REAL> > * getFirstElement() { return pmfConf->getHead(); }
};

#ifndef CHECK_ASSERTIONS
#error "Directive CHECK_ASSERTIONS not defined"
#endif

#include "PMF.cpp"


#endif // PMF_HPP_INCLUDED
