#ifndef PMF_HPP_INCLUDED
#define PMF_HPP_INCLUDED

#include <ctime>
#include <ostream>
#include <iostream>

#include "configurationlist.hpp"
#include "birthsheap.hpp"
#include "intersectionsheap.hpp"
#include "edgepoints.hpp"
#include "blockslists.hpp"


#if pmf_LOG_ADD
std::ostream out(std::cout.rdbuf());
#endif

template <class T_REAL>
class PMF
{
    private :
        /* PMF modification helpers */
        inline
        void PrepareEvolution(BirthsHeap<T_REAL> *, T_REAL, T_REAL, T_REAL, BlocksLists<T_REAL> *);
        inline
        void EvolveRestOfField(BirthsHeap<T_REAL> *, IntersectionsHeap<T_REAL> *, T_REAL, T_REAL, long, long &, BlocksLists<T_REAL> *, EdgePoints<T_REAL> *);
        inline
        void SetPerpendicularNeighbor(BirthsHeap<T_REAL> *, IntersectionsHeap<T_REAL> *, pmf_point<T_REAL> *, pmf_point<T_REAL> *, long &, EdgePoints<T_REAL> *, T_REAL, T_REAL);


    protected :
        T_REAL fieldWidth, fieldHeight;
        time_t seed;
        ConfigurationList<T_REAL> * pmfConf;
        BlocksLists<T_REAL> *   blocksLists;


    public :
        PMF(T_REAL, T_REAL);
		virtual ~PMF();

        PMF<T_REAL> * Clone();
		void Generate(T_REAL);
		void RotatePointTypes(T_REAL, T_REAL);
		void DetermineTypesFromLeftToRight();

		void AddBirthPoint(T_REAL, T_REAL, T_REAL, T_REAL);
		void UpdatePointVelocity(long, T_REAL, T_REAL);
		void RemoveBirthPoint(long, T_REAL, T_REAL);
		void AddBirthSegment(T_REAL, T_REAL, T_REAL, EdgePoints<T_REAL> *);

		void SetSeed(time_t);
		bool LoadConfiguration(const char *);
		bool SaveConfiguration(const char *);
		bool SaveConfigurationAsSVG(const char *, double, double);

		pmf_point<T_REAL> * FindClosestTo(T_REAL, T_REAL);
		pmf_point<T_REAL> * GetPointWithId(long);

		inline ConfigurationList<T_REAL> * GetPMFConfiguration()  { return pmfConf; }
		inline T_REAL GetPMFWidth()  { return fieldWidth; }
		inline T_REAL GetPMFHeight() { return fieldHeight; }
		inline long GetPMFPointsNumber() { return pmfConf->get_size(); }

        inline Element<pmf_point<T_REAL> > * getFirstElement() { return pmfConf->getHead(); }
};

#ifndef CHECK_ASSERTIONS
#error "Directive CHECK_ASSERTIONS not defined"
#endif

#include "PMF.cpp"


#endif // PMF_HPP_INCLUDED
