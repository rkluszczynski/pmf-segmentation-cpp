#ifndef DETECTORSWEEPLINE_HPP
#define DETECTORSWEEPLINE_HPP

#include <set>

#include "SweepLineElement.hpp"


class DetectorSweepLine
{
    typedef SweepLineElement::SEGMENT SEGMENT;
    typedef SEGMENT::POINT POINT;
    typedef SweepLineElement ENTRY;

    class SweepLineEntryComparator
    {
        public :
            SweepLineEntryComparator(const DetectorSweepLine & dslst) : _dslst(dslst) {}
            bool operator() (ENTRY * const & lhs, ENTRY * const & rhs) const { return _dslst.BelowComparator(lhs, rhs); }
        private :
            const DetectorSweepLine & _dslst;
    };
    typedef std::set<ENTRY *, SweepLineEntryComparator> STATUS;

    public:
        typedef STATUS::iterator Iterator;
        typedef STATUS::const_iterator ConstInterator;

        DetectorSweepLine();
        virtual ~DetectorSweepLine();

        inline Iterator begin() const { return _st->begin(); }
        inline Iterator end()   const { return _st->end(); }

        inline bool IsNull(Iterator it) { return it == _st->end(); }

        std::pair<Iterator,bool>
        Insert(double x, SEGMENT * seg)
        {
            _x0 = x;
            return _st->insert( (new ENTRY(_x0, seg)) );
        }

        void Erase(SEGMENT * seg)
        {
            Iterator it = Find(seg);
            Erase(it);
        }

        void Erase(Iterator it)
        {
            delete *it;
            _st->erase(it);
        }

        Iterator Find(SEGMENT * seg)
        {
            ENTRY * entry = new ENTRY(_x0, seg);
            Iterator it = _st->find( entry );
            delete entry;
            return it;
        }

        inline Iterator Above(Iterator it)   { return ((it == _st->begin()) ? _st->end() : --it); }
        inline Iterator Above(SEGMENT * seg) { return Above(Find(seg)); }

        inline Iterator Below(Iterator it)   { return (++it); }
        inline Iterator Below(SEGMENT * seg) { return Below(Find(seg)); }

        friend std::ostream & operator << (std::ostream &, const DetectorSweepLine *);

        bool BelowComparator(ENTRY * const &, ENTRY * const &) const;


    protected:
        inline void SetSweepLinePosition(double x)  { _x0 = x; }

    private:
        SweepLineEntryComparator * slec;
        STATUS * _st;
        double _x0;
};

#endif // DETECTORSWEEPLINE_HPP
