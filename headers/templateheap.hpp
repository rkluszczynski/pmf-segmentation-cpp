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
class TemplateHeap
{
    private :
        vector<T_TYPE> * data;

        void min_heapify (long);
        virtual bool less_then (T_TYPE, T_TYPE);

    public :
        TemplateHeap();
        ~TemplateHeap();

        inline long size()  { return data->size(); }
        inline bool empty() { return data->size() == 0; }

        T_TYPE extract_min ();
        void insert (T_TYPE);
};


template <class T_TYPE>
TemplateHeap<T_TYPE>::TemplateHeap ()
{
    data = new vector<T_TYPE> ();
	std::cout << "[ PMF HEAP ]  created" << std::endl;
};


template <class T_TYPE>
TemplateHeap<T_TYPE>::~TemplateHeap ()
{
    delete data;
	std::cout << "[ PMF HEAP ]  destroyed" << std::endl;
};


template <class T_TYPE>
void
TemplateHeap<T_TYPE>::min_heapify (long index)
{
    long l = PMF_HEAP_LEFT(index);
    long r = PMF_HEAP_RIGHT(index);
    long largest = index;

    //if (l < size()  &&  (*data)[l] > (*data)[largest])  largest = l;
    //if (r < size()  &&  (*data)[r] > (*data)[largest])  largest = r;
    if (l < size()  &&  less_then((*data)[largest], (*data)[l]))  largest = l;
    if (r < size()  &&  less_then((*data)[largest], (*data)[r]))  largest = r;
    if (largest != index)  {
        swap((*data)[index], (*data)[largest]);
        min_heapify(largest);
    }
}


template <class T_TYPE>
T_TYPE
TemplateHeap<T_TYPE>::extract_min ()
{
    assert(size() != 0);
    T_TYPE topElement = (*data)[0];
    swap ((*data)[0], (*data)[size()-1]);
    data->pop_back();
    min_heapify (0);
    return topElement;
}


template <class T_TYPE>
void
TemplateHeap<T_TYPE>::insert (T_TYPE element)
{
    int index = size();
    data->push_back(element);
    while (index > 0  &&  less_then((*data)[PMF_HEAP_PARENT(index)], element))
    {
        (*data)[index] = (*data)[PMF_HEAP_PARENT(index)];
        index = PMF_HEAP_PARENT(index);
    }
    (*data)[index] = element;
}

#undef PMF_HEAP_RIGHT
#undef PMF_HEAP_LEFT
#undef PMF_HEAP_PARENT

#endif // TEMPLATEHEAP_HPP_INCLUDED
