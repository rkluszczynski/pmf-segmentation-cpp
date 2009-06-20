#ifndef __LISTELEMENT_HPP__
#define __LISTELEMENT_HPP__

#include <cstdio>


template <class TYPE> struct Element
{
    static long long element_counter;

	TYPE * data;
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
	Element (TYPE * x, Element * y, Element * z = NULL) //: data(x), prev(y), next(z)
	{
	    data = x;
	    prev = y;
	    next = z;
	    ++element_counter;
    }
    ~Element ()  { --element_counter; }
};

template<class TYPE> long long Element<TYPE>::element_counter = 0;

#endif  /* __LISTELEMENT_HPP__ */
