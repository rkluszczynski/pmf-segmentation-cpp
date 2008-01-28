#ifndef BLOCKLIST_HPP_INCLUDED
#define BLOCKLIST_HPP_INCLUDED

#include "point.hpp"
#include "templatelist.hpp"

template <class T_REAL>
class BlockList : public TemplateList<pmf_point<T_REAL> >
{
    public :
        void push_back (pmf_point<T_REAL> *);
        void print_points ();
        void pop_point (pmf_point<T_REAL> *);

		friend std::ostream & operator << (std::ostream & out, const BlockList<T_REAL> * list)
		{
			Element<pmf_point<T_REAL> > * iter = list->head;
			while (iter) {
                  out << " " << *iter->data;
                  iter = iter->next;
            }
			out << std::endl;
			return out;
		}
};


template <class T_REAL>
void BlockList<T_REAL>::pop_point (pmf_point<T_REAL> * pt)
{
    Element<pmf_point<T_REAL> > * pointer = pt->pointer;
    assert(pt);
    if (pointer == TemplateList<pmf_point<T_REAL> >::head) {
        TemplateList<pmf_point<T_REAL> >::head = pointer->next;
        if (pointer->next) {
            TemplateList<pmf_point<T_REAL> >::head->prev = NULL;
        }
        else {
            TemplateList<pmf_point<T_REAL> >::tail = NULL;
        }
    }
    else {
        if (pointer == TemplateList<pmf_point<T_REAL> >::tail)
            TemplateList<pmf_point<T_REAL> >::tail = pointer->prev;
        pointer->prev->next = pointer->next;
        if (pointer->next)
            pointer->next->prev = pointer->prev;
    }
    if (pointer->data) cout << " DEL : " << *pointer->data << endl;
    if (pointer->next) cout << "NEXT : " << *pointer->next->data << endl;
    if (pointer->prev) cout << "PREV : " << *pointer->prev->data << endl;
    delete pointer;
}


template <class T_REAL>
void BlockList<T_REAL>::print_points ()
{
    using std::cout;
	Element<pmf_point<T_REAL> > * iter = TemplateList<pmf_point<T_REAL> >::head;
	while (iter) {
        cout << " " << *iter->data;
        cout << "~";
        Element<pmf_point<T_REAL> > * tmp = iter->data->pointer;
        if (tmp->prev) {
            cout << tmp->prev->data->id << ".";
        }
        else {
            cout << "x.";
        }
        cout << (iter->data)->block;
        if (tmp->next) {
            cout << "." << tmp->next->data->id;
        }
        else {
            cout << ".x";
        }
        cout << " ";
        iter = iter->next;
    }
	cout << std::endl;
}



/**
 * Adding element at the end of list.
 **/
template <class T_REAL>
void BlockList<T_REAL>::push_back (pmf_point<T_REAL> * pt)
{
    TemplateList<pmf_point<T_REAL> >::push_back (pt);
    pt->pointer = TemplateList<pmf_point<T_REAL> >::tail;
}


#endif // BLOCKLIST_HPP_INCLUDED
