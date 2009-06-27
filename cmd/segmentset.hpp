#ifndef SEGMENTSET_HPP_INCLUDED
#define SEGMENTSET_HPP_INCLUDED

namespace pmf
{

    class MutableSegmentSet
    {
        typedef double ENTRY;

        class SegmentComparator
        {
            public :
                SegmentComparator(const MutableSegmentSet & set) : m_set(set) {}
                bool operator() (const ENTRY & lhs, const ENTRY & rhs) const { return m_set.IsLessCompare(lhs, rhs); }

            private :
                const MutableSegmentSet & m_set;
        };
        typedef std::set<ENTRY, SegmentComparator> SegSet;
        typedef SegSet::iterator Iterator;
        typedef SegSet::const_iterator ConstInterator;


        public :

            MutableSegmentSet() : m_set(SegmentComparator(*this)), m_pos(0.0) {}

            void SetCoord(double x) { m_pos = x; }
            void SetCoord(double x, ENTRY & seg1, ENTRY & seg2)
            {
                Erase(Find(seg1));
                Erase(Find(seg2));
                SetCoord(x);
                Insert(seg1);
                Insert(seg2);
            }

            // methods: begin, end
            Iterator begin() { return m_set.begin(); }
            Iterator end() { return m_set.end(); }

            // methods: insert, erase, find
            pair<Iterator,bool> Insert(ENTRY & ent)
            {
                pair<Iterator,bool> res = m_set.insert(ent);
                return res;
            }
            void Erase(Iterator it) { m_set.erase(it); }
            Iterator Find(ENTRY & ent) { return m_set.find(ent); }

            bool
            IsLessCompare(const ENTRY & lhs, const ENTRY & rhs) const
            {
                return lhs < rhs;
            }


        private :
            SegSet m_set;
            double m_pos;

    };

}

#endif // SEGMENTSET_HPP_INCLUDED
