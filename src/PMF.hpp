#ifndef PMF_HPP_INCLUDED
#define PMF_HPP_INCLUDED

#include <ctime>
#include <ostream>
#include <iostream>

#include "configurationlist.hpp"

#if pmf_LOG_ADD
std::ostream out(std::cout.rdbuf());
#endif

template <class T_REAL>
class PMF
{
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

		void SetSeed(time_t);
		pmf_point<T_REAL> * FindClosestTo(T_REAL, T_REAL);

		bool LoadConfiguration(const char *);
		bool SaveConfiguration(const char *);
		bool SaveConfigurationAsSVG(const char *, double, double);

        inline Element<pmf_point<T_REAL> > * getFirstElement() { return pmfConf->getHead(); }
};

#ifndef CHECK_ASSERTIONS
#error "Directive CHECK_ASSERTIONS not defined"
#endif

#include "PMF.cpp"


#endif // PMF_HPP_INCLUDED
