#ifndef TESTING_HPP_INCLUDED
#define TESTING_HPP_INCLUDED


template <class REAL>
bool
PMF<REAL> :: IsOldBirthBeforeOldDeathInEpsilonDistance ()
{

    return false;
}


    template <class REAL> class PointLexicographicalComparator
    {
        public:
            bool operator() (Point<REAL> * const & p1, Point<REAL> * const & p2) const
            {
                if (Geometry::IsZero(p1->x - p2->x)) return p1->y > p2->y;
                return p1->x > p2->x;
            }
    };

template <class REAL>
bool
PMF<REAL> :: TestPointsCoincidence ()
{
    PMFLog("[ TST ] : .begin()");
    bool wrong = false;

    Point<REAL> * pt, * prev = NULL;
    if (! cf->IsEmpty())
    {
        PointPriorityQueue ppq( cf->begin(), cf->end(), PointComparator<REAL>(nparams.GetAxisEpsilon()) );

        while (! ppq.empty())
        {
            pt = ppq.top();
            ppq.pop();

            REAL epsilon = nparams.GetAxisEpsilon();
            if (    prev
                and
                    Geometry::IsZero(pt->x - prev->x, epsilon)
                and
                    Geometry::IsZero(pt->y - prev->y, epsilon)
               )
            {
                wrong = true;
                cout << " [ TST ] :eq: " << prev << endl;
                cout << " [ TST ] :eq: " << pt << endl;
                cout.flush();
                break;
            }
            prev = pt;
        }
    }
    PMFLog("[ TST ] : .end(%d)", wrong);
    return !wrong;
}


template <class REAL>
bool
PMF<REAL> :: IsFreeEpsilonCoincidence (REAL xx, REAL yy)
{
    out.precision(20);
    out << "=>  checking  x = " << xx << endl;
    out << "=>  checking  y = " << yy << endl;

        FOREACH(it, *cf)
        {
            Point<REAL> * pt = *it;
            REAL epsilon = nparams.GetAxisEpsilon();
            if (Geometry::IsZero(pt->x - xx, epsilon)  and  Geometry::IsZero(pt->y - yy, epsilon))
            {
                out.precision(20);
                out << "[ BANG ] : x = " << xx << endl;
                out << "[ BANG ] : y = " << yy << endl;
                out << "[ BANG ] : " << pt << endl;
                return false;
            }
        }

    return true;
}


#endif // TESTING_HPP_INCLUDED
