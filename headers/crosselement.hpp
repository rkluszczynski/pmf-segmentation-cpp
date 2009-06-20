#ifndef __CROSSELEMENT_HPP__
#define __CROSSELEMENT_HPP__

#include <cstdio>

#include "point.hpp"


template <class T_REAL> struct CrosspointElement
{
    static long long crosselement_counter;
    pmf_point<T_REAL> * pt;
    long p1, p2;

	CrosspointElement (pmf_point<T_REAL> * ppt, long pp1, long pp2)
		: pt(ppt), p1(pp1), p2(pp2)
	{
	    ++crosselement_counter;
    }

	~CrosspointElement ()
	{
	    --crosselement_counter;
    }

    bool operator< (const CrosspointElement<T_REAL> & cpt) const { return(pt->x < cpt.pt->x); }
    bool operator< (const CrosspointElement<T_REAL> * & cpt) const { return(pt->x < cpt->pt->x); }

    /**
     * Funkcja wypisujaca na ekran informacje o punkcie, dziala na wskazniku.
     **/
	friend std::ostream& operator << (std::ostream& out, const CrosspointElement<T_REAL> cpt)
	{
	    --crosselement_counter;
		out << (*cpt.pt) << "/" << cpt.p1 << "," << cpt.p2 << "/";
		return out;
	}
};
/*
template <class T>
class MyClass
{
    // static member declaration
    static int count;
    ...
};
// static member definition
template<class T> int MyClass<T>::count = 0;
//*/
template<class T_REAL> long long CrosspointElement<T_REAL>::crosselement_counter = 0;


#endif  /* __CROSSELEMENT_HPP__ */
