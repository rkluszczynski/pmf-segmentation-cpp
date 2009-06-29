#ifndef BIRTHSITES_HPP_INCLUDED
#define BIRTHSITES_HPP_INCLUDED

#include "../headers/macros.hpp"
#include "../cmd/point.hpp"

namespace pmf
{

    template <class REAL> class BirthSites
    {
        struct classcomp {
            bool operator() (Point<REAL> * const & lhs, Point<REAL> * const & rhs)
            const { return lhs->x < rhs->x;}
        };


        private :
            REAL sinL, cosL;

            set<Point<REAL> *, classcomp> * pts;
            map<const long, _Rb_tree_const_iterator<Point<REAL>*> > * ids;

            void OnInit();
            void DestroyPoints();


        public :
            BirthSites() : sinL(0.0), cosL(1.0)  { OnInit(); }
            BirthSites(REAL _s, REAL _c) : sinL(_s), cosL(_c)  { OnInit(); }
            ~BirthSites();

            void Insert( Point<REAL> * pt );
            Point<REAL> * Erase(long id);
            Point<REAL> * GetPoint(long id);

            Point<REAL> * GetFirst() { return *pts->begin(); }
            Point<REAL> * Erase(Point<REAL> * pt) { return Erase(pt->id); }

            long GetSize() { return pts->size(); }
            bool IsEmpty() { return pts->empty(); }

            /*
            void clone_from ( ConfigurationList<T_REAL> * );
            void calculate_statistics (int *);
            void set_points_ids ();
            void save_configuration (std::ostream & out);
            void save_svg (std::ostream & out, double scale = 100.0, double strokeWidth = 0.15);
            void load_configuration (std::istream & in);
            bool remove_point_with_id (long);
            */
            void PrintSites (std::ostream & out);
    };


    template <class REAL>
    void BirthSites<REAL>::OnInit ()
    {
        pts = new set<Point<REAL> *, classcomp> ();
        ids = new map<const long, _Rb_tree_const_iterator<Point<REAL>*> > ();
    }


    template <class REAL>
    BirthSites<REAL>::~BirthSites ()
    {
        DestroyPoints();
        delete pts;
        delete ids;
    }

    template <class REAL>
    void BirthSites<REAL>::DestroyPoints ()
    {
        FOREACH(it, *pts)  delete *it;
    }


    template <class REAL>
    void BirthSites<REAL>::Insert (Point<REAL> * pt)
    {
        pair<_Rb_tree_const_iterator<Point<REAL>*>, bool>  setret = pts->insert(pt);
        assert(setret.second == true);

        pair<long, _Rb_tree_const_iterator<Point<REAL>*> >  tmp(pt->id, setret.first);
        //pair<map<long, set<void *> >::iterator, bool> mapret =
        pair<_Rb_tree_iterator<pair<const long, _Rb_tree_const_iterator<Point<REAL>*> > >, bool>  mapret = ids->insert(tmp);
        assert(mapret.second == true);
    }


    template <class REAL>
    Point<REAL> * BirthSites<REAL>::Erase (long id)
    {
        //map<const long, set<void *>::iterator>::iterator mapit = ids->find(pt->id);
        ///map<const long, _Rb_tree_const_iterator<Point<double>*> >::iterator  mapit = ids->find(pt->id);
        std::_Rb_tree_iterator<std::pair<const long int, std::_Rb_tree_const_iterator<pmf::Point<REAL>*> > >  mapit = ids->find(id);
        assert(mapit != ids->end());

        Point<REAL> * erased = *mapit->second;
        pts->erase(mapit->second);
        ids->erase(mapit);
        return erased;
    }


    template <class REAL>
    Point<REAL> * BirthSites<REAL>::GetPoint (long id)
    {
        //map<const long, set<void *>::iterator>::iterator mapit = ids->find(pt->id);
        ///map<const long, _Rb_tree_const_iterator<Point<double>*> >::iterator  mapit = ids->find(pt->id);
        std::_Rb_tree_iterator<std::pair<const long int, std::_Rb_tree_const_iterator<pmf::Point<REAL>*> > >  mapit = ids->find(id);
        assert(mapit != ids->end());
        return *mapit->second;
    }


    template <class REAL>
    void BirthSites<REAL>::PrintSites (std::ostream & out)
    {
        out << "[ BIRTH SITES ] :> ";
        FOREACH(it, (*pts)) {
            out << " " << (*it);
        }
        out << endl;
    }

}


#endif // BIRTHSITES_HPP_INCLUDED
