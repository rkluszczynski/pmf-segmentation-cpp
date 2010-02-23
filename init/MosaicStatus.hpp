#ifndef MOSAICSTATUS_HPP_INCLUDED
#define MOSAICSTATUS_HPP_INCLUDED

#include <limits>

#include "../init/MosaicElement.hpp"


template <class REAL = double>
class MosaicSweepLineStatus
{
    typedef typename MosaicSweepLineElement<REAL>::SEGMENT SEGMENT;
    typedef typename SEGMENT::POINT POINT;
    typedef MosaicSweepLineElement<REAL> ENTRY;

    class MosaicSweepComparator
    {
        public :
            MosaicSweepComparator(const MosaicSweepLineStatus & mslst) : _mslst(mslst) {}
            bool operator() (ENTRY * const & lhs, ENTRY * const & rhs) const { return _mslst.BelowComparator(lhs, rhs); }
        private :
            const MosaicSweepLineStatus & _mslst;
    };

    typedef std::set<ENTRY *, MosaicSweepComparator> STATUS;


    public :
        typedef typename STATUS::iterator Iterator;
        typedef typename STATUS::const_iterator ConstInterator;

        MosaicSweepLineStatus() : _st(MosaicSweepComparator(*this)), _x0(-std::numeric_limits<REAL>::max())  {}

        Iterator begin() const { return _st.begin(); }
        Iterator end()   const { return _st.end(); }

        REAL GetSweepLinePosition() { return _x0; }
        void SetSweepLinePosition(REAL x)
        {
            _x0 = x;
        }

        pair<Iterator,bool>
        //Iterator
        //void
        Insert(const POINT pt, SEGMENT * seg)
        {
            if (pt) SetSweepLinePosition(pt->x + 1e-9);
            return
                _st.insert( (new ENTRY(_x0, seg)) );
        }
        //*
        Iterator Find(SEGMENT * seg)
        {
            ENTRY * entry = new ENTRY(_x0, seg);
            Iterator it = _st.find( entry );
            delete entry;
            return it;
        }

        inline
        bool IsNull(Iterator it) { return it == _st.end(); }

        void Erase(SEGMENT * seg)
        {
            Iterator it = Find(seg);
            Erase(it);
        }

        void Erase(Iterator it)
        {
            delete *it;
            _st.erase(it);
        }

        Iterator Above(Iterator it) { return ((it == _st.begin()) ? _st.end() : --it); }
        Iterator Above(SEGMENT * seg) { return Above(Find(seg)); }

        Iterator Below(Iterator it) { return (++it); }
        Iterator Below(SEGMENT * seg) { return Below(Find(seg)); }
        //*/

        friend ostream & operator << (ostream & out, const MosaicSweepLineStatus<REAL> * msls)
        {
            out << "[ STATUS SEGMENTS ] : at position " << msls->_x0 << endl;
            FOREACH(it, (*msls)) out << " " << *(*it) << endl;
            out << "\\__________________ : left at position " << msls->_x0 << endl;
            out << endl;
            return out;
        }


        bool BelowComparator(ENTRY * const & e1, ENTRY * const & e2) const
        {
            SEGMENT * s1 = e1->GetSegment();
            SEGMENT * s2 = e2->GetSegment();

            REAL y1 = e1->yy0(_x0);
            REAL y2 = e2->yy0(_x0);

            bool res = (y1 < y2);

            return res;
        }


    private :
        STATUS _st;
        REAL _x0;

};

#endif // MOSAICSTATUS_HPP_INCLUDED
