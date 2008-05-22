#ifndef CONFIGURATIONLIST_HPP_INCLUDED
#define CONFIGURATIONLIST_HPP_INCLUDED

#include "point.hpp"
#include "templatelist.hpp"


template <class T_REAL>
class ConfigurationList : public TemplateList<pmf_point<T_REAL> >
{
    private :
        T_REAL fieldWidth, fieldHeight;

        void destroy();


    public :
        ConfigurationList(T_REAL, T_REAL);

        T_REAL get_field_width()  { return  fieldWidth; }
        T_REAL get_field_height() { return fieldHeight; }

        void set_points_ids ();

		void save_configuration (std::ostream & out);
		void save_svg (std::ostream & out, double scale = 100.0, double strokeWidth = 0.15);

		void load_configuration (std::istream & in);
};


template <class T_REAL>
ConfigurationList<T_REAL>::ConfigurationList (T_REAL width, T_REAL height)
: TemplateList<pmf_point<T_REAL> >()
{
    fieldWidth = width;
    fieldHeight = height;
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
        long id;
        double x, y;
        char tmp[128];

        in >> id;
        in >> x;
        in >> y;
        in >> firstIds[id];
        in >> secondIds[id];
        in.getline(tmp, 128);
#ifdef CHECK_ASSERTIONS
        assert(i == id);
#endif
        ptTab[i] = new pmf_point<T_REAL>(x, y, NULL, NULL, 0.0, 0.0, id, PT_UNKNOWN);
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

    // cornsilk, floralwhite, oldlace, papayawhip, whitesmoke
    out << "<g style=\"stroke:none; fill:cornsilk;\">" << endl;
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
