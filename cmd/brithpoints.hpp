#ifndef BRITHPOINTS_HPP_INCLUDED
#define BRITHPOINTS_HPP_INCLUDED

namespace pmf
{

    template <class REAL> class BirthSites
    {
        struct classcomp {
            bool operator() (const Point<REAL> * & lhs, const Point<REAL> * & rhs)
            const { return lhs->x < rhs->x;}
        };
        typedef Point<REAL> RealPoint;
        typedef set<RealPoint *, classcomp> PointsSet;

        private :
            PointsSet * pts;
            REAL fieldWidth, fieldHeight;
            map<long int, set<void *>::iterator> ids;

            void DestroyPoints();


        public :
            Configuration(REAL, REAL);
            ~Configuration();

            REAL GetFieldWidth()  { return  fieldWidth; }
            REAL GetFieldHeight() { return fieldHeight; }

            void Insert( Point<REAL> * pt );
            //inline void push_back ( pmf_point<T_REAL> *, BlocksLists<T_REAL> * );


            /*
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
        cf = new PointsSet ();
    }


    template <class REAL>
    Configuration<REAL>::~Configuration ()
    {
        DestroyPoints();
        //cerr << "[ CONF ] : destructing" << endl;
    }
}

#endif // BRITHPOINTS_HPP_INCLUDED
