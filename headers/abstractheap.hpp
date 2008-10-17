#ifndef TEMPLATEHEAP_HPP_INCLUDED
#define TEMPLATEHEAP_HPP_INCLUDED

#include <iostream>
#include <algorithm>
#include <vector>
#include <assert.h>

using namespace std;

#define PMF_HEAP_PARENT(X)   (((X)-1)>>1)
#define PMF_HEAP_LEFT(X)     (((X)<<1) + 1)
#define PMF_HEAP_RIGHT(X)    (((X)<<1) + 2)

template <class T_TYPE>
class AbstractHeap
{
    private :
        virtual bool __less_then (T_TYPE, T_TYPE) = 0;
        inline bool __less_equal (T_TYPE, T_TYPE);

    protected :
        vector<T_TYPE> * data;
        void min_heapify (long);

    public :
        AbstractHeap();
        virtual ~AbstractHeap()
        {
            delete data;
            //std::cout << "[ PMF HEAP ]  destroyed" << std::endl;
        };

        inline long   size() { return data->size(); }
        inline bool  empty() { return data->size() == 0; }
        inline T_TYPE  top() { return (*data)[0]; }
        inline T_TYPE  get(long i) { return (*data)[i]; }

        T_TYPE extract_min ();
        void insert (T_TYPE);
};


template <class T_TYPE>
AbstractHeap<T_TYPE>::AbstractHeap ()
{
    data = new vector<T_TYPE> ();
	//std::cout << "[ PMF HEAP ]  created" << std::endl;
};

//*
template <class T_TYPE>
void
AbstractHeap<T_TYPE>::min_heapify (long index)
{
    while (true)
    {
        long l = PMF_HEAP_LEFT(index);
        long r = PMF_HEAP_RIGHT(index);
        long best = index;

        if (l < size()  &&  __less_then((*data)[l], (*data)[best]))  best = l;
        if (r < size()  &&  __less_then((*data)[r], (*data)[best]))  best = r;
        if (best == index)  break;

        swap((*data)[index], (*data)[best]);
        index = best;
    }
}//*/
/*
template <class T_TYPE>
void
AbstractHeap<T_TYPE>::min_heapify (long index)
{
    long l = PMF_HEAP_LEFT(index);
    long r = PMF_HEAP_RIGHT(index);
    long best = index;

    //if (l < size()  &&  (*data)[l] > (*data)[largest])  largest = l;
    //if (r < size()  &&  (*data)[r] > (*data)[largest])  largest = r;
    if (l < size()  &&  __less_then((*data)[l], (*data)[best]))  best = l;
    if (r < size()  &&  __less_then((*data)[r], (*data)[best]))  best = r;
    if (best != index)  {
        swap((*data)[index], (*data)[best]);
        min_heapify(best);
    }
}
//*/

template <class T_TYPE>
T_TYPE
AbstractHeap<T_TYPE>::extract_min ()
{
#if CHECK_ASSERTIONS
    assert(size() != 0);
#endif
    T_TYPE topElement = (*data)[0];
    swap ((*data)[0], (*data)[size()-1]);
    data->pop_back();
    min_heapify (0);
    return topElement;
}


template <class T_TYPE>
void
AbstractHeap<T_TYPE>::insert (T_TYPE element)
{
    int index = size();
    data->push_back(element);
    while (index > 0  &&  __less_then(element, (*data)[PMF_HEAP_PARENT(index)]))
    {
        (*data)[index] = (*data)[PMF_HEAP_PARENT(index)];
        index = PMF_HEAP_PARENT(index);
    }
    (*data)[index] = element;
}


template <class T_TYPE>
inline bool
AbstractHeap<T_TYPE>::__less_equal (T_TYPE el1, T_TYPE el2)
{
    return (! __less_then(el2, el1));
}

#undef PMF_HEAP_RIGHT
#undef PMF_HEAP_LEFT
#undef PMF_HEAP_PARENT

#endif // TEMPLATEHEAP_HPP_INCLUDED
