#ifndef CONFIGURATION_HPP_INCLUDED
#define CONFIGURATION_HPP_INCLUDED

#include "..\headers\macros.hpp"
#include "..\cmd\point.hpp"

namespace pmf
{

    template <class REAL> class Configuration
    {
        private :
            struct classcomp {
                bool operator() (const int& lhs, const int& rhs) const
                {return lhs<rhs;}
            };

            set<Point<REAL> *, classcomp>  * cf;
            REAL fieldWidth, fieldHeight;

            void DestroyPoints();


        public :

            Configuration(REAL, REAL);
            ~Configuration();

            REAL GetFieldWidth()  { return  fieldWidth; }
            REAL GetFieldHeight() { return fieldHeight; }

            /*
            //inline void push_back ( pmf_point<T_REAL> *, BlocksLists<T_REAL> * );
            void clone_from ( ConfigurationList<T_REAL> * );

            void calculate_statistics (int *);

            void set_points_ids ();

            void save_configuration (std::ostream & out);
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
        cf = new set<Point<REAL> *, classcomp> ();
    }


    template <class REAL>
    Configuration<REAL>::~Configuration ()
    {
        DestroyPoints();
        //cerr << "[ CONF ] : destructing" << endl;
    }

    template <class REAL>
    void Configuration<REAL>::DestroyPoints ()
    {
        FOREACH(it, *cf)  delete *it;
    }


}


#endif // CONFIGURATION_HPP_INCLUDED
