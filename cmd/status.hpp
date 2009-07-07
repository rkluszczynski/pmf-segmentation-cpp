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
                bool operator() (const ENTRY & lhs, const ENTRY & rhs) const { return _slst.BelowComparator(lhs, rhs); }
            private :
                const SweepLineStatus & _slst;
        };

        typedef std::set<ENTRY, SweepComparator> STATUS;


        public :
            typedef typename STATUS::iterator Iterator;
            typedef typename STATUS::const_iterator ConstInterator;

            SweepLineStatus() : _st(SweepComparator(*this)), _x0(0.0) {}

            Iterator begin() const { return _st.begin(); }
            Iterator end()   const { return _st.end(); }

            void SetSweepLinePosition(REAL x)
            {
                assert(x >= _x0);
                cout << "!!! SWEEP LINE POSITION set to " << x << "   #######" << endl;
                _x0 = x;
            }

            pair<Iterator,bool> Insert(const POINT pt, SEGMENT * seg)
            {
                SetSweepLinePosition(pt->x + EPSILON);
            //    return Insert(seg);
            //}
            //pair<Iterator,bool> Insert(SEGMENT * seg)
            //{
                assert( _endids.insert( seg->GetQ()->id ).ND );
                return _st.insert( ENTRY(_x0, seg) );
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
                SEGMENT * tmp = it->GetSegment();
                _endids.erase( tmp->GetQ()->id );
                _st.erase(it);
                //delete tmp;
            }

            bool IsNull(Iterator it) { return it == _st.end(); }
            Iterator Find(SEGMENT * seg) { return _st.find( ENTRY(_x0, seg) ); }

            Iterator Above(Iterator it) { return ((it == _st.begin()) ? _st.end() : --it); }
            Iterator Above(SEGMENT * seg) { return Above(Find(seg)); }

            Iterator Below(Iterator it) { return (++it); }
            Iterator Below(SEGMENT * seg) { return Below(Find(seg)); }

            bool BelowComparator(const ENTRY & e1, const ENTRY & e2) const
            {
/// TODO (klusi#3#): cases with vertical segment(s)
                REAL y1 = e1.yy0(_x0);
                REAL y2 = e2.yy0(_x0);
                bool res = (y1 < y2);
                /*
                cout << "[?]  " << _x0 << endl;
                cout << "[?]  " << e1 << "  <  " << e2 << "   ::   (" << y1 << " < " << y2 << ") == " << (res ? "T" : "F") << endl;
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
                FOREACH(it, (*lss)) cout << " " << *it << endl;
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
