// *** ADDED BY HEADER FIXUP ***
#include <wx/dcmemory.h>
#include <wx/filename.h>
#include <wx/strconv.h>
#include <wx/string.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>
// *** END ***
#ifndef CONFIGURATION_HPP_INCLUDED
#define CONFIGURATION_HPP_INCLUDED

#include "../headers/macros.hpp"
#include "../cmd/point.hpp"
#include "../include/statistics.h"


namespace pmf
{

    template <class REAL> class Configuration
    {
        private :
            REAL fieldWidth, fieldHeight;
            std::vector<Point<REAL> *> * pts;

            inline
            void DestroyPoints();


        public :
            typedef typename std::vector<Point<REAL> *>::iterator Iterator;

            Configuration(REAL, REAL);
            ~Configuration();

            Iterator begin() const { return pts->begin(); }
            Iterator end()   const { return pts->end(); }

            void PushBack ( Point<REAL> * pt ) { pts->push_back(pt); }
            void PopBack () { pts->pop_back(); }
            Point<REAL> * Back() { return pts->back(); }
            void SetPointsIDs ();

            inline REAL GetFieldWidth()  { return  fieldWidth; }
            inline REAL GetFieldHeight() { return fieldHeight; }
            inline long GetPointsCount() { return pts->size(); }
            inline bool IsEmpty() { return pts->empty(); }
            inline Point<REAL> * SeeLastPoint() { return pts->back(); }

            pmf::Statistics GetStatistics();

            void SaveToFile (const char *);
            void ShowConfiguration (std::ostream & out, int precision = 17);
            void PrintConfiguration (std::ostream & out, int precision = 17);
            void SaveConfigurationAsGGB (const char *);
            void SaveConfigurationAsGGB (std::ostream & out);

            void LoadConfiguration (std::istream & in);
            void DrawConfiguration (wxMemoryDC& dc, int scale);
            /*
            void clone_from ( ConfigurationList<T_REAL> * );
            void calculate_statistics (int *);
            void save_svg (std::ostream & out, double scale = 100.0, double strokeWidth = 0.15);
            void load_configuration (std::istream & in);
            bool remove_point_with_id (long);
            */

            void ClearNullPointers();

            inline void ClearPointsContainer() { pts->clear(); }
            //inline void ClearPointsContainer() { while(! pts->empty()) pts->pop_back(); }
    };


    template <class REAL>
    Configuration<REAL>::Configuration (REAL width, REAL height)
    {
        fieldWidth = width;
        fieldHeight = height;
        pts = new vector<Point<REAL> *> ();
    }


    template <class REAL>
    Configuration<REAL>::~Configuration ()
    {
        DestroyPoints();
        delete pts;
    }

    template <class REAL>
    void Configuration<REAL>::DestroyPoints ()  { FOREACH(it, *pts)  delete *it; }


    template <class REAL>
    void Configuration<REAL>::ClearNullPointers ()
    {
        std::vector<Point<REAL> *> * npts = new vector<Point<REAL> *> ();
        FOREACH(it, *pts)
            if (*it)  npts->push_back(*it);
        ClearPointsContainer();
        delete pts;
        pts = npts;
    }


    template <class REAL>
    pmf::Statistics Configuration<REAL>::GetStatistics ()
    {
        pmf::Statistics stats;
        FOREACH(it, *pts)
        {
            Point<REAL> * pt = *it;
            switch (pt->type)
            {
                case PT_BirthOnBorder :
                case PT_BirthInField  :
                                    stats.IncrementBirth();
                                    break;;
                case PT_Update        :
                                    stats.IncrementUpdate();
                                    break;;
                case PT_Collision     :
                case PT_DeathOnBorder :
                                    stats.IncrementDeath();
                                    break;;
                default :
                        assert("WRONG POINT TYPE DURING GETTING STATISTICS" && false);
            }
        }
        assert( stats.GetNumberOfBirths() + stats.GetNumberOfDeaths() + stats.GetNumberOfUpdates() == pts->size());
        return stats;
    }


    template <class REAL>
    void Configuration<REAL>::SetPointsIDs ()
    {
        //REP(i, pts->size())  pts->at(i)->id = i+1;
        int n = 0;
        FOREACH(it, (*pts)) {
            (*it)->oid = (*it)->id;
            (*it)->id = ++n;
        }
    }


    template <class REAL>
    void Configuration<REAL>::PrintConfiguration (std::ostream & out, int precision)
    {
        out << fieldWidth << " " << fieldHeight << endl;
        out << pts->size() << endl;

        out.precision(precision);
        FOREACH(it, (*pts))
        {
            Point<REAL> * pt = *it;

            out.width(4);   out << pt->id << " ";
            out.width(precision+5);  out << pt->x << " ";
            out.width(precision+5);  out << pt->y << " ";
            out.width(4);   out << ((pt->n1) ? pt->n1->id : 0) << " ";
            out.width(4);   out << ((pt->n2) ? pt->n2->id : 0) << " ";
            out.width(precision+5);  out << pt->l1 << " ";
            out.width(precision+5);  out << pt->l2 << " ";
            out.width(2);   out << pt->type << " ";
            out.width(3);   out << "-1 ";// pt->block << " ";
            out.width(4);   out << pt->oid << " ";
            out << endl;
        }
    }


