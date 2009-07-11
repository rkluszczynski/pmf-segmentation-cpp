#ifndef PREPARE_HPP_INCLUDED
#define PREPARE_HPP_INCLUDED


    template <class REAL> class PointComparator
    {
        public:
            bool operator() (Point<REAL> * const & p1, Point<REAL> * const & p2) const { return p1->x > p2->x; }
    };


template <class REAL>
void
PMF<REAL> :: PrepareEvolution (REAL sinL, REAL cosL, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, REAL rotxx)
{
    RotatePoints (sinL, cosL);

    priority_queue<Point<REAL> *, std::vector<Point<REAL> *>, PointComparator<REAL> >  pq( cf->begin(), cf->end(), PointComparator<REAL>() );

    cf->ClearPointsContainer();
    while (pq.top()->x < rotxx)
    {
        Point<REAL> * pt = pq.top();
        cf->PushBack(pt);

        //if (pt->n1->x > rotxx)

        pq.pop();
    }

    while(! pq.empty())
    {
        Point<REAL> * pt = pq.top();
        pmf::out << " ~~~~~~    " << pt << endl;

        pq.pop();
    }
    //*/
    return;
}


#endif // PREPARE_HPP_INCLUDED
