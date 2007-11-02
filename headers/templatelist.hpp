#ifndef __TEMPLATELIST_HPP__
#define __TEMPLATELIST_HPP__

#include <cstdio>
#include <iostream>

#include "listelement.hpp"

#define null 0

template <class T> class TemplateList
{
	private :
		Element<T> * head;
		Element<T> * tail;
		long size;

		void clear() { while (! empty())  pop_front(); }

	public :
		typedef Element<T> * NODE;

		TemplateList();
		~TemplateList();

		void push_back(const T *);
		void push_in_order(const T *);
		void pop_front();

		bool empty() { return(! head); }

		T& front() { return(head->data); }
		const T& front() const { return(head->data); }

		Element<T> * begin() { return(head); }
		Element<T> * end() { return(null); }

		const Element<T> * begin() const { return(head); }
		const Element<T> * end() const { return(null); }

		Element<T> * getHead() { return(head); }
		void setHead(Element<T> * p) { head = p; }

		long getSize() { return(size); }
		void setSize(long val) { size = val; }

		friend std::ostream & operator << (std::ostream & out, const TemplateList<T> * list)
		{
			out << std::endl << "[ LIST ] :> ";
			Element<T> * iter = list->head;
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
template <class T>
TemplateList<T>::TemplateList () : head(null), tail(null), size(0)
{
	std::cout << "[ LIST ]  created" << std::endl;
};


/**
 * Destruktor klasy.
 **/
template <class T>
TemplateList<T>::~TemplateList ()
{
	clear();
	std::cerr << "[ LIST ]  destroyed" << std::endl;
}


/**
 * Wstawia element na koniec listy.
 **/
template <class T>
void TemplateList<T>::push_back (const T * data)
{
	if (head) {
		Element<T> * tmp = new Element<T> (data, tail, null);
		tail->next = tmp;
		tail = tmp;
	}
	else {
		Element<T> * tmp = new Element<T> (data, null, null);
		head = tail = tmp;
	}
	++size;
}


/**
 * Wstawia element w kolejnosci rosnacej.
 **/
template <class T>
void TemplateList<T>::push_in_order (const T * data)
{
	Element<T> * iter = head;
	Element<T> * prev = null;
	while (iter  &&  (*(iter->data) < *data))
	{
		prev = iter;
		iter = iter->next;
	}

	Element<T> * tmp = new Element<T> (data, prev, iter);
	if (iter == head)  head = tmp;
	else prev->next = tmp;
	++size;
}


/**
 * Usuwa pierwszy element z listy.
 **/
template <class T>
void TemplateList<T>::pop_front ()
{
	if (head) {
		Element<T> * tmp = head;
		head = head->next;
		delete tmp;
	}
	--size;
}

#undef null

#endif /* __TEMPLATELIST_HPP__ */