    template <class REAL>
    void Configuration<REAL>::ShowConfiguration (std::ostream & out, int precision)
    {
        out << "[ CONFIGURATION ] :> ";
        out << fieldWidth << " x " << fieldHeight << "  [ " << pts->size() << " ]";
        out << endl;
        PrintConfiguration(out, precision);
        out << endl;
    }


    template <class REAL>
    void Configuration<REAL>::SaveToFile (const char * filename)
    {
        std::cerr << std::endl <<"[ SAVE ] : saving configuration to a file '" << filename << "'" << std::endl;
        ofstream fout(filename);
        SetPointsIDs();
        PrintConfiguration(fout);
        fout.close();
    }


    template <class REAL>
    void Configuration<REAL>::LoadConfiguration (std::istream & in)
    {
        if (! IsEmpty())  DestroyPoints();

        in >> fieldHeight;
        in >> fieldWidth;
        int ptNumber;
        in >> ptNumber;

        Point<REAL> ** ptTab = new Point<REAL> * [ptNumber+1];
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
            ptTab[i] = new Point<REAL>(x, y, NULL, NULL, l1, l2, id, PT_Unknown);
            if (sscanf(tmp, "%li %li %li", &tt, &bb, &oid) > 0)
                ptTab[i]->oid = oid;
        }

        for (int i = 1; i <= ptNumber; i++)
        {
            ptTab[i]->n1 = (firstIds[i] > 0) ? ptTab[firstIds[i]] : NULL;
            ptTab[i]->n2 = (secondIds[i] > 0) ? ptTab[secondIds[i]] : NULL;
        }
        for (int i = 1; i <= ptNumber; i++)  pts->push_back(ptTab[i]);

