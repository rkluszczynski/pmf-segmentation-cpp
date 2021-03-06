#ifndef CONFIGURATIONLIST_HPP_INCLUDED
#define CONFIGURATIONLIST_HPP_INCLUDED

#include <assert.h>

#include "point.hpp"
#include "templatelist.hpp"
#include "blockslists.hpp"


template <class T_REAL>
class ConfigurationList : public TemplateList<pmf_point<T_REAL> >
{
    private :
        T_REAL fieldWidth, fieldHeight;

        void destroy();


    public :
        ConfigurationList(T_REAL, T_REAL);
        ~ConfigurationList();

        //inline void push_back ( pmf_point<T_REAL> *, BlocksLists<T_REAL> * );
        void clone_from ( ConfigurationList<T_REAL> * );

        void calculate_statistics (int *);

        T_REAL get_field_width()  { return  fieldWidth; }
        T_REAL get_field_height() { return fieldHeight; }

        void set_points_ids ();

		void save_configuration (std::ostream & out);
		void save_svg (std::ostream & out, double scale = 100.0, double strokeWidth = 0.15);

		void load_configuration (std::istream & in);

		bool remove_point_with_id (long);
};


template <class T_REAL>
ConfigurationList<T_REAL>::ConfigurationList (T_REAL width, T_REAL height)
: TemplateList<pmf_point<T_REAL> >()
{
    fieldWidth = width;
    fieldHeight = height;
}


template <class T_REAL>
ConfigurationList<T_REAL>::~ConfigurationList ()
{
    destroy();
    //cerr << "[ CONF ] : destructing" << endl;
}


template <class T_REAL>
bool ConfigurationList<T_REAL>::remove_point_with_id (long ptId)
{
    Element<pmf_point<T_REAL> > * iter = TemplateList<pmf_point<T_REAL> >::head;
    Element<pmf_point<T_REAL> > * pop = iter;
    while (iter) {
        if (iter->data->id == ptId) {
            //if (blocksLists)  blocksLists->pop(iter->data);
            delete iter->data;
            if (iter == TemplateList<pmf_point<T_REAL> >::head) {
                TemplateList<pmf_point<T_REAL> >::head = iter->next;
                delete(iter);
                pop = TemplateList<pmf_point<T_REAL> >::head;
                iter = TemplateList<pmf_point<T_REAL> >::head;
            }
            else {
                pop->next = iter->next;
                delete(iter);
                iter = pop->next;
            }
            TemplateList<pmf_point<T_REAL> >::size--;
            return true;
        }
        else {
            pop = iter;
            iter = iter->next;
        }
    }
    return false;
}


template <class T_REAL>
void ConfigurationList<T_REAL>::calculate_statistics ( int * stats )
{
    for (int i = 0; i < PT_MAX_NUMBER; i++)  stats[i] = 0;

    Element<pmf_point<T_REAL> > * iter = TemplateList<pmf_point<T_REAL> >::head;
    while (iter) {
        stats[iter->data->type]++;
        iter = iter->next;
    }
}

/*
template <class T_REAL>
inline
void ConfigurationList<T_REAL>::push_back (pmf_point<T_REAL> * pt, BlocksLists<T_REAL> * blocks)
{
    if (blocks) {
        pt->block = blocks->determine_point_block(pt);
        blocks->push(pt);
    }
    TemplateList<pmf_point<T_REAL> >::push_back (pt);
}
*/

