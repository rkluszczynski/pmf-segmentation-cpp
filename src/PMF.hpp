#ifndef PMF_HPP_INCLUDED
#define PMF_HPP_INCLUDED

#include <ctime>

#include "configurationlist.hpp"


template <class T_REAL = double>
class PMF
{
    protected :
        T_REAL fieldWidth, fieldHeight;
        time_t seed;
        ConfigurationList<T_REAL> * pmf;

    public :
        PMF(T_REAL, T_REAL);
		virtual ~PMF();

		void SetSeed(time_t);

};


#endif // PMF_HPP_INCLUDED
