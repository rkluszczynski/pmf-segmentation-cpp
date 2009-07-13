#ifndef STATUS_HPP_INCLUDED
#define STATUS_HPP_INCLUDED

#include "../headers/macros.hpp"
#include "../cmd/element.hpp"


namespace pmf
{
    template <class REAL = double>
    class SweepLineStatus
    {
        typedef typename SweepLineElement<REAL>::SEGMENT SEGMENT;
        typedef typename SEGMENT::POINT POINT;
        typedef SweepLineElement<REAL> ENTRY;

        class SweepComparator
        {
            public :
                SweepComparator(const SweepLineStatus & slst) : _slst(slst) {}
                bool operator() (ENTRY * const & lhs, ENTRY * const & rhs) const { return _slst.BelowComparator(lhs, rhs); }
            private :
                const SweepLineStatus & _slst;
        };

        typedef std::set<ENTRY *, SweepComparator> STATUS;


        public :
            typedef typename STATUS::iterator Iterator;
            typedef typename STATUS::const_iterator ConstInterator;

            SweepLineStatus() : _st(SweepComparator(*this)), _x0(-numeric_limits<REAL>::max())  {}

            Iterator begin() const { return _st.begin(); }
            Iterator end()   const { return _st.end(); }

            void SetSweepLinePosition(REAL x)
            {
                using Geometry::IsZero;
                assert(x >= _x0 || IsZero(x - _x0));
                cout << "!!! SWEEP LINE POSITION set to " << x << "   #######" << endl;
                if (x >= _x0) _x0 = x;
            }

            pair<Iterator,bool> Insert(const POINT pt, SEGMENT * seg)
            {
                SetSweepLinePosition(pt->x + EPSILON);
            //    return Insert(seg);
            //}
            //pair<Iterator,bool> Insert(SEGMENT * seg)
            //{
                assert( _endids.insert( seg->GetQ()->id ).ND );
                return _st.insert( new ENTRY(_x0, seg) );
            }
            /*
            void Swap(const POINT & pt, SEGMENT * seg1, SEGMENT * seg2)
            {
                vector<ENTRY> vle;
                _x0 = pt.x() - EPSILON;
                Iterator it1 = Find(seg1);
                Iterator it2 = Find(seg2);
                assert(!IsNull(it1));
                assert(!IsNull(it2));
                ENTRY le1 = *it1;
                ENTRY le2 = *it2;
                if (! BelowComparator(le1, le2)) { swap(it1, it2); }
                ++it2;
                for(Iterator it = it1; it != it2; it++)
                {
                    vle.PB(*it);
                    Remove(it);
                }
                _x0 = pt.x() + EPS;
                REP(i, SIZE(vle))  Insert(vle[i].segment());
            }
            //*/
            void Erase(SEGMENT * seg)
            {
                Iterator it = Find(seg);
                if(IsNull(it))
                {
                    cout << "TO BEDZIE NULLEM  :  " << seg << "   #######" << endl;
                }
                Erase(it);
            }
            void Erase(Iterator it)
            {
                assert(! IsNull(it));
                SEGMENT * tmp = (*it)->GetSegment();
                _endids.erase( tmp->GetQ()->id );
                delete *it;
                _st.erase(it);
                //delete tmp;
            }

            bool IsNull(Iterator it) { return it == _st.end(); }
            Iterator Find(SEGMENT * seg)
            {
                ENTRY * entry = new ENTRY(_x0, seg);
                Iterator it = _st.find( entry );
                delete entry;
                return it;
            }

            Iterator Above(Iterator it) { return ((it == _st.begin()) ? _st.end() : --it); }
            Iterator Above(SEGMENT * seg) { return Above(Find(seg)); }

            Iterator Below(Iterator it) { return (++it); }
            Iterator Below(SEGMENT * seg) { return Below(Find(seg)); }

            bool BelowComparator(ENTRY * const & e1, ENTRY * const & e2) const
            {
                using Geometry::IsZero;
/// TODO (klusi#3#): cases with vertical segment(s)
                SEGMENT * s1 = e1->GetSegment();
                SEGMENT * s2 = e2->GetSegment();

                REAL y1 = e1->yy0(_x0);
                REAL y2 = e2->yy0(_x0);
                //bool res = true;
                bool res = (y1 < y2);
                /*
                cout << "[?]  _________" << _x0 << endl;
                cout << "[?]  " << *e1 << "  <  " << *e2 << "   ::   (" << y1 << " < " << y2 << ") " << endl;
                //*/
                if (s1 != s2  &&  s1->GetP() == s2->GetP())
                {
                    //cout << "[?]  EQUAL  ";
                    if (s1->isVertical() && !s2->isVertical())
                    {
                        //cout << "#######  1   " << s1->GetQ()->y << "   ";
                        if (y1 < s1->GetQ()->y) res = false;
                        else res = true;
                    }
                    else if (!s1->isVertical() && s2->isVertical())
                    {
                        //cout << "#######  2   " << s2->GetQ()->y << "   ";
                        if (y2 < s2->GetQ()->y) res = true;
                        else res = false;
                    }
                    else if (s1->isVertical() && s2->isVertical())
                    {
                        //cout << "#######  3   ";
                        assert(!(s1->isVertical() && s2->isVertical()));
                    }
                }
                /*
                cout.precision(12);
                cout << "[?]  (" << y1 << " < " << y2 << ") == " << (res ? "T" : "F") << endl;
                //*/
                return res;
                //return e1.yy0(_x0) < e2.yy0(_x0);// ||
                    //(e1.y0(x0) == e2.y0(x0)  //&&  (e1.type() == SOURCE || e2.type() == SOURCE)
                      //  &&  e1.segment()->slope() < e2.segment()->slope());
            }

            bool HasSegmentWithEndId(long id) { return _endids.find(id) != _endids.end(); }

            friend ostream & operator << (ostream & out, const SweepLineStatus<REAL> * lss)
            {
                out << "[ STATUS SEGMENTS ] : at position " << lss->_x0 << endl;
                FOREACH(it, (*lss)) cout << " " << *(*it) << endl;
                out << "[ STATUS IDS ] :";
                FOREACH(it, (lss->_endids)) cout << " " << *it;
                out << endl;
                return out;
            }


        private :
            STATUS _st;
            REAL _x0;
            set<long> _endids;

    };

}

#endif // STATUS_HPP_INCLUDED
