#ifndef PREPARE_HPP_INCLUDED
#define PREPARE_HPP_INCLUDED





    template <class REAL> class SegmentComparator
    {
        public:
            bool operator() (Segment<REAL> * const & s1, Segment<REAL> * const & s2) const { return s1->GetP()->x > s2->GetP()->x; }
    };




template <class REAL>
void
PMF<REAL> :: PrepareTheEvolution (EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, PointPriorityQueue & ppq, SegmentsMap & smap, REAL rotxx)
{
    typedef priority_queue<Segment<REAL> *, std::vector<Segment<REAL> *>, SegmentComparator<REAL> > SegmentPriorityQueue;
    //typedef map<pair<long, long>, Segment<REAL> *, SegmentMapComparator>                                     SegmentsMap;
    typedef typename SegmentsMap::iterator                                                           SegmentsMapIterator;

    SegmentPriorityQueue spq( (SegmentComparator<REAL>()) );
    //SegmentsMap         smap( (SegmentMapComparator()) );
    while (! ppq.empty())
    {
        Point<REAL> * pt = ppq.top();
        ppq.pop();
        out << " ~~~~~~    " << pt << endl;

        Segment<REAL> * s1 = NULL, * s2 = NULL;
        if (            pt->n1->x < rotxx  ||  Geometry::IsZero(rotxx - pt->n1->x))
        {
            if ( smap.find( make_pair(pt->n1->id, pt->id) ) == smap.end() )
                spq.push( (s1 = new Segment<REAL> (pt->n1, pt)) );
        }
        if (pt->n2  &&  (pt->n2->x < rotxx || Geometry::IsZero(rotxx - pt->n2->x)))
        {
            if ( smap.find( make_pair(pt->n2->id, pt->id) ) == smap.end() )
                spq.push( (s2 = new Segment<REAL> (pt->n2, pt)) );
        }

        SegmentsMapIterator it1, it2;
        if (s1) out << "  #>-{s1}-> " << s1 << endl;
        if (s2) out << "  #>-{s2}-> " << s2 << endl;
        switch (pt->type)
        {
            case  PT_BirthInField :
                                if (!s2)
                                {
                                    s2 = new Segment<REAL> (pt, pt->n2);
                                    smap[ make_pair(pt->id, pt->n2->id) ] = s2;
                                }
            case PT_BirthOnBorder :
                                if (!s1)
                                {
                                    s1 = new Segment<REAL> (pt, pt->n1);
                                    smap[ make_pair(pt->id, pt->n1->id) ] = s1;
                                }
                                break;;
            case     PT_Collision :
                                if (!s2)
                                {
                                    it2 = smap.find( make_pair(pt->n2->id, pt->id) );
                                    assert(it2 != smap.end());
                                    s2 = it2->ND;
                                }
            case PT_DeathOnBorder :
                                if (!s1)
                                {
                                    it1 = smap.find( make_pair(pt->n1->id, pt->id) );
                                    assert(it1 != smap.end());
                                    s1 = it1->ND;
                                }
                                break;;
            case        PT_Update :
                                if (!s1)
                                {
                                    it1 = smap.find( make_pair(pt->n1->id, pt->id) );
                                    assert(it1 != smap.end());
                                    s1 = it1->ND;
                                }
                                s2 = new Segment<REAL> (pt, pt->n2);
                                smap[ make_pair(pt->id, pt->n2->id)] = s2;
                                break;;
            default:
                assert("WRONG POINT TYPE" && false);
        }
        if (s1) out << "   >-{s1}-> " << s1 << endl;
        if (s2) out << "   >-{s2}-> " << s2 << endl;
        evts->Insert( new OldEvent(pt, s1, s2) );
        //*
        out << " MAPA : ";
        FOREACH(it, smap)
        {
            out << " {" << it->ST.ST << "," << it->ST.ND << "}";
        }
        out << endl;
        // */
    }
    // */

    while(! spq.empty())
    {
        Segment<REAL> * s = spq.top();
        spq.pop();

        line->Insert( s->GetP(), s );
    }
    return;
}


#endif // PREPARE_HPP_INCLUDED
