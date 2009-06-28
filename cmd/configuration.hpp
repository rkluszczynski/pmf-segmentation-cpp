#ifndef CONFIGURATION_HPP_INCLUDED
#define CONFIGURATION_HPP_INCLUDED

#include "../headers/macros.hpp"
#include "../cmd/point.hpp"

namespace pmf
{

    template <class REAL> class Configuration
    {
        private :
            REAL fieldWidth, fieldHeight;
            vector<Point<REAL> *> * pts;

            void DestroyPoints();


        public :
            Configuration(REAL, REAL);
            ~Configuration();

            void PushBack( Point<REAL> * pt ) { pts->push_back(pt); }
            //inline void push_back ( pmf_point<T_REAL> *, BlocksLists<T_REAL> * );
            void SetPointsIds ();

            inline REAL GetFieldWidth()  { return  fieldWidth; }
            inline REAL GetFieldHeight() { return fieldHeight; }
            inline bool IsEmpty() { return pts->empty(); }
            inline Point<REAL> * SeeLastPoint() { return pts->back(); }

            void SaveToFile (const char *);
            void ShowConfiguration (std::ostream & out);
            void PrintConfiguration (std::ostream & out);

            /*
            void clone_from ( ConfigurationList<T_REAL> * );
            void calculate_statistics (int *);
            void save_svg (std::ostream & out, double scale = 100.0, double strokeWidth = 0.15);
            void load_configuration (std::istream & in);
            bool remove_point_with_id (long);
            */
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
        //cerr << "[ CONF ] : destructing" << endl;
    }

    template <class REAL>
    void Configuration<REAL>::DestroyPoints ()
    {
        FOREACH(it, *pts)  delete *it;
    }


    template <class REAL>
    void Configuration<REAL>::SetPointsIds ()
    {
        //REP(i, pts->size())  pts->at(i)->id = i+1;
        int n = 0;
        FOREACH(it, (*pts)) {
            (*it)->oid = (*it)->id;
            (*it)->id = ++n;
        }
    }


    template <class REAL>
    void Configuration<REAL>::PrintConfiguration (std::ostream & out)
    {
        out << fieldWidth << " " << fieldHeight << endl;
        out << pts->size() << endl;
        FOREACH(it, (*pts))
        {
            Point<REAL> * pt = *it;

            out.precision(7);
            out.width(4);   out << pt->id << " ";
            out.width(11);  out << pt->x << " ";
            out.width(11);  out << pt->y << " ";
            out.width(4);   out << ((pt->n1) ? pt->n1->id : 0) << " ";
            out.width(4);   out << ((pt->n2) ? pt->n2->id : 0) << " ";
            out.width(11);  out << pt->l1 << " ";
            out.width(11);  out << pt->l2 << " ";
            out.width(2);   out << pt->type << " ";
            out.width(3);   out << "-1 ";// pt->block << " ";
            out.width(4);   out << pt->oid << " ";
            out << endl;
        }
    }


    template <class REAL>
    void Configuration<REAL>::ShowConfiguration (std::ostream & out)
    {
        out << "[ CONFIGURATION ] :> ";
        out << fieldWidth << " x " << fieldHeight << "  [ " << pts->size() << " ]";
        out << endl;
        PrintConfiguration(out);
        out << endl;
    }


    template <class REAL>
    void Configuration<REAL>::SaveToFile (const char * filename)
    {
        std::cerr << std::endl <<"[ SAVE ] : saving configuration to a file '" << filename << "'" << std::endl;
        ofstream fout(filename);
        SetPointsIds();
        PrintConfiguration(fout);
        fout.close();
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
