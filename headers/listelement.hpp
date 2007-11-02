#ifndef __LISTELEMENT_HPP__
#define __LISTELEMENT_HPP__

#include <cstdio>

template <class TYPE> struct Element
{
	const TYPE * data;
	Element * next;
	Element * prev;
    /*
	Element (const TYPE x, Element * y, Element * z = NULL)
		: prev(y), next(z)
	{
        TYPE * tmp = new TYPE;
        *tmp = x;
	    data = tmp;
    }
    */
	Element (const TYPE * x, Element * y, Element * z = NULL)
		: data(x), prev(y), next(z)
	{}
};

#endif  /* __LISTELEMENT_HPP__ */
