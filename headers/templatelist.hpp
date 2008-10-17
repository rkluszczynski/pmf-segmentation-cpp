#ifndef __TEMPLATELIST_HPP__
#define __TEMPLATELIST_HPP__

#include <cstdio>
#include <iostream>
#include <fstream>

#include "listelement.hpp"

#define null 0

template <class T_TYPE> class TemplateList
{
	protected :
		Element<T_TYPE> * head;
		Element<T_TYPE> * tail;
		long size;

		void clear() { while (! empty())  pop_front(); }

	public :
		typedef Element<T_TYPE> * NODE;

		TemplateList();
		~TemplateList();

        //void push_back(const T_TYPE *);
		//void push_in_order(const T_TYPE *);
        void push_back(T_TYPE *);
		void push_in_order(T_TYPE *);
		void pop_front();

		bool empty() { return(! head); }

		T_TYPE * front() { return(head->data); }
//		const T_TYPE * front() const { return(head->data); }

		Element<T_TYPE> * begin() { return(head); }
		Element<T_TYPE> * end() { return(null); }

//		const Element<T_TYPE> * begin() const { return(head); }
//		const Element<T_TYPE> * end() const { return(null); }

		Element<T_TYPE> * getHead() { return(head); }
		void setHead(Element<T_TYPE> * p) { head = p; }

		long get_size() { return(size); }
		void set_size(long val) { size = val; }

		friend std::ostream & operator << (std::ostream & out, const TemplateList<T_TYPE> * list)
		{
			out << std::endl << "[ LIST ] :> ";
			Element<T_TYPE> * iter = list->head;
			while (iter) {
                  out << " " << *iter->data;
                  iter = iter->next;
            }
			out << std::endl;
			return out;
		}
};


/**
 * Konstruktor klasy.
 **/
template <class T_TYPE>
TemplateList<T_TYPE>::TemplateList () : head(null), tail(null), size(0)
{
	;//std::cout << "[ LIST ]  created" << std::endl;
};


/**
 * Destruktor klasy.
 **/
template <class T_TYPE>
TemplateList<T_TYPE>::~TemplateList ()
{
	clear();
	//std::cerr << "[ LIST ]  destroyed" << std::endl;
}


/**
 * Wstawia element na koniec listy.
 **/
template <class T_TYPE>
//void TemplateList<T>::push_back (const T * data)
void TemplateList<T_TYPE>::push_back (T_TYPE * data)
{
	if (head) {
		Element<T_TYPE> * tmp = new Element<T_TYPE> (data, tail, null);
		tail->next = tmp;
		tail = tmp;
	}
	else {
		Element<T_TYPE> * tmp = new Element<T_TYPE> (data, null, null);
		head = tail = tmp;
	}
	++size;
}


/**
 * Wstawia element w kolejnosci rosnacej.
 **/
template <class T_TYPE>
//void TemplateList<T_TYPE>::push_in_order (const T_TYPE * data)
void TemplateList<T_TYPE>::push_in_order (T_TYPE * data)
{
	Element<T_TYPE> * iter = head;
	Element<T_TYPE> * prev = null;
	while (iter  &&  (*(iter->data) < *data))
	{
		prev = iter;
		iter = iter->next;
	}

	Element<T_TYPE> * tmp = new Element<T_TYPE> (data, prev, iter);
	if (iter == head)  head = tmp;
	else prev->next = tmp;
	++size;
}


/**
 * Usuwa pierwszy element z listy.
 **/
template <class T_TYPE>
void TemplateList<T_TYPE>::pop_front ()
{
	if (head) {
		Element<T_TYPE> * tmp = head;
		head = head->next;
		delete tmp;
	}
	--size;
}


#undef null

#endif /* __TEMPLATELIST_HPP__ */
