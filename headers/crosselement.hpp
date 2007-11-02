#ifndef __CROSSELEMENT_HPP__
#define __CROSSELEMENT_HPP__

#include <cstdio>

#include "point.hpp"


template <class REAL> struct CrosspointElement
{
    pmf_point<REAL> * pt;
    long p1, p2;

	CrosspointElement (pmf_point<REAL> * ppt, long pp1, long pp2)
		: pt(ppt), p1(pp1), p2(pp2)
	{}

    bool operator< (const CrosspointElement<REAL> & cpt) const { return(pt->x < cpt.pt->x); }
    bool operator< (const CrosspointElement<REAL> * & cpt) const { return(pt->x < cpt->pt->x); }

    /**
     * Funkcja wypisujaca na ekran informacje o punkcie, dziala na wskazniku.
     **/
	friend std::ostream& operator << (std::ostream& out, const CrosspointElement<REAL> cpt)
	{
		out << (*cpt.pt) << "/" << cpt.p1 << "," << cpt.p2 << "/";
		return out;
	}
};

#endif  /* __CROSSELEMENT_HPP__ */
