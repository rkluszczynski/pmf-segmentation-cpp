#ifndef MODIFYLISTS_HPP_INCLUDED
#define MODIFYLISTS_HPP_INCLUDED

#include "point.hpp"

#define REAL double
int pmf_which_neighbor_is_id(pmf_point<REAL> * pt, long id)
{
    if (pt->n1 != NULL  &&  pt->n1->id == id) return 1;
    if (pt->n2 != NULL  &&  pt->n2->id == id) return 2;
    return 0;
}


#include <stack>
pmf_point<REAL> * pmf_delete_path()
{
    std::stack<long int> st;

    return NULL;
}
#undef REAL

#endif // MODIFYLISTS_HPP_INCLUDED
