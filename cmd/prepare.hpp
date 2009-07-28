#ifndef PREPARE_HPP_INCLUDED
#define PREPARE_HPP_INCLUDED


    template <class REAL> class PointComparator
    {
        public:
            bool operator() (Point<REAL> * const & p1, Point<REAL> * const & p2) const
            {
                //*
                if (Geometry::IsZero(p1->x - p2->x))
                {
                    if (p1->type == PT_BirthOnBorder  &&  p2->type != PT_BirthOnBorder) return false;
                    if (p2->type == PT_BirthOnBorder  &&  p1->type != PT_BirthOnBorder) return true;
                    if (p1->type == PT_BirthInField   &&  p2->type != PT_BirthInField) return false;
                    if (p2->type == PT_BirthInField   &&  p1->type != PT_BirthInField) return true;
                    if (p1->type == PT_Update         &&  p2->type != PT_Update) return false;
                    if (p2->type == PT_Update         &&  p1->type != PT_Update) return true;
                    if (p1->type == PT_Collision      &&  p2->type != PT_Collision) return false;
                    if (p2->type == PT_Collision      &&  p1->type != PT_Collision) return true;
                    if (p1->type == PT_DeathOnBorder  &&  p2->type != PT_DeathOnBorder) return false;
                    if (p2->type == PT_DeathOnBorder  &&  p1->type != PT_DeathOnBorder) return true;
                }
                //*/
                return p1->x > p2->x;
            }
    };


    template <class REAL> class SegmentComparator
    {
        public:
            bool operator() (Segment<REAL> * const & s1, Segment<REAL> * const & s2) const { return s1->GetP()->x > s2->GetP()->x; }
    };


    class SegmentMapComparator
    {
        public:
            bool operator() (const pair<long,long> & e1, const pair<long,long> & e2) const
            {
                return e1.ST < e2.ST || (e1.ST == e2.ST  &&  e1.ND < e2.ND);
            }
    };


template <class REAL>
void
PMF<REAL> :: PrepareTheEvolution (REAL sinL, REAL cosL, EventsSchedule<REAL> * evts, SweepLineStatus<REAL> * line, REAL rotxx)
{
    typedef priority_queue<Point<REAL> *, std::vector<Point<REAL> *>, PointComparator<REAL> >         PointPriorityQueue;
    typedef priority_queue<Segment<REAL> *, std::vector<Segment<REAL> *>, SegmentComparator<REAL> > SegmentPriorityQueue;
    typedef map<pair<long, long>, Segment<REAL> *, SegmentMapComparator>                                     SegmentsMap;
    typedef typename SegmentsMap::iterator                                                           SegmentsMapIterator;

    if (cf->IsEmpty()) return;
    cf->PrintConfiguration(out);
    RotatePoints2 (sinL, cosL);
    //SavePMF("output/geo-rot.zip", GeoGebraFile);
    out << "[ ROTATED ]" << endl;  FOREACH(it, *cf) out << (*it) << endl;
    SavePMF("../output/geo-rot.zip", GeoGebraFile);

    PointPriorityQueue   ppq( cf->begin(), cf->end(), PointComparator<REAL>() );
    SegmentPriorityQueue spq( (SegmentComparator<REAL>()) );
    SegmentsMap         smap( (SegmentMapComparator()) );

    cf->ClearPointsContainer();
    while (ppq.top()->x < rotxx)
    {
        Point<REAL> * pt = ppq.top();
        cf->PushBack(pt);
        ppq.pop();
        if (ppq.empty()) return;
    }

    while (! ppq.empty())
    {
        Point<REAL> * pt = ppq.top();
        ppq.pop();
        out << " ~~~~~~    " << pt << endl;

        Segment<REAL> * s1 = NULL, * s2 = NULL;
        if (            pt->n1->x < rotxx)  spq.push( (s1 = new Segment<REAL> (pt->n1, pt)) );
        if (pt->n2  &&  pt->n2->x < rotxx)  spq.push( (s2 = new Segment<REAL> (pt->n2, pt)) );

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
        /*
        out << " MAPA : ";
        FOREACH(it, smap)
        {
            out << " {" << it->ST.ST << "," << it->ST.ND << "}";
        }
        out << endl;
        //*/
    }
    //*/

    while(! spq.empty())
    {
        Segment<REAL> * s = spq.top();
        spq.pop();

        line->Insert( s->GetP(), s );
    }
    return;
}


#endif // PREPARE_HPP_INCLUDED