template <class T_REAL>
void ConfigurationList<T_REAL>::clone_from ( ConfigurationList<T_REAL> * originalPMF )
{
    if (originalPMF == NULL) return;
    if (TemplateList<pmf_point<T_REAL> >::get_size() > 0)  destroy();

    TemplateList<pmf_point<T_REAL> >::head = NULL;
    TemplateList<pmf_point<T_REAL> >::tail = NULL;
    TemplateList<pmf_point<T_REAL> >::size = 0;

    originalPMF->set_points_ids();
    fieldHeight = originalPMF->get_field_height();
    fieldWidth  = originalPMF->get_field_width();
    int  amount = originalPMF->get_size();

    pmf_point<T_REAL> ** ptTab = new pmf_point<T_REAL> * [amount+1];
    long *  firstIds = new long[amount+1];
    long * secondIds = new long[amount+1];

    Element<pmf_point<T_REAL> > * iter = originalPMF->getHead();
    while (iter) {
        pmf_point<T_REAL> * opt = iter->data;
        firstIds[opt->id]  = ((opt->n1) ? opt->n1->id : 0);
        secondIds[opt->id] = ((opt->n2) ? opt->n2->id : 0);
        //ptTab[opt->id] = new pmf_point<T_REAL>(opt->x, opt->y, NULL, NULL, opt->l1, opt->l2, opt->id, PT_UNKNOWN);
        ptTab[opt->id] = new pmf_point<T_REAL>(opt->x, opt->y, NULL, NULL, opt->l1, opt->l2, opt->id, opt->type);
        iter = iter->next;
    }

    for (int i = 1; i <= amount; i++)
    {
        ptTab[i]->n1 = (firstIds[i] > 0) ? ptTab[firstIds[i]] : NULL;
        ptTab[i]->n2 = (secondIds[i] > 0) ? ptTab[secondIds[i]] : NULL;
    }
    for (int i = 1; i <= amount; i++)
        TemplateList<pmf_point<T_REAL> >::push_back(ptTab[i]);

    delete[] secondIds;
    delete[] firstIds;
    delete[] ptTab;
}


template <class T_REAL>
void ConfigurationList<T_REAL>::destroy ()
{
    if (TemplateList<pmf_point<T_REAL> >::get_size() <= 0)  return;

    while (! TemplateList<pmf_point<T_REAL> >::empty())
    {
        delete TemplateList<pmf_point<T_REAL> >::front();
        TemplateList<pmf_point<T_REAL> >::pop_front();
    }
    TemplateList<pmf_point<T_REAL> >::head = NULL;
    TemplateList<pmf_point<T_REAL> >::tail = NULL;
    TemplateList<pmf_point<T_REAL> >::size = 0;
}


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
void ConfigurationList<T_REAL>::load_configuration (std::istream & in)
{
    if (TemplateList<pmf_point<T_REAL> >::get_size() > 0)  destroy();

    in >> fieldHeight;
    in >> fieldWidth;
    int ptNumber;
    in >> ptNumber;

    pmf_point<T_REAL> ** ptTab = new pmf_point<T_REAL> * [ptNumber+1];
    long * firstIds = new long[ptNumber+1];
    long * secondIds = new long[ptNumber+1];

    for (int i = 1; i <= ptNumber; i++)
    {
        long id, tt, bb, oid;
        double x, y, l1, l2;
        char tmp[128];

        in >> id;
        in >> x;
        in >> y;
        in >> firstIds[id];
        in >> secondIds[id];
        in >> l1;
        in >> l2;
        in.getline(tmp, 128);
#ifdef CHECK_ASSERTIONS
        assert(i == id);
#endif
        ptTab[i] = new pmf_point<T_REAL>(x, y, NULL, NULL, l1, l2, id, PT_UNKNOWN);
        if (sscanf(tmp, "%li %li %li", &tt, &bb, &oid) > 0)
            ptTab[i]->oid = oid;
    }

    for (int i = 1; i <= ptNumber; i++)
    {
        ptTab[i]->n1 = (firstIds[i] > 0) ? ptTab[firstIds[i]] : NULL;
        ptTab[i]->n2 = (secondIds[i] > 0) ? ptTab[secondIds[i]] : NULL;
    }

    for (int i = 1; i <= ptNumber; i++)
        TemplateList<pmf_point<T_REAL> >::push_back(ptTab[i]);

    delete[] secondIds;
    delete[] firstIds;
    delete[] ptTab;
}


