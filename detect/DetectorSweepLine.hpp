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


        friend std::ostream & operator << (std::ostream &, const DetectorSweepLine *);

        bool BelowComparator(ENTRY * const &, ENTRY * const &) const;


    protected:
    private:
        SweepLineEntryComparator * slec;
        STATUS * _st;
        double _x0;
};

#endif // DETECTORSWEEPLINE_HPP
