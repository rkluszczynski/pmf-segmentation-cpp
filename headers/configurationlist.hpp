#ifndef CONFIGURATIONLIST_HPP_INCLUDED
#define CONFIGURATIONLIST_HPP_INCLUDED

#include "templatelist.hpp"


template <class T_REAL>
class ConfigurationList : public TemplateList<pmf_point<T_REAL> >
{
    public :
        void set_points_ids ();
		void save_configuration (std::ostream & out, double fieldHeight , double fieldWidth);
};


template <class T_REAL>
void ConfigurationList<T_REAL>::set_points_ids ()
{
    Element<pmf_point<T_REAL> > * iter = TemplateList<pmf_point<T_REAL> >::head;
    int newID = 0;
    while (iter) {
        iter->data->oid = iter->data->id;
        (* iter->data).id = (++newID);
        iter = iter->next;
    }
}


template <class T_REAL>
void ConfigurationList<T_REAL>::save_configuration (std::ostream & out, double fieldHeight , double fieldWidth)
{
    Element<pmf_point<T_REAL> > * iter = TemplateList<pmf_point<T_REAL> >::head;
    out << fieldHeight << " " << fieldWidth << endl;
    out << TemplateList<pmf_point<T_REAL> >::size << endl;
    while (iter) {
        pmf_point<T_REAL> * pt = iter->data;
        out << pt->id << " ";
        out << pt->x << " " << pt->y << " ";
        //out << pt->l1 << " " << pt->l2 << " ";
        out << ((pt->n1) ? pt->n1->id : 0) << " ";
        out << ((pt->n2) ? pt->n2->id : 0) << " ";
        out << pt->type << " ";
        out << pt->block << " ";
        out << pt->oid << " ";
        out << endl;
        iter = iter->next;
    }
}


#endif // CONFIGURATIONLIST_HPP_INCLUDED
