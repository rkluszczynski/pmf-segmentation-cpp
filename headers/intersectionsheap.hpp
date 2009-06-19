#ifndef INTERSECTIONSHEAP_HPP_INCLUDED
#define INTERSECTIONSHEAP_HPP_INCLUDED

#include <cmath>
#include "crosselement.hpp"
#include "abstractheap.hpp"

#define X_ROTATED(XX,YY,SSIN,CCOS) ((XX)*(CCOS)-(YY)*(SSIN))

template <class T_REAL>
class IntersectionsHeap : public AbstractHeap<CrosspointElement<T_REAL> *>
{
    private :
        double sinL, cosL;

        virtual inline bool __less_then (CrosspointElement<T_REAL> * cpt1, CrosspointElement<T_REAL> * cpt2)
        {
            return X_ROTATED(cpt1->pt->x, cpt1->pt->y, sinL, cosL) < X_ROTATED(cpt2->pt->x, cpt2->pt->y, sinL, cosL);
        };

    public :
        IntersectionsHeap() : sinL(0.0), cosL(1.0) {};
        IntersectionsHeap(double ssinL, double ccosL) : sinL(ssinL), cosL(ccosL) {};
        ~IntersectionsHeap();

        inline pmf_point<T_REAL> * top() { return AbstractHeap<CrosspointElement<T_REAL> *>::top()->pt; }
        inline long topP1() { return AbstractHeap<CrosspointElement<T_REAL> *>::top()->p1; }
        inline long topP2() { return AbstractHeap<CrosspointElement<T_REAL> *>::top()->p2; }

        pmf_point<T_REAL> * extract_min (long & , long & );
        void insert (pmf_point<T_REAL> *, long , long );

        bool remove_intersections_with_id(long, BlocksLists<T_REAL> *);

		friend std::ostream & operator << (std::ostream & out, IntersectionsHeap<T_REAL> * iHeap)
		{
			out << std::endl << "[ IHEAP ] :> ";
			for (int i = 0; i < iHeap->size(); i++)
                out << " " << *(iHeap->get(i)->pt) << "~" << iHeap->get(i)->p1 << "+" << iHeap->get(i)->p2;
            out << std::endl;
			return out;
		}
};


template <class T_REAL>
IntersectionsHeap<T_REAL>::~IntersectionsHeap ()
{
    if (AbstractHeap<CrosspointElement<T_REAL> *>::size() <= 0)  return;

    while (! AbstractHeap<CrosspointElement<T_REAL> *>::empty())
    {
        delete AbstractHeap<CrosspointElement<T_REAL> *>::extract_min()->pt;
        delete AbstractHeap<CrosspointElement<T_REAL> *>::extract_min();
    }
    //cerr << "[ IHEAP ] : destructing" << endl;
}


template <class T_REAL>
bool
IntersectionsHeap<T_REAL>::remove_intersections_with_id (long ptId, BlocksLists<T_REAL> * blocks = NULL)
{
    bool deleted = false;
    for (int i = 0; i < AbstractHeap<CrosspointElement<T_REAL> *>::size(); i++)
    {
        CrosspointElement<T_REAL> * cEl = AbstractHeap<CrosspointElement<T_REAL> *>::get(i);
        if (cEl->p1 == ptId  ||  cEl->p2 == ptId) {
            std::swap ((*AbstractHeap<CrosspointElement<T_REAL> *>::data)[i], (*AbstractHeap<CrosspointElement<T_REAL> *>::data)[AbstractHeap<CrosspointElement<T_REAL> *>::size()-1]);
            AbstractHeap<CrosspointElement<T_REAL> *>::data->pop_back();
            if (blocks)  blocks->pop(cEl->pt);
            AbstractHeap<CrosspointElement<T_REAL> *>::min_heapify (i);
            --i;
            delete cEl->pt;
            deleted = true;
        }
    }
    return deleted;
}


template <class T_REAL>
void
IntersectionsHeap<T_REAL>::insert (pmf_point<T_REAL> * pt, long id1, long id2)
{
    CrosspointElement<T_REAL> * cpt = new CrosspointElement<T_REAL> (pt, id1, id2);
    AbstractHeap<CrosspointElement<T_REAL> *>::insert (cpt);
}


template <class T_REAL>
pmf_point<T_REAL> *
IntersectionsHeap<T_REAL>::extract_min (long & id1, long & id2)
{
    CrosspointElement<T_REAL> * cpt = AbstractHeap<CrosspointElement<T_REAL> *>::extract_min ();
    id1 = cpt->p1;
    id2 = cpt->p2;
    pmf_point<T_REAL> * pt = cpt->pt;
    delete cpt;
    return pt;
}

#undef X_ROTATED

#endif // INTERSECTIONSHEAP_HPP_INCLUDED
