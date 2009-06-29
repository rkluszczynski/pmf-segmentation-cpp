#ifndef SWEEPLINE_HPP_INCLUDED
#define SWEEPLINE_HPP_INCLUDED

#include <set>
#include "lineelement.hpp"

class LineSweepStatus;

class sweepCompare
{
    typedef Types::NT      NT;
    typedef Types::FT      FT;
    typedef LineElement ENTRY;
    public :
        sweepCompare(const LineSweepStatus & st) : _st(st) {}
        bool operator() (const ENTRY & lhs, const ENTRY & rhs) const;
    private :
        const LineSweepStatus & _st;
};

class LineSweepStatus
{
    typedef Types::Segment SEGMENT;
    typedef Types::Point   POINT;
    typedef Types::NT      NT;
    typedef Types::FT      FT;
    typedef LineElement ENTRY;

    public :
        typedef std::set<ENTRY, sweepCompare> STATUS;
        typedef STATUS::iterator Iterator;
        typedef STATUS::const_iterator ConstInterator;


        LineSweepStatus() : _st(sweepCompare(*this)), _x0(0.0) {}

        Iterator begin() const { return _st.begin(); }
        Iterator end()   const { return _st.end(); }


        void SetLine(FT x) { _x0 = x; }

        pair<Iterator,bool> Insert(const POINT & pt, SEGMENT * seg)
        {
            _x0 = pt.x();
            return Insert(seg);
        }
        pair<Iterator,bool> Insert(SEGMENT * seg) { return _st.insert( ENTRY(_x0, seg) ); }

        void Swap(const POINT & pt, SEGMENT * seg1, SEGMENT * seg2)
        {
            vector<ENTRY> vle;
            _x0 = pt.x() - EPS;
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

        void Remove(Iterator it) { _st.erase(it); }
        void Remove(SEGMENT * seg) { _st.erase(Find(seg)); }


        bool IsNull(Iterator it) { return it == _st.end(); }
        Iterator Find(SEGMENT * seg) { return _st.find( ENTRY(_x0, seg) ); }

        Iterator Above(Iterator it) { return ((it == _st.begin()) ? _st.end() : --it); }
        Iterator Above(SEGMENT * seg) { return Above(Find(seg)); }

        Iterator Below(Iterator it) { return (++it); }
        Iterator Below(SEGMENT * seg) { return Below(Find(seg)); }

        bool BelowComparator(const ENTRY & e1, const ENTRY & e2) const
        {
            FT y1 = e1.yy0(_x0);
            FT y2 = e2.yy0(_x0);
            bool res = (y1 < y2);
            //cout << "[?]  " << _x0 << endl;
            //cout << "[?]  " << e1 << "  <  " << e2 << "   ::   (" << y1 << " < " << y2 << ") == " << (res ? "T" : "F") << endl;
            return res;
            //return e1.yy0(_x0) < e2.yy0(_x0);// ||
                //(e1.y0(x0) == e2.y0(x0)  //&&  (e1.type() == SOURCE || e2.type() == SOURCE)
                  //  &&  e1.segment()->slope() < e2.segment()->slope());
        }
        friend ostream & operator << (ostream & out, const LineSweepStatus & lss)
        {
            out << "[ STATUS ] :";
            FOREACH(it, lss)  cout << " " << *it;
            return out;
        }

    private :
        STATUS _st;
        NT _x0;
};

bool sweepCompare::operator() (const ENTRY & lhs, const ENTRY & rhs) const { return _st.BelowComparator(lhs, rhs); }

#endif // SWEEPLINE_HPP_INCLUDED
