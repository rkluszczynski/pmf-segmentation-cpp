#ifndef HEAP_HPP_INCLUDED
#define HEAP_HPP_INCLUDED

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

#define HEAP_PARENT(X)   (((X)-1)>>1)
#define HEAP_LEFT(X)     (((X)<<1) + 1)
#define HEAP_RIGHT(X)    (((X)<<1) + 2)


template <class T_TYPE> class Heap
{
    private :
        vector<T_TYPE> * data;

        inline long get_size ()  { return data->size(); }
        void min_heapify (long);

    public :
        Heap();
        ~Heap();

        T_TYPE heap_extract_min ();
        void heap_insert (T_TYPE);
        void build_min_heap ();
};


template <class T_TYPE>
Heap<T_TYPE>::Heap ()
{
    data = new vector<T_TYPE> ();
	std::cout << "[ HEAP ]  created" << std::endl;
};


template <class T_TYPE>
Heap<T_TYPE>::~Heap ()
{
    delete data;
	std::cout << "[ HEAP ]  destroyed" << std::endl;
};


template <class T_TYPE>
void Heap<T_TYPE>::min_heapify (long index)
{
    long l = HEAP_LEFT(index);
    long r = HEAP_RIGHT(index);
    long largest = index;

    if (l < get_size()  &&  (*data)[l] > (*data)[largest])  largest = l;
    if (r < get_size()  &&  (*data)[r] > (*data)[largest])  largest = r;
    if (largest != index)  {
        swap((*data)[index], (*data)[largest]);
        min_heapify(largest);
    }
}


template <class T_TYPE>
void Heap<T_TYPE>::build_min_heap ()
{
    for (int i = get_size()>>1; i >= 0; i--)  min_heapify (i);
}


template <class T_TYPE>
T_TYPE Heap<T_TYPE>::heap_extract_min ()
{
    if (get_size() == 0)  { cout << "[ HEAP ]  ERROR - empty"; }
    T_TYPE topElement = (*data)[0];
    swap ((*data)[0], (*data)[get_size()-1]);
    data->pop_back();
    min_heapify (0);
    return topElement;
}


template <class T_TYPE>
void Heap<T_TYPE>::heap_insert (T_TYPE element)
{
    int index = get_size();
    data->push_back(element);
    while (index > 0  &&  (*data)[HEAP_PARENT(index)] < element) {
        (*data)[index] = (*data)[HEAP_PARENT(index)];
        index = HEAP_PARENT(index);
    }
    (*data)[index] = element;
}


#undef HEAP_RIGHT
#undef HEAP_LEFT
#undef HEAP_PARENT

#endif // HEAP_HPP_INCLUDED