template <class T_REAL>
void ConfigurationList<T_REAL>::save_configuration (std::ostream & out)
{
    using std::endl;
    Element<pmf_point<T_REAL> > * iter = TemplateList<pmf_point<T_REAL> >::head;
    out << fieldHeight << " " << fieldWidth << endl;
    out << TemplateList<pmf_point<T_REAL> >::size << endl;
    while (iter) {
        pmf_point<T_REAL> * pt = iter->data;
        out.precision(7);
        out.width(4);   out << pt->id << " ";
        out.width(11);  out << pt->x << " ";
        out.width(11);  out << pt->y << " ";
        out.width(4);   out << ((pt->n1) ? pt->n1->id : 0) << " ";
        out.width(4);   out << ((pt->n2) ? pt->n2->id : 0) << " ";
        out.width(11);  out << pt->l1 << " ";
        out.width(11);  out << pt->l2 << " ";
        out.width(2);   out << pt->type << " ";
        out.width(3);   out << pt->block << " ";
        out.width(4);   out << pt->oid << " ";
        out << endl;
        iter = iter->next;
    }
}


template <class T_REAL>
void ConfigurationList<T_REAL>::save_svg (std::ostream & out, double scale, double strokeWidth)
{
    using std::endl;
    Element<pmf_point<T_REAL> > * iter = TemplateList<pmf_point<T_REAL> >::head;

    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
    out << "<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.0//EN' 'http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd'>" << endl;
    out << "<svg xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns=\"http://www.w3.org/2000/svg\" ";
    out << "width=\"" << scale*fieldWidth << "\" height=\"" << scale*fieldHeight << "\">" << endl;
    out << endl;

    // cornsilk, floralwhite, oldlace, papayawhip, whitesmoke
    out << "<g style=\"stroke-width:" << strokeWidth*10.0 << "; stroke:black; fill:cornsilk;\">" << endl;
    out << "\t<path d=\"M0,0 L" << scale*fieldWidth << ",0 " << scale*fieldWidth << ",";
    out << scale*fieldHeight << " 0," << scale*fieldHeight << "z\"/>" << endl;
    out << "</g>" << endl;

    out << "<g style=\"stroke-width:" << strokeWidth << "; stroke:blue; fill:none;\">" << endl;
    while (iter) {
        pmf_point<T_REAL> * pt = iter->data;
        if (pt->n1 && pt->n2) {
            double hx1 = 0.5 * (pt->n1->x + pt->x);
            double hy1 = 0.5 * (pt->n1->y + pt->y);
            double hx2 = 0.5 * (pt->n2->x + pt->x);
            double hy2 = 0.5 * (pt->n2->y + pt->y);

            out << "\t<path d=\"M" << scale*hx1 << "," << scale*hy1;
            out << " L" << scale*pt->x << "," << scale*pt->y;
            out << " " << scale*hx2 << "," << scale*hy2;
            out << "\"/>" << endl;
        }
        else if (pt->n1) {
            double hx1 = 0.5 * (pt->n1->x + pt->x);
            double hy1 = 0.5 * (pt->n1->y + pt->y);

            out << "\t<line x1=\"" << scale*pt->x << "\" y1=\"" << scale*pt->y << "\" ";
            out << "x2=\"" << scale*hx1 << "\" y2=\"" << scale*hy1 << "\"/>" << endl;
        }
        else if (pt->n2) {
            double hx2 = 0.5 * (pt->n2->x + pt->x);
            double hy2 = 0.5 * (pt->n2->y + pt->y);

            out << "\t<line x1=\"" << scale*pt->x << "\" y1=\"" << scale*pt->y << "\" ";
            out << "x2=\"" << scale*hx2 << "\" y2=\"" << scale*hy2 << "\"/>" << endl;
        }
        iter = iter->next;
    }
    out << "</g>" << endl;

    out << "<g style=\"stroke-width:" << 2.0*strokeWidth << "; stroke:black; fill:none;\">" << endl;
    out << "\t<path d=\"M0,0 L" << scale*fieldWidth << ",0 " << scale*fieldWidth << ",";
    out << scale*fieldHeight << " 0," << scale*fieldHeight << "z\"/>" << endl;
    out << "</g>" << endl;

    out << "</svg>" << endl;
}


#endif // CONFIGURATIONLIST_HPP_INCLUDED
