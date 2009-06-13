#ifndef TESTING_HPP_INCLUDED
#define TESTING_HPP_INCLUDED

/*
inline
int pmf_which_neighbor_is_id(pmf_point<double> * pt, long id)
{
    if (pt->n1 != NULL  &&  pt->n1->id == id) return 1;
    if (pt->n2 != NULL  &&  pt->n2->id == id) return 2;
    return 0;
}
//*/


#define DIST(PT1, PT2) (sqrt(((PT1)->x-(PT2)->x)*((PT1)->x-(PT2)->x)+((PT1)->y-(PT2)->y)*((PT1)->y-(PT2)->y)))
template <class T_REAL>
bool
PMF<T_REAL> :: TestVirtualLengthsOfPoint(pmf_point<T_REAL> * pt)
{
    return true;
    pmf_point<T_REAL> * n1 = pt->n1;
    pmf_point<T_REAL> * n2 = pt->n2;
#if pmf_LOG_ADD
    out << " Testing point : " << *pt << endl;
#endif
    assert(n1 || n2);
    if (n1)
    {
#if pmf_LOG_ADD
        out << " Neighbor 1 : " << *n1 << endl;
#endif
        int nans = pmf_which_neighbor_is_id(pt, n1->id);
        assert(nans == 1);

        /// TODO (Rafel#1#): Sprawdzic ostrosc nierownosci
        assert(DIST(pt, n1) <= pt->l1 + EPSILON);
        long ans = pmf_which_neighbor_is_id(n1, pt->id);
        if (ans == 1) {
            //assert(pt->l1 == n1->l1);
            assert(abs(pt->l1 - n1->l1) < EPSILON);
        }
        else if (ans == 2) {
            //assert(pt->l1 == n1->l2);
            assert(abs(pt->l1 - n1->l2) < EPSILON);
        }
        else return false;
#if pmf_LOG_ADD
        out << " Neighbor 1 : OK " << endl;
#endif
    }
    if (n2)
    {
#if pmf_LOG_ADD
        out << " Neighbor 2 : " << *n2 << endl;
#endif
        int nans = pmf_which_neighbor_is_id(pt, n2->id);
        assert(nans == 2);

        /// TODO (Rafel#1#): sprawdzic ostrosc nierownosci
        assert(DIST(pt, n2) <= pt->l2 + EPSILON);
        long ans = pmf_which_neighbor_is_id(n2, pt->id);
        if (ans == 1) {
            //assert(pt->l2 == n2->l1);
            assert(abs(pt->l2 - n2->l1) < EPSILON);
        }
        else if (ans == 2) {
            //assert(pt->l2 == n2->l2);
            assert(abs(pt->l2 - n2->l2) < EPSILON);
        }
        else return false;
#if pmf_LOG_ADD
        out << " Neighbor 2 : OK " << endl;
#endif
    }
    return true;
}
#undef DIST


template <class T_REAL>
bool
PMF<T_REAL> :: TestConfigurationPoints()
{
    Element<pmf_point<T_REAL> > * iterPt = pmfConf->getHead();
    while (iterPt)
    {
        assert( TestVirtualLengthsOfPoint( iterPt->data ) );
        iterPt = iterPt->next;
    }
    return true;
}


#endif // TESTING_HPP_INCLUDED