        delete[] secondIds;
        delete[] firstIds;
        delete[] ptTab;
    }


    template <class REAL>
    void Configuration<REAL>::DrawConfiguration (wxMemoryDC& dc, int scale)
    {
        FOREACH(it, *pts)
        {
            Point<REAL> * pt = *it;
            int x0 = int(pt->x * double(scale));
            int y0 = int(pt->y * double(scale));
            if (pt->n1)
            {
                int x1 = int(pt->n1->x * double(scale));
                int y1 = int(pt->n1->y * double(scale));
                dc.DrawLine(x0, y0, x1, y1);
            }
            if (pt->n2)
            {
                int x2 = int(pt->n2->x * double(scale));
                int y2 = int(pt->n2->y * double(scale));
                dc.DrawLine(x0, y0, x2, y2);
            }
        }
    }


    template <class REAL>
    void Configuration<REAL>::SaveConfigurationAsGGB (const char * filename)
    {
        wxCSConv conv_ascii(_T("ISO-8859-1"));
        wxString outputfile( filename, conv_ascii );
        //wxFFileOutputStream fout(wxString::Format(_T("%s"), filename));
        wxFFileOutputStream fout(outputfile);
        wxZipOutputStream zip(fout);
        wxTextOutputStream txt(zip);
        wxString sep(wxFileName::GetPathSeparator());
        /*
        zip.PutNextEntry(_T("entry1.txt"));
        txt << _T("Some text for entry1.txt\n");

        zip.PutNextEntry(_T("subdir") + sep + _T("entry2.txt"));
        txt << _T("Some text for subdir/entry2.txt\n");
        //*/
        zip.PutNextEntry(_T("geogebra.xml"));
        //*
        txt << _("<?xml version='1.0' encoding='utf-8'?>") << endl;
        txt << _("<geogebra format='3.2'>") << endl;
        txt << _("    <gui>") << endl;
		txt << _("        <show algebraView='true' spreadsheetView='false' auxiliaryObjects='true' algebraInput='true' cmdList='true'/>") << endl;
		txt << _("        <splitDivider loc='822' locVertical='400' loc2='221' locVertical2='300' horizontal='true'/>") << endl;
		txt << _("        <labelingStyle  val='3'/>") << endl;
		txt << _("        <font  size='12'/>") << endl;
        txt << _("    </gui>") << endl;
        txt << _("    <euclidianView>") << endl;
		txt << _("        <size  width='691' height='707'/>") << endl;
		txt << _("        <coordSystem xZero='49.20558651917091' yZero='641.537966311083' scale='597.2884022128751' yscale='597.2884022128751'/>") << endl;
		txt << _("        <evSettings axes='true' grid='true' gridIsBold='false' pointCapturing='0' pointStyle='0' rightAngleStyle='1' checkboxSize='13' gridType='0'/>") << endl;
		txt << _("        <bgColor r='255' g='255' b='255'/>") << endl;
		txt << _("        <axesColor r='0' g='0' b='0'/>") << endl;
		txt << _("        <gridColor r='192' g='192' b='192'/>") << endl;
		txt << _("        <lineStyle axes='1' grid='10'/>") << endl;
		txt << _("        <axis id='0' show='true' label='x' unitLabel='' tickStyle='0' showNumbers='true'/>") << endl;
		txt << _("        <axis id='1' show='true' label='y' unitLabel='' tickStyle='0' showNumbers='true'/>") << endl;
        txt << _("    </euclidianView>") << endl;
        txt << _("    <kernel>") << endl;
        txt << _("        <continuous val='true'/>") << endl;
		txt << _("        <decimals val='10'/>") << endl;
		txt << _("        <angleUnit val='degree'/>") << endl;
		txt << _("        <coordStyle val='0'/>") << endl;
        txt << _("    </kernel>") << endl;
        txt << _("    <construction title='' author='Rafal Kluszczynski' date=''>") << endl;
        //*
        FOREACH(it, (*pts))
        {
            Point<REAL> * pt = *it;

            txt << _("<element type='point' label='P") << wxString::Format(_("%li"), pt->id) << _("'>") << endl;
			txt << _("    <show object='true' label='true'/>") << endl;
			txt << _("    <objColor r='0' g='0' b='255' alpha='0.0'/>") << endl;
			txt << _("    <layer val='0'/>") << endl;
			txt << _("    <labelMode val='0'/>") << endl;
			txt << _("    <animation step='0.1' speed='1' type='0' playing='false'/>") << endl;
			txt << _("    <coords x='") << wxString::Format(_("%.20lf"), pt->x) << _("' y='") << wxString::Format(_("%.20lf"), pt->y) << _("' z='1.0'/>") << endl;
			txt << _("    <pointSize val='3'/>") << endl;
            txt << _("</element>") << endl;
        }

        FOREACH(it, (*pts))
        {
            Point<REAL> * pt = *it;
            /*
            txt << _("<command name='Segment'>") << endl;
            txt << _("    <input a0='P") << wxString::Format(_("%li"), pt->n2->id) << _("' a1='P") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
            txt << _("    <output a0='p") << wxString::Format(_("%li"), pt->n2->id) << _("p") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
            txt << _("</command>") << endl;
            txt << _("<command name='Segment'>") << endl;
            txt << _("    <input a0='P") << wxString::Format(_("%li"), pt->n1->id) << _("' a1='P") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
            txt << _("    <output a0='p") << wxString::Format(_("%li"), pt->n1->id) << _("p") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
            txt << _("</command>") << endl;
            //*/
            //*
            switch (pt->type)
            {
                case     PT_Collision :
                                txt << _("<command name='Segment'>") << endl;
                                txt << _("    <input a0='P") << wxString::Format(_("%li"), pt->n2->id) << _("' a1='P") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
                                txt << _("    <output a0='p") << wxString::Format(_("%li"), pt->n2->id) << _("p") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
                                txt << _("</command>") << endl;
                case PT_DeathOnBorder :
                case        PT_Update :
                                txt << _("<command name='Segment'>") << endl;
                                txt << _("    <input a0='P") << wxString::Format(_("%li"), pt->n1->id) << _("' a1='P") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
                                txt << _("    <output a0='p") << wxString::Format(_("%li"), pt->n1->id) << _("p") << wxString::Format(_("%li"), pt->id) << _("'/>") << endl;
                                txt << _("</command>") << endl;
                                break;;
                default :
                                ;
            }
            //*/
        }

        txt << _("    </construction>") << endl;
        txt << _("</geogebra>") << endl;

        zip.CloseEntry();
        zip.SetComment(_("PMF Configuration"));
        //*/
    }


    template <class REAL>
    void Configuration<REAL>::SaveConfigurationAsGGB (std::ostream & out)
    {
        //out << fieldWidth << " " << fieldHeight << endl;
        //out << pts->size() << endl;
        FOREACH(it, (*pts))
        {
            Point<REAL> * pt = *it;

            out.precision(20);
            out << "P" << pt->id << "=(" << pt->x << "," << pt->y << ")" << endl;
            switch (pt->type)
            {
                case     PT_Collision :
                                out << "p" << pt->n2->id << "p" << pt->id << "=Odcinek[P" << pt->n2->id << ",P" << pt->id << "]" << endl;
                case PT_DeathOnBorder :
                case        PT_Update :
                                out << "p" << pt->n1->id << "p" << pt->id << "=Odcinek[P" << pt->n1->id << ",P" << pt->id << "]" << endl;
                                break;;
                default :
                                ;
            }
        }
    }
/*
    template <class REAL>
    void Configuration<REAL>::Insert (Point<REAL> * pt)
    {
        pair<set<void *>::iterator, bool> setret = cf->insert(pt);
        assert(setret.second == true);
        pair<map<long int, set<void *> >::iterator, bool> mapret = ids.insert(pt->id, setret.first);
        assert(mapret.second == true);
    }
    //*/
}


#endif // CONFIGURATION_HPP_INCLUDED
