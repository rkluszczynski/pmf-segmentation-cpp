#ifndef CONFIGURATIONLIST_HPP_INCLUDED
#define CONFIGURATIONLIST_HPP_INCLUDED

#include "point.hpp"
#include "templatelist.hpp"


template <class T_REAL>
class ConfigurationList : public TemplateList<pmf_point<T_REAL> >
{
    private :
        T_REAL fieldWidth, fieldHeight;

    public :
        ConfigurationList(T_REAL, T_REAL);

        T_REAL get_field_width()  { return  fieldWidth; }
        T_REAL get_field_height() { return fieldHeight; }

        void set_points_ids ();
		void save_configuration (std::ostream & out);
		void save_svg (std::ostream & out, double scale = 100.0, double strokeWidth = 0.15);
};


template <class T_REAL>
ConfigurationList<T_REAL>::ConfigurationList (T_REAL width, T_REAL height)
: TemplateList<pmf_point<T_REAL> >()
{
    fieldWidth = width;
    fieldHeight = height;
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
void ConfigurationList<T_REAL>::save_configuration (std::ostream & out)
{
    using std::endl;
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
    out << "<g style=\"stroke-width:" << strokeWidth << "; stroke:blue; fill:none;\">" << endl;
    while (iter) {
        pmf_point<T_REAL> * pt = iter->data;
        if (pt->n1) {
            out << "\t<line x1=\"" << scale*pt->x << "\" y1=\"" << scale*pt->y << "\" ";
            out << "x2=\"" << scale*pt->n1->x << "\" y2=\"" << scale*pt->n1->y << "\"/>" << endl;
        }
        if (pt->n2) {
            out << "\t<line x1=\"" << scale*pt->x << "\" y1=\"" << scale*pt->y << "\" ";
            out << "x2=\"" << scale*pt->n2->x << "\" y2=\"" << scale*pt->n2->y << "\"/>" << endl;
        }
        iter = iter->next;
    }
    out << "</g>" << endl << "</svg>" << endl;
}


#endif // CONFIGURATIONLIST_HPP_INCLUDED
