#ifndef PREPARE_HPP_INCLUDED
#define PREPARE_HPP_INCLUDED


    template <class REAL> class PointComparator
    {
        public:
            bool operator() (Point<REAL> * const & p1, Point<REAL> * const & p2) const { return p1->x > p2->x; }
    };


    template <class REAL> class SegmentComparator
    {
        public:
            bool operator() (Segment<REAL> * const & s1, Segment<REAL> * const & s2) const { return s1->GetP()->x > s2->GetP()->x; }
    };


template <class REAL>
void
PMF<REAL> :: PrepareTheEvolution (REAL sinL, REAL cosL, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, REAL rotxx)
{
    typedef priority_queue<Point<REAL> *, std::vector<Point<REAL> *>, PointComparator<REAL> >         PointPriorityQueue;
    typedef priority_queue<Segment<REAL> *, std::vector<Segment<REAL> *>, SegmentComparator<REAL> > SegmentPriorityQueue;

    RotatePoints (sinL, cosL);

    PointPriorityQueue   ppq( cf->begin(), cf->end(), PointComparator<REAL>() );
    SegmentPriorityQueue spq( (SegmentComparator<REAL>()) );

    cf->ClearPointsContainer();
    while (ppq.top()->x < rotxx)
    {
        Point<REAL> * pt = ppq.top();
        cf->PushBack(pt);
        ppq.pop();
    }

    while(! ppq.empty())
    {
        Point<REAL> * pt = ppq.top();
        ppq.pop();
        out << " ~~~~~~    " << pt << endl;

        if (            pt->n1->x < rotxx)  spq.push( new Segment<REAL> (pt->n1, pt) );
        if (pt->n2  &&  pt->n2->x < rotxx)  spq.push( new Segment<REAL> (pt->n2, pt) );

        switch (pt->type)
        {
            case  PT_BirthInField :
                                    break;;
            case PT_BirthOnBorder :
                                    break;;
            case PT_DeathOnBorder :
                                    break;;
            case     PT_Collision :
                                    break;;
            case        PT_Update :
                                    break;;
            default:
                assert("WRONG POINT TYPE" && false);
        }
    }
    //*/
    return;
}


#endif // PREPARE_HPP_INCLUDED
